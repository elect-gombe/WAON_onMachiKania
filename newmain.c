#define _SUPPRESS_PLIB_WARNING
// TETRIS�i�L�����N�^�[�x�[�X�j Rev.1.0 2013/8/22
// �@with �e�L�X�gVRAM�R���|�W�b�g�o�̓V�X�e�� for PIC32MX1xx/2xx by K.Tanaka
// rev.2 PS/2�L�[�{�[�h�V�X�e���ɑΉ�

#include <plib.h>
#include <stdlib.h>
#include "colortext32.h"
#include "SDFSIO.h"

#include "audio.h"
#include <stdint.h>

// ���̓{�^���̃|�[�g�A�r�b�g��`
#define KEYPORT PORTB
#define KEYUP 0x0400
#define KEYDOWN 0x0080
#define KEYLEFT 0x0100
#define KEYRIGHT 0x0200
#define KEYSTART 0x0800
#define KEYFIRE 0x4000



#define SAMPLING_FREQ 32000
#define OUTPUT_FREQ 100000
#define CLOCK_FREQ 57300000


typedef unsigned int uint;

void audiotask(void);

//sounddata�z��@�Ⴂ�h?3�I�N�^�[�u���̎����J�E���^�l�APR3�ɏ������ނƉ����ݒ肳���
unsigned char sounddata[SIZEOFSOUNDBF] = {0};

unsigned char *cursor; //�J�[�\���ʒu
unsigned char cursorc; //�J�[�\���F

void printchar(unsigned char x, unsigned char y, unsigned char c, unsigned char n) {
    //���W(x,y)�ɃJ���[�ԍ�c�Ńe�L�X�g�R�[�hn��1�����\��
    cursor = TVRAM + y * WIDTH_X + x;
    cursorc = c;
    *(cursor + ATTROFFSET) = cursorc;
    *cursor++ = n;
}

void printstr(unsigned char x, unsigned char y, unsigned char c, unsigned char *s) {
    //���W(x,y)����J���[�ԍ�c�ŕ�����s��\��
    cursor = TVRAM + y * WIDTH_X + x;
    cursorc = c;
    while (*s) {
        *(cursor + ATTROFFSET) = cursorc;
        *cursor++ = *s++;
    }
}

void printchar2(unsigned char n) {
    //�J�[�\���ʒu�A�ݒ�J���[�Ńe�L�X�g�R�[�hn��1�����\��
    *(cursor + ATTROFFSET) = cursorc;
    *cursor++ = n;
}

void locate(unsigned char x, unsigned char y, unsigned char c) {
    //�J�[�\���ʒu�ƃJ���[�ݒ�
    cursor = TVRAM + y * WIDTH_X + x;
    cursorc = c;
}

void printnumber6(unsigned char x, unsigned char y, unsigned char c, unsigned int s) {
    //���W(x,y)�ɃJ���[�ԍ�c�œ��_s��\���i6���j
    x += 6;
    do {
        printchar(x, y, c, '0' + s % 10);
        s /= 10;
        x--;
    } while (s != 0);
}

FSFILE *fhandle;
void main(void) {
    int i;

    //�|�[�g�̏����ݒ�
    TRISA = 0x0010; // RA4�͓���
    CNPUA = 0x0010; // RA4���v���A�b�v
    ANSELA = 0x0000; // �S�ăf�W�^��
    TRISB = KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT; // �{�^���ڑ��|�[�g���͐ݒ�
    CNPUBSET = KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT; // �v���A�b�v�ݒ�
    ANSELB = 0x0000; // �S�ăf�W�^��
    LATACLR = 2; // RA1=0�i�{�^�����[�h�j

    RPB13R = 5; //RPB13�s����OC4�����蓖��
    OC4R = 0;
    OC4CON = 0x000e; // Timer3�x�[�X�APWM���[�h
    OC4CONSET = 0x8000; //OC4�X�^�[�g
    T3CON = 0x0000; // �v���X�P�[��1:1
    PR3 = 256;
    T3CONSET = 0x8000; // �^�C�}3�X�^�[�g

    T4CONbits.SIDL = 0;
    T4CONbits.TCKPS = 3;
    T4CONbits.T32 = 0;
    T4CONbits.TCS = 0;
    TMR4 = 0; /*�Ƃ肠�����a�����Đ����܂����B�\�[�X�����J���܂��B�n�[�h�͓����ő������v�ł����S�z�Ȃ烍�[�p�X�t�B���^�[�����Ă��������B���荞�݂Ȃǂ͈�؎g���Ă��܂���B
�R�[�h�̓��t�@�N�^�����O���Ă����܂��B*/
    PR4 = CLOCK_FREQ / 8 / SAMPLING_FREQ;
    T4CONbits.ON = 1;

    DmaChnOpen(0, 0, DMA_OPEN_AUTO);

    DmaChnSetEventControl(0, DMA_EV_START_IRQ(_TIMER_4_IRQ));

    DmaChnSetTxfer(0, sounddata, (void*) &OC4RS, sizeof (sounddata), 1, 1);

    DmaChnEnable(0);

    init_composite(); // �r�f�I�o�̓V�X�e���̏�����

    int curr=2;
#define FILENAME "MARIO.WAV"
    printstr(3,curr++,3,"SD INIT...");
	if(FSInit()==FALSE){
        printstr(3,curr++,3,"SD INIT ERR");
		while(1) asm("wait");
	} else {
		fhandle = FSfopen(FILENAME,"r");
        if(!fhandle){
            printstr(3,curr++,3,"FILE <"FILENAME"> NOT FOUND");
            while(1) asm("wait");            
        }
        printstr(3,curr++,3,"FILE <"FILENAME"> FOUND");
    }
    while(1){
        int time=0;
        musicTask();
        printnumber6(3,6,3,time++);
    }
}

void musicTask(void) {
    audiotask();
}

void audiotask(void) {
    static uint prevtrans = 1;
    uint8_t *buff;

#ifdef SIMMODE
    buff = &sounddata[0];
#else
    buff = NULL;//&sounddata[0];
#endif
    if (DmaChnGetEvFlags(0) & DMA_EV_SRC_HALF) {
        DmaChnClrEvFlags(0, DMA_EV_SRC_HALF);
        if (prevtrans == 2) {
            prevtrans = 1;
            buff = &sounddata[0];
        }
    } else if (DmaChnGetEvFlags(0) & DMA_EV_SRC_FULL) {
        DmaChnClrEvFlags(0, DMA_EV_SRC_FULL);
        if (prevtrans == 1) {
            prevtrans = 2;
            buff = &sounddata[SIZEOFSOUNDBF / 2];
        }
    }

    if (buff) {
        int i;
        for (i = 0; i < SIZEOFSOUNDBF / 2; i++) {
            buff[i] = 128;
        }
        //soundTask(buff);
        FSfread(buff,1,SIZEOFSOUNDBF / 2,fhandle);
    }
}