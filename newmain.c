
#define _SUPPRESS_PLIB_WARNING
#include "audio.h"
#include <plib.h>
#include "composite32-high4.h"
#include "lib_composite32-high.h"
#include <stdint.h>

//�O�t���N���X�^�� with PLL (15�{)
#pragma config PMDL1WAY = OFF, IOL1WAY = OFF
#pragma config FPLLIDIV = DIV_1, FPLLMUL = MUL_15, FPLLODIV = DIV_1
#pragma config FNOSC = PRIPLL, FSOSCEN = OFF, POSCMOD = XT, OSCIOFNC = OFF
#pragma config FPBDIV = DIV_1, FWDTEN = OFF, JTAGEN = OFF, ICESEL = ICS_PGx1

#define SAMPLING_FREQ 16000
#define OUTPUT_FREQ 100000
#define CLOCK_FREQ 53800000


typedef unsigned int uint;

void audiotask(void);

//sounddata�z��@�Ⴂ�h?3�I�N�^�[�u���̎����J�E���^�l�APR3�ɏ������ނƉ����ݒ肳���
unsigned char sounddata[SIZEOFSOUNDBF]={0};

void main(void){
  int i;
  uint prevtrans;

  //�|�[�g�̏����ݒ�
  TRISA = 0x0010; // RA4�͓���
  CNPUA = 0x0010; // RA4���v���A�b�v
  ANSELA = 0x0000; // �S�ăf�W�^��
  TRISB = KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT;// �{�^���ڑ��|�[�g���͐ݒ�
  CNPUBSET=KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT;// �v���A�b�v�ݒ�
  ANSELB = 0x0000; // �S�ăf�W�^��
  LATACLR=2;// RA1=0�i�{�^�����[�h�j

  RPB13R=5;//RPB13�s����OC4�����蓖��
  OC4R=0;
  OC4CON=0x000e;// Timer3�x�[�X�APWM���[�h
  OC4CONSET=0x8000;//OC4�X�^�[�g
  T3CON=0x0000;// �v���X�P�[��1:1
  PR3=256;
  T3CONSET=0x8000;// �^�C�}3�X�^�[�g
    
  T4CONbits.SIDL = 0;
  T4CONbits.TCKPS = 3;
  T4CONbits.T32 = 0;
  T4CONbits.TCS = 0;
  TMR4 = 0;/*�Ƃ肠�����a�����Đ����܂����B�\�[�X�����J���܂��B�n�[�h�͓����ő������v�ł����S�z�Ȃ烍�[�p�X�t�B���^�[�����Ă��������B���荞�݂Ȃǂ͈�؎g���Ă��܂���B
�R�[�h�̓��t�@�N�^�����O���Ă����܂��B*/
  PR4 = CLOCK_FREQ / 8 / SAMPLING_FREQ;
  T4CONbits.ON = 1;
    
  DmaChnOpen(0, 0, DMA_OPEN_AUTO);

  DmaChnSetEventControl(0, DMA_EV_START_IRQ(_TIMER_4_IRQ));
 
  DmaChnSetTxfer(0, sounddata, (void*)&OC4RS, sizeof(sounddata), 1, 1);
 
  DmaChnEnable(0);
    
  init_composite(); // �r�f�I�o�̓V�X�e���̏�����

  int j;
  while(1){
      j=(j+1)%SOUND_CH;
    mktone(60 + j*4,10200,180,j);   
  
    for(i=0;i<=10;i++){
      //60����n�b�E�F�C�g
      audiotask();      
      drawcount=0;
      
      while(!drawcount);
    }
  }
}


void audiotask(void){
  static uint prevtrans=1;
  uint8_t *buff;

  buff = NULL;

  if(DmaChnGetEvFlags(0)&DMA_EV_SRC_HALF){
    DmaChnClrEvFlags(0,DMA_EV_SRC_HALF);
    if(prevtrans==2){
      prevtrans=1;
      buff = &sounddata[0];
    }
  }else if(DmaChnGetEvFlags(0)&DMA_EV_SRC_FULL){
    DmaChnClrEvFlags(0,DMA_EV_SRC_FULL);
    if(prevtrans==1){
      prevtrans=2;
      buff = &sounddata[SIZEOFSOUNDBF/2];
    }
  }

  if(buff){
      int i;
      for(i=0;i<SIZEOFSOUNDBF/2;i++){
          buff[i]=128;
      }
      soundTask(buff);
  }
}
