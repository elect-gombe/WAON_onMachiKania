#define _SUPPRESS_PLIB_WARNING
// TETRIS（キャラクターベース） Rev.1.0 2013/8/22
// 　with テキストVRAMコンポジット出力システム for PIC32MX1xx/2xx by K.Tanaka
// rev.2 PS/2キーボードシステムに対応

#include <plib.h>
#include <stdlib.h>
#include "colortext32.h"
#include "SDFSIO.h"

#include "audio.h"
#include <stdint.h>

// 入力ボタンのポート、ビット定義
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

//sounddata配列　低いド?3オクターブ分の周期カウンタ値、PR3に書き込むと音程設定される
unsigned char sounddata[SIZEOFSOUNDBF] = {0};

unsigned char *cursor; //カーソル位置
unsigned char cursorc; //カーソル色

void printchar(unsigned char x, unsigned char y, unsigned char c, unsigned char n) {
    //座標(x,y)にカラー番号cでテキストコードnを1文字表示
    cursor = TVRAM + y * WIDTH_X + x;
    cursorc = c;
    *(cursor + ATTROFFSET) = cursorc;
    *cursor++ = n;
}

void printstr(unsigned char x, unsigned char y, unsigned char c, unsigned char *s) {
    //座標(x,y)からカラー番号cで文字列sを表示
    cursor = TVRAM + y * WIDTH_X + x;
    cursorc = c;
    while (*s) {
        *(cursor + ATTROFFSET) = cursorc;
        *cursor++ = *s++;
    }
}

void printchar2(unsigned char n) {
    //カーソル位置、設定カラーでテキストコードnを1文字表示
    *(cursor + ATTROFFSET) = cursorc;
    *cursor++ = n;
}

void locate(unsigned char x, unsigned char y, unsigned char c) {
    //カーソル位置とカラー設定
    cursor = TVRAM + y * WIDTH_X + x;
    cursorc = c;
}

void printnumber6(unsigned char x, unsigned char y, unsigned char c, unsigned int s) {
    //座標(x,y)にカラー番号cで得点sを表示（6桁）
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

    //ポートの初期設定
    TRISA = 0x0010; // RA4は入力
    CNPUA = 0x0010; // RA4をプルアップ
    ANSELA = 0x0000; // 全てデジタル
    TRISB = KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT; // ボタン接続ポート入力設定
    CNPUBSET = KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT; // プルアップ設定
    ANSELB = 0x0000; // 全てデジタル
    LATACLR = 2; // RA1=0（ボタンモード）

    RPB13R = 5; //RPB13ピンにOC4を割り当て
    OC4R = 0;
    OC4CON = 0x000e; // Timer3ベース、PWMモード
    OC4CONSET = 0x8000; //OC4スタート
    T3CON = 0x0000; // プリスケーラ1:1
    PR3 = 256;
    T3CONSET = 0x8000; // タイマ3スタート

    T4CONbits.SIDL = 0;
    T4CONbits.TCKPS = 3;
    T4CONbits.T32 = 0;
    T4CONbits.TCS = 0;
    TMR4 = 0; /*とりあえず和音を再生しました。ソースを公開します。ハードは同じで多分大丈夫ですが心配ならローパスフィルターを入れてください。割り込みなどは一切使っていません。
コードはリファクタリングしておきます。*/
    PR4 = CLOCK_FREQ / 8 / SAMPLING_FREQ;
    T4CONbits.ON = 1;

    DmaChnOpen(0, 0, DMA_OPEN_AUTO);

    DmaChnSetEventControl(0, DMA_EV_START_IRQ(_TIMER_4_IRQ));

    DmaChnSetTxfer(0, sounddata, (void*) &OC4RS, sizeof (sounddata), 1, 1);

    DmaChnEnable(0);

    init_composite(); // ビデオ出力システムの初期化

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