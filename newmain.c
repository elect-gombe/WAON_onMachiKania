
#define _SUPPRESS_PLIB_WARNING
#include "audio.h"
#include <plib.h>
#include "composite32-high4.h"
#include "lib_composite32-high.h"
#include <stdint.h>

//外付けクリスタル with PLL (15倍)
#pragma config PMDL1WAY = OFF, IOL1WAY = OFF
#pragma config FPLLIDIV = DIV_1, FPLLMUL = MUL_15, FPLLODIV = DIV_1
#pragma config FNOSC = PRIPLL, FSOSCEN = OFF, POSCMOD = XT, OSCIOFNC = OFF
#pragma config FPBDIV = DIV_1, FWDTEN = OFF, JTAGEN = OFF, ICESEL = ICS_PGx1

#define SAMPLING_FREQ 16000
#define OUTPUT_FREQ 100000
#define CLOCK_FREQ 53800000


typedef unsigned int uint;

void audiotask(void);

//sounddata配列　低いド?3オクターブ分の周期カウンタ値、PR3に書き込むと音程設定される
unsigned char sounddata[SIZEOFSOUNDBF]={0};

void main(void){
  int i;
  uint prevtrans;

  //ポートの初期設定
  TRISA = 0x0010; // RA4は入力
  CNPUA = 0x0010; // RA4をプルアップ
  ANSELA = 0x0000; // 全てデジタル
  TRISB = KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT;// ボタン接続ポート入力設定
  CNPUBSET=KEYSTART | KEYFIRE | KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT;// プルアップ設定
  ANSELB = 0x0000; // 全てデジタル
  LATACLR=2;// RA1=0（ボタンモード）

  RPB13R=5;//RPB13ピンにOC4を割り当て
  OC4R=0;
  OC4CON=0x000e;// Timer3ベース、PWMモード
  OC4CONSET=0x8000;//OC4スタート
  T3CON=0x0000;// プリスケーラ1:1
  PR3=256;
  T3CONSET=0x8000;// タイマ3スタート
    
  T4CONbits.SIDL = 0;
  T4CONbits.TCKPS = 3;
  T4CONbits.T32 = 0;
  T4CONbits.TCS = 0;
  TMR4 = 0;/*とりあえず和音を再生しました。ソースを公開します。ハードは同じで多分大丈夫ですが心配ならローパスフィルターを入れてください。割り込みなどは一切使っていません。
コードはリファクタリングしておきます。*/
  PR4 = CLOCK_FREQ / 8 / SAMPLING_FREQ;
  T4CONbits.ON = 1;
    
  DmaChnOpen(0, 0, DMA_OPEN_AUTO);

  DmaChnSetEventControl(0, DMA_EV_START_IRQ(_TIMER_4_IRQ));
 
  DmaChnSetTxfer(0, sounddata, (void*)&OC4RS, sizeof(sounddata), 1, 1);
 
  DmaChnEnable(0);
    
  init_composite(); // ビデオ出力システムの初期化

  int j;
  while(1){
      j=(j+1)%SOUND_CH;
    mktone(60 + j*4,10200,180,j);   
  
    for(i=0;i<=10;i++){
      //60分のn秒ウェイト
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
