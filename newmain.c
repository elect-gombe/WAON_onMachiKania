#define _SUPPRESS_PLIB_WARNING
// TETRIS（キャラクターベース） Rev.1.0 2013/8/22
// 　with テキストVRAMコンポジット出力システム for PIC32MX1xx/2xx by K.Tanaka
// rev.2 PS/2キーボードシステムに対応

#include <plib.h>
#include <stdlib.h>
#include "colortext32.h"
#include "tetris.h"

#include "audio.h"
#include <stdint.h>



#define SAMPLING_FREQ 32000
#define OUTPUT_FREQ 100000
#define CLOCK_FREQ 57300000


typedef unsigned int uint;

void audiotask(void);

//sounddata配列　低いド?3オクターブ分の周期カウンタ値、PR3に書き込むと音程設定される
unsigned char sounddata[SIZEOFSOUNDBF] = {0};

const music_dat_t part1[] = {
#include "humen.h"
};
const music_dat_t part2[] = {
    {240, 240, 40, 100},
    {480, 240, 52, 100},
    {720, 240, 40, 100},
    {960, 240, 52, 100},
    {1200, 240, 40, 100},
    {1440, 240, 52, 100},
    {1680, 240, 40, 100},
    {1920, 240, 52, 100},
    {2160, 240, 45, 100},
    {2400, 240, 57, 100},
    {2640, 240, 45, 100},
    {2880, 240, 57, 100},
    {3120, 240, 45, 100},
    {3360, 240, 57, 100},
    {3600, 240, 45, 100},
    {3840, 240, 57, 100},
    {4080, 120, 44, 100},
    {4200, 120, 44, 100},
    {4320, 120, 56, 100},
    {4560, 240, 44, 100},
    {4800, 240, 56, 100},
    {5040, 240, 40, 100},
    {5280, 240, 52, 100},
    {5520, 240, 40, 100},
    {5760, 240, 52, 100},
    {6000, 240, 45, 100},
    {6240, 240, 57, 100},
    {6480, 240, 45, 100},
    {6720, 240, 57, 100},
    {6960, 240, 45, 100},
    {7200, 240, 57, 100},
    {7440, 240, 47, 100},
    {7680, 240, 48, 100},
    {7920, 240, 50, 100},
    {8160, 240, 38, 100},
    {8640, 240, 38, 100},
    {9120, 240, 38, 100},
    {9360, 240, 45, 100},
    {9600, 240, 43, 100},
    {9840, 240, 36, 100},
    {10080, 240, 48, 100},
    {10560, 240, 48, 100},
    {10800, 240, 36, 100},
    {11040, 240, 45, 100},
    {11280, 240, 43, 100},
    {11760, 240, 47, 100},
    {12000, 240, 59, 100},
    {12480, 240, 59, 100},
    {12960, 240, 52, 100},
    {13200, 240, 44, 100},
    {13440, 240, 52, 100},
    {13680, 240, 45, 100},
    {13920, 240, 52, 100},
    {14160, 240, 45, 100},
    {14400, 240, 52, 100},
    {14640, 480, 45, 100},
    {15600, 240, 40, 100},
    {15840, 240, 52, 100},
    {16080, 240, 40, 100},
    {16320, 240, 52, 100},
    {16560, 240, 40, 100},
    {16800, 240, 52, 100},
    {17040, 240, 40, 100},
    {17280, 240, 52, 100},
    {17520, 240, 45, 100},
    {17760, 240, 57, 100},
    {18000, 240, 45, 100},
    {18240, 240, 57, 100},
    {18480, 240, 45, 100},
    {18720, 240, 57, 100},
    {18960, 240, 45, 100},
    {19200, 240, 57, 100},
    {19440, 120, 44, 100},
    {19560, 120, 44, 100},
    {19680, 120, 56, 100},
    {19920, 240, 44, 100},
    {20160, 240, 56, 100},
    {20400, 240, 40, 100},
    {20640, 240, 52, 100},
    {20880, 240, 40, 100},
    {21120, 240, 52, 100},
    {21360, 240, 45, 100},
    {21600, 240, 57, 100},
    {21840, 240, 45, 100},
    {22080, 240, 57, 100},
    {22320, 240, 45, 100},
    {22560, 240, 57, 100},
    {22800, 240, 47, 100},
    {23040, 240, 48, 100},
    {23280, 240, 50, 100},
    {23520, 240, 38, 100},
    {24000, 240, 38, 100},
    {24480, 240, 38, 100},
    {24720, 240, 45, 100},
    {24960, 240, 43, 100},
    {25200, 240, 36, 100},
    {25440, 240, 48, 100},
    {25920, 240, 48, 100},
    {26160, 240, 36, 100},
    {26400, 240, 45, 100},
    {26640, 240, 43, 100},
    {27120, 240, 47, 100},
    {27360, 240, 59, 100},
    {27840, 240, 59, 100},
    {28320, 240, 52, 100},
    {28560, 240, 44, 100},
    {28800, 240, 52, 100},
    {29040, 240, 45, 100},
    {29280, 240, 52, 100},
    {29520, 240, 45, 100},
    {29760, 240, 52, 100},
    {30000, 480, 45, 100},
    {30960, 240, 45, 100},
    {31200, 240, 52, 100},
    {31440, 240, 45, 100},
    {31680, 240, 52, 100},
    {31920, 240, 45, 100},
    {32160, 120, 52, 100},
    {32280, 120, 52, 100},
    {32400, 240, 45, 100},
    {32640, 240, 52, 100},
    {32880, 240, 44, 100},
    {33120, 240, 52, 100},
    {33360, 240, 44, 100},
    {33600, 240, 52, 100},
    {33840, 240, 44, 100},
    {34080, 240, 52, 100},
    {34320, 240, 44, 100},
    {34560, 240, 52, 100},
    {34800, 240, 45, 100},
    {35040, 240, 52, 100},
    {35280, 240, 45, 100},
    {35520, 240, 52, 100},
    {35760, 240, 45, 100},
    {36000, 120, 52, 100},
    {36120, 120, 52, 100},
    {36240, 240, 45, 100},
    {36480, 240, 52, 100},
    {36720, 240, 44, 100},
    {36960, 240, 52, 100},
    {37200, 240, 44, 100},
    {37440, 240, 52, 100},
    {38640, 240, 45, 100},
    {38880, 240, 52, 100},
    {39120, 240, 45, 100},
    {39360, 240, 52, 100},
    {39600, 240, 45, 100},
    {39840, 120, 52, 100},
    {39960, 120, 52, 100},
    {40080, 240, 45, 100},
    {40320, 240, 52, 100},
    {40560, 240, 44, 100},
    {40800, 240, 52, 100},
    {41040, 240, 44, 100},
    {41280, 240, 52, 100},
    {41520, 240, 44, 100},
    {41760, 240, 52, 100},
    {42000, 240, 44, 100},
    {42240, 240, 52, 100},
    {42480, 240, 45, 100},
    {42720, 240, 52, 100},
    {42960, 240, 45, 100},
    {43200, 240, 52, 100},
    {43440, 240, 45, 100},
    {43680, 240, 52, 100},
    {43920, 240, 45, 100},
    {44160, 240, 52, 100},
    {44400, 240, 44, 100},
    {44640, 240, 52, 100},
    {44880, 240, 44, 100},
    {45120, 240, 52, 100},

};


//外付けクリスタル with PLL (16倍)
#pragma config PMDL1WAY = OFF, IOL1WAY = OFF
#pragma config FPLLIDIV = DIV_1, FPLLMUL = MUL_16, FPLLODIV = DIV_1
#pragma config FNOSC = PRIPLL, FSOSCEN = OFF, POSCMOD = XT, OSCIOFNC = OFF
#pragma config FPBDIV = DIV_1, FWDTEN = OFF, JTAGEN = OFF, ICESEL = ICS_PGx1

unsigned char *cursor; //カーソル位置
unsigned char cursorc; //カーソル色
unsigned char board[25][12]; //ブロックを配置する配列
unsigned int score, highscore; //得点、ハイスコア
unsigned int gcount = 0; //カウンタ、乱数の種に使用
unsigned short keyold; //前回キー入力状態（リピート入力防止用）
char downkeyrepeat; //下キーのリピート制御
unsigned char next; //次のブロックの種類
unsigned char fallspeed, fallcount; //ブロックの落下速度、カウンタ
unsigned char level; //現在のレベル
unsigned char gamestatus;
// gamestatus
// 0:ゲームスタート
// 1:新ブロック出現
// 2:ブロック落下中
// 3:ステージクリア
// 4:ゲームオーバー

unsigned char lines; //消去したライン累積数
const unsigned int scorearray[] = {40, 100, 300, 1200}; //同時消去したライン数による得点

//ブロックの形状、色、向きの初期値定義
const _Block block[7] = {
    {1, 0, -1, 0, -2, 0, COLOR_IBLOCK, 1}, //I
    {1, 0, -1, 0, -1, -1, COLOR_JBLOCK, 3}, //J
    {1, 0, 1, -1, -1, 0, COLOR_LBLOCK, 3}, //L
    {1, 0, 0, -1, -1, -1, COLOR_ZBLOCK, 1}, //Z
    {0, -1, 1, -1, -1, 0, COLOR_SBLOCK, 1}, //S
    {0, -1, -1, 0, -1, -1, COLOR_OBLOCK, 0}, //O
    {1, 0, 0, -1, -1, 0, COLOR_TBLOCK, 3} //T
};

_Block falling; //現在落下中のブロックの構造体
unsigned char blockx, blocky, blockangle, blockno; //現在落下中のブロックの座標、向き、種類

_Music music; //演奏中の音楽構造体
const unsigned short *sounddatap; //ブロック着地効果音配列の位置、演奏中の音楽よりこちらを優先


//musicdata配列　音階,長さ,音階,長さ,・・・・・　最後に音階254で曲終了、253で最初へリピート
//				音階　0:ド?12:上のド?24:その上のド　　255:休符
//				長さ　60分のn秒

//コロブチカ
const unsigned char musicdata1[] = {
    9, 20, 4, 10, 5, 10, 7, 20, 5, 10, 4, 10, 2, 30, 5, 10, 9, 20, 7, 10, 5, 10,
    4, 30, 5, 10, 7, 20, 9, 20, 5, 20, 2, 18, 255, 2, 2, 30, 255, 10,
    10, 30, 12, 10, 14, 20, 12, 10, 10, 10, 9, 30, 5, 10, 9, 20, 7, 10, 5, 10,
    4, 30, 5, 10, 7, 20, 9, 20, 5, 20, 2, 18, 255, 2, 2, 30, 255, 10,
    9, 30, 8, 10, 9, 30, 8, 10, 9, 20, 14, 20, 9, 20, 7, 10, 5, 10,
    4, 30, 5, 10, 7, 20, 9, 20, 5, 20, 2, 18, 255, 2, 2, 30, 255, 10,
    253
};

//カチューシャ
const unsigned char musicdata2[] = {
    9, 30, 11, 10, 12, 30, 9, 10, 12, 9, 255, 1, 12, 10, 11, 10, 9, 10, 11, 20, 4, 20,
    11, 30, 12, 10, 14, 30, 11, 10, 14, 9, 255, 1, 14, 10, 12, 10, 11, 10, 9, 30, 255, 10,
    16, 20, 21, 20, 19, 20, 21, 10, 19, 10, 17, 20, 16, 10, 14, 10, 16, 20, 9, 20,
    255, 10, 17, 20, 14, 10, 16, 30, 12, 10, 11, 10, 4, 10, 12, 10, 11, 10, 9, 30, 255, 10,
    253
};

//トロイカ
const unsigned char musicdata3[] = {
    9, 10,
    14, 29, 255, 1, 14, 9, 255, 1, 14, 9, 255, 1, 14, 10, 13, 10, 14, 10, 16, 30, 13, 10, 9, 20, 255, 10, 9, 10,
    17, 20, 14, 20, 5, 20, 7, 9, 255, 1, 7, 10, 9, 60, 255, 10, 9, 10,
    14, 30, 16, 10, 17, 10, 16, 10, 14, 10, 9, 10, 7, 30, 10, 10, 14, 20, 16, 10, 14, 10,
    9, 30, 10, 10, 9, 10, 7, 10, 4, 10, 5, 10, 2, 60, 255, 10,
    253
};

//一週間
const unsigned char musicdata4[] = {
    9, 10, 12, 10,
    16, 9, 255, 1, 16, 9, 255, 1, 16, 10, 14, 10, 16, 9, 255, 1, 16, 9, 255, 1, 16, 10, 14, 10, 16, 10, 14, 10, 12, 30, 255, 10, 16, 10, 14, 10,
    16, 20, 14, 10, 12, 10, 14, 20, 12, 10, 11, 10, 12, 10, 11, 10, 9, 30, 255, 10, 9, 10, 12, 10,
    16, 9, 255, 1, 16, 9, 255, 1, 16, 10, 14, 10, 16, 9, 255, 1, 16, 9, 255, 1, 16, 10, 14, 10, 16, 10, 14, 10, 12, 30, 255, 10, 16, 10, 14, 10,
    16, 9, 255, 1, 16, 10, 14, 10, 12, 10, 14, 20, 16, 20, 21, 50, 255, 10,
    253
};

//カリンカ
const unsigned char musicdata5[] = {
    9, 20,
    7, 20, 4, 10, 5, 10, 7, 20, 4, 10, 5, 10, 7, 20, 5, 10, 4, 10, 2, 20, 9, 9, 255, 1, 9, 10,
    7, 20, 4, 10, 5, 10, 7, 20, 4, 10, 5, 10, 7, 20, 5, 10, 4, 10, 2, 20, 9, 9, 255, 1, 9, 10,
    7, 20, 4, 10, 5, 10, 7, 20, 4, 10, 5, 10, 7, 20, 5, 10, 4, 10, 2, 20, 9, 9, 255, 1, 9, 10,
    7, 20, 4, 10, 5, 10, 7, 20, 4, 10, 5, 10, 7, 20, 5, 10, 4, 10, 2, 20, 9, 9, 255, 1, 9, 10,
    7, 20, 4, 10, 5, 10, 7, 20, 4, 10, 5, 10, 7, 20, 5, 10, 4, 10, 2, 20, 9, 9, 255, 1, 9, 10,
    7, 20, 4, 10, 5, 10, 7, 20, 4, 10, 5, 10, 7, 20, 5, 10, 4, 10, 2, 20, 14, 60, 255, 20, 12, 60, 255, 20,
    9, 20, 12, 20, 10, 20, 9, 10, 7, 10, 5, 40, 0, 40, 9, 20, 12, 20, 10, 20, 9, 10, 7, 10, 5, 40, 0, 40,
    2, 19, 255, 1, 2, 19, 255, 1, 2, 20, 4, 20, 7, 20, 5, 20, 4, 20, 2, 20, 0, 39, 255, 1, 0, 39, 255, 1, 0, 40, 255, 40,
    9, 20, 12, 20, 10, 20, 9, 10, 7, 10, 5, 40, 0, 40, 9, 20, 12, 20, 7, 20, 9, 20, 5, 40, 0, 40,
    2, 19, 255, 1, 2, 19, 255, 1, 2, 20, 4, 20, 7, 20, 5, 20, 4, 20, 2, 20, 12, 40, 10, 40, 9, 40, 255, 20,
    253
};

//ПАРНАС
const unsigned char musicdata6[] = {
    7, 29, 255, 1, 7, 14, 255, 1, 7, 14, 255, 1, 7, 14, 255, 1, 7, 14, 255, 1, 7, 14, 255, 1, 7, 15, 5, 15, 8, 30, 255, 55,
    12, 15, 7, 14, 255, 1, 7, 15, 8, 14, 255, 1, 8, 14, 255, 1, 8, 14, 255, 1, 8, 15, 7, 15, 5, 15, 7, 14, 255, 1, 7, 14, 255, 1, 7, 30, 255, 30,
    0, 14, 255, 1, 0, 14, 255, 1, 0, 14, 255, 1, 0, 15, 3, 15, 2, 15, 0, 15, 2, 14, 255, 1, 2, 14, 255, 1, 2, 14, 255, 1, 2, 15, 0, 15, 7, 30, 255, 30,
    12, 30, 7, 30, 255, 30, 0, 15, 255, 15, 3, 15, 255, 15, 3, 15, 255, 15, 0, 15, 255, 15, 3, 15, 255, 15, 3, 15, 255, 15,
    0, 15, 2, 15, 3, 45, 0, 15, 5, 15, 3, 15, 2, 45, 255, 15, 2, 15, 3, 15, 5, 45, 2, 15, 7, 15, 5, 15, 3, 45, 255, 15,
    8, 14, 255, 1, 8, 14, 255, 1, 8, 29, 255, 1, 8, 30, 7, 14, 255, 1, 7, 14, 255, 1, 7, 29, 255, 1, 7, 30, 12, 45, 10, 15, 8, 30, 7, 60, 255, 30,
    10, 14, 255, 1, 10, 14, 255, 1, 10, 29, 255, 1, 10, 30, 9, 14, 255, 1, 9, 14, 255, 1, 9, 29, 255, 1, 9, 30,
    8, 14, 255, 1, 8, 14, 255, 1, 8, 14, 255, 1, 8, 14, 255, 1, 8, 14, 255, 1, 8, 15, 7, 60, 255, 30,
    12, 30, 7, 30, 255, 30, 10, 15, 8, 15, 7, 30, 255, 30, 5, 30, 3, 30, 255, 30, 2, 15, 0, 15, 2, 30, 255, 30,
    7, 29, 255, 1, 7, 30, 255, 30, 8, 29, 255, 1, 8, 30, 255, 30, 12, 29, 255, 1, 12, 40, 5, 6, 6, 6, 7, 6, 8, 6, 9, 6, 10, 6, 11, 6, 12, 18, 255, 60,
    253
};

// 各レベルの曲指定
const unsigned char *musicdatap[] = {
    musicdata1, musicdata2, musicdata3, musicdata4, musicdata5, musicdata6
};

//ブロック着地時の効果音（消去したブロック1行?4行用）
const unsigned short soundDong[5][SOUNDDONGLENGTH] = {
    {40000, 40000, 40000, 40000, 40000, 1, 0}, //ブロック着地音
    {40000, 40000, 30000, 30000, 30000, 1, 0}, //1行クリアの音
    {30000, 30000, 30000, 20000, 20000, 20000, 0}, //2行クリアの音
    {30000, 30000, 30000, 10000, 10000, 10000, 0}, //3行クリアの音
    {16000, 14000, 12500, 11000, 9500, 8000, 0} //4行クリアの音
};

//背景画像コード 横24×縦14キャラクター
const unsigned char bitmap1[] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x62, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x63, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x64, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x65, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x66, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x67, 0x68, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x69, 0x6a, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x6b, 0x6c, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x6d, 0x6e, 0x6f, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x70, 0x71, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x72, 0x73, 0x74, 0x75, 0x76, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x20, 0x20,
    0x20, 0x20, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x8b, 0x8c, 0x8d, 0x20, 0x8e, 0x8f, 0x90, 0x20,
    0x20, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x9a, 0x9b, 0x9c, 0x20, 0x9d, 0x9e, 0x20, 0x20,
    0x20, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0x20, 0x20, 0x20, 0x20, 0x20, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0x20, 0x20,
    0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0x20, 0x20, 0x20, 0x20, 0x20, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0x20, 0x20,
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0x20, 0x20, 0x20, 0x20, 0x20, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0x20,
    0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0x20, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8,
    0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0x20, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
};

void wait60thsec(unsigned short n) {
    // 60分のn秒ウェイト（ビデオ画面の最下行信号出力終了まで待つ）
    drawcount = 0;
    while (drawcount < n) asm("wait");
}

unsigned char startkeycheck(unsigned short n) {
    // 60分のn秒ウェイト
    // スタートボタンが押されればすぐ戻る
    //　戻り値　スタートボタン押されれば1、押されなければ0
    drawcount = 0;
    while (drawcount < n) {
        if (~KEYPORT & KEYSTART) return 1;
        asm("wait");
        gcount++;
    }
    return 0;
}

void playmusic1step(void) {
    //演奏中の曲を1つ進める
    if (music.stop) return; //演奏終了済み
    music.count--;
    if (music.count > 0) {
        PR3 = music.pr;
        return;
    }
    //次の音を鳴らす
    if (*music.p == 254) { //曲終了
        music.stop = 1;
        music.pr = 0;
        PR3 = 0;
        return;
    }
    if (*music.p == 253) { //曲の最初に戻る
        music.p = music.startp;
    }
    if (*music.p == 255) {
        music.pr = 0;
        PR3 = 0; //休符
    } else {
        music.pr = sounddata[*music.p]; //周期データ
        PR3 = music.pr;
    }
    music.p++;
    music.count = *music.p; //音符長さ
    music.p++;
}

void playmusic60thsec(void) {
    //60分の1秒ウェイト後、現在演奏中の曲を1つ進める
    wait60thsec(1); //60分の1秒ウェイト
    //	playmusic1step();
    musicTask();
}

void wait60thsecmusic(unsigned short n) {
    //60分のn秒間、曲を演奏しながらウェイト
    while (n--) playmusic60thsec();
}

void startmusic(unsigned char *m) {
    music.p = m;
    music.startp = m;
    music.count = 1;
    music.stop = 0;
}

void stopmusic(void) {
    music.stop = 1;
    music.pr = 0;
    PR3 = 0;
}

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

void writehighscore(void) {
    //ハイスコアのフラッシュ書き込み
    stop_composite(); //表示停止
    NVMErasePage((void *) FLASHPAGE); //ページ消去
    NVMWriteWord((void *) FLASHPAGE, highscore); //ワード書き込み
    start_composite(); //表示開始
}

void printnext(void) {
    //NEXTエリアに次のブロックを表示
    const _Block *bp;
    bp = &block[next];
    printstr(25, 19, 0, "    ");
    printstr(25, 20, 0, "    ");
    printstr(25, 21, 0, "    ");
    printstr(25, 22, 0, "    ");
    printchar(27, 21, bp->color, CODE_BLOCK);
    printchar(27 + bp->x1, 21 + bp->y1, bp->color, CODE_BLOCK);
    printchar(27 + bp->x2, 21 + bp->y2, bp->color, CODE_BLOCK);
    printchar(27 + bp->x3, 21 + bp->y3, bp->color, CODE_BLOCK);
}

void show(void) {
    //board配列の内容を画面に表示
    char x, y;
    for (y = 1; y <= 23; y++) {
        for (x = 1; x <= 10; x++) {
            printchar(11 + x, y, board[y][x], CODE_BLOCK);
        }
    }
}

void displayscore(void) {
    //得点表示
    printnumber6(0, 16, 7, score);
    printnumber6(0, 19, 7, highscore);
}

char check(_Block *bp, char x, char y) {
    //x,yの位置に_Block構造体bl（ポインタ渡し）をおけるかチェック
    //戻り値　0:おける　-1:おけない
    if (board[y][x]) return -1;
    if (board[y + bp->y1][x + bp->x1]) return -1;
    if (board[y + bp->y2][x + bp->x2]) return -1;
    if (board[y + bp->y3][x + bp->x3]) return -1;
    return 0;
}

void putblock(void) {
    //board配列に落下中のブロックを書き込み
    _Block *bp;
    bp = &falling;
    board[blocky][blockx] = bp->color;
    board[blocky + bp->y1][blockx + bp->x1] = bp->color;
    board[blocky + bp->y2][blockx + bp->x2] = bp->color;
    board[blocky + bp->y3][blockx + bp->x3] = bp->color;
}

void eraseblock(void) {
    //board配列から落下中のブロックを消去
    _Block *bp;
    bp = &falling;
    board[blocky][blockx] = COLOR_SPACE;
    board[blocky + bp->y1][blockx + bp->x1] = COLOR_SPACE;
    board[blocky + bp->y2][blockx + bp->x2] = COLOR_SPACE;
    board[blocky + bp->y3][blockx + bp->x3] = COLOR_SPACE;
}

char newblock(void) {
    //次のブロック出現
    //戻り値：通常0、置けなければ-1（ゲームオーバー）
    const _Block *blockp;

    blockp = &block[next];
    falling.x1 = blockp->x1;
    falling.y1 = blockp->y1;
    falling.x2 = blockp->x2;
    falling.y2 = blockp->y2;
    falling.x3 = blockp->x3;
    falling.y3 = blockp->y3;
    falling.color = blockp->color;
    falling.rot = blockp->rot;
    blockx = 6;
    blocky = 3;
    blockangle = 0;
    blockno = next;
    fallcount = fallspeed;
    next = rand() % 7;
    if (check(&falling, blockx, blocky)) return -1;
    printnext(); //NEXTの場所に次のブロック表示
    putblock(); //落下開始のブロック配置
    downkeyrepeat = 0; //下キーのリピートを阻止
    return 0;
}

void displaylevel(void) {
    //レベル表示
    printstr(13, 13, 7, "LEVEL");
    printnumber6(14, 13, 7, level);
    wait60thsecmusic(60 * 3);
    printstr(13, 13, 0, "        ");
    printstr(1, 22, 0, "     ");
    printnumber6(0, 22, 7, lines);
    printnumber6(0, 25, 7, level);
}

void moveblock(void) {
    //ブロックの落下、キー入力チェックで回転、移動
    //落下できない場合はgamestatus=1とする

    unsigned short k;
    _Block tempblock;
    const _Block *blockp;
    char movedflag;

    movedflag = 0;

    // ボタンチェック
    k = ~KEYPORT & (KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT);
    if (keyold != KEYUP && k == KEYUP) { //上ボタン（回転）
        if (blockangle < falling.rot) { //軸中心に90度回転
            tempblock.x1 = -falling.y1;
            tempblock.y1 = falling.x1;
            tempblock.x2 = -falling.y2;
            tempblock.y2 = falling.x2;
            tempblock.x3 = -falling.y3;
            tempblock.y3 = falling.x3;
            if (check(&tempblock, blockx, blocky) == 0) {
                falling.x1 = tempblock.x1;
                falling.y1 = tempblock.y1;
                falling.x2 = tempblock.x2;
                falling.y2 = tempblock.y2;
                falling.x3 = tempblock.x3;
                falling.y3 = tempblock.y3;
                blockangle++;
                movedflag = -1;
            }
        } else { //回転を初期位置に戻す場合
            blockp = &block[blockno];
            tempblock.x1 = blockp->x1;
            tempblock.y1 = blockp->y1;
            tempblock.x2 = blockp->x2;
            tempblock.y2 = blockp->y2;
            tempblock.x3 = blockp->x3;
            tempblock.y3 = blockp->y3;
            if (check(&tempblock, blockx, blocky) == 0) {
                falling.x1 = tempblock.x1;
                falling.y1 = tempblock.y1;
                falling.x2 = tempblock.x2;
                falling.y2 = tempblock.y2;
                falling.x3 = tempblock.x3;
                falling.y3 = tempblock.y3;
                blockangle = 0;
                movedflag = -1;
            }
        }
    } else if (keyold != KEYRIGHT && k == KEYRIGHT) { //右ボタン
        if (check(&falling, blockx + 1, blocky) == 0) blockx++;
        movedflag = -1;
    } else if (keyold != KEYLEFT && k == KEYLEFT) { //左ボタン
        if (check(&falling, blockx - 1, blocky) == 0) blockx--;
        movedflag = -1;
    } else if (downkeyrepeat && k == KEYDOWN) { //下ボタン
        if (check(&falling, blockx, blocky + 1) == 0) {
            blocky++;
            movedflag = -1;
            score++;
            if (score > highscore) {
                highscore = score;
            }
        }
    }
    if ((k & KEYDOWN) == 0) {
        downkeyrepeat = -1; // 新ブロック出現時の下キーリピート解除
    }
    keyold = k;

    fallcount--;
    if (fallcount == 0) { //自然落下
        fallcount = fallspeed;
        if (check(&falling, blockx, blocky + 1)) gamestatus = 1; //着地完了（固定）
        else {
            blocky++;
            movedflag = -1;
        }
    }
    if (movedflag) {
        if (check(&falling, blockx, blocky + 1)) {
            sounddatap = soundDong[0]; //着地音
        }
    }
}

void linecheck(void) {
    //完成ラインのチェックと消去、得点加算
    char x, y, x2, y2, c, cleared, cleared2;

    //消去するラインがあれば白いブロックに変更
    cleared = 0;
    y = blocky + 2;
    if (y > 23) y = 23;
    while (y >= blocky - 2) {
        c = 0;
        for (x = 1; x <= 10; x++) {
            if (board[y][x]) c++;
        }
        if (c == 10) {
            cleared++;
            locate(12, y, COLOR_CLEARBLOCK);
            for (x = 1; x <= 10; x++) printchar2(CODE_CLEARBLOCK);
        }
        y--;
    }
    if (cleared == 0) return;

    wait60thsecmusic(15); //60分の15秒待ち

    //白いブロックの行を消去して、一番上に獲得した得点表示
    y = blocky + 2;
    if (y > 23) y = 23;
    cleared2 = cleared;
    while (cleared2 > 0) {
        c = 0;
        for (x = 1; x <= 10; x++) {
            if (board[y][x]) c++;
        }
        if (c == 10) {
            printstr(12, y, 0, "          ");
            cleared2--;
        }
        y--;
    }
    printnumber6(12, y + 1, 7, scorearray[cleared - 1]);

    wait60thsecmusic(15); //60分の15秒待ち

    //消去した行の分、全体を落下させる
    cleared = 0;
    y = blocky + 2;
    if (y > 23) y = 23;
    while (y >= blocky - 2 && cleared < 4) {
        c = 0;
        for (x = 1; x <= 10; x++) {
            if (board[y][x]) c++;
        }
        if (c == 10) {
            cleared++;
            for (y2 = y; y2 > 0; y2--) {
                for (x2 = 1; x2 <= 10; x2++) {
                    board[y2][x2] = board[y2 - 1][x2];
                }
            }
        } else y--;
    }

    //消去した行数に合わせて得点加算
    score += scorearray[cleared - 1];
    if (score > highscore) {
        highscore = score;
    }
    lines += cleared;
    sounddatap = soundDong[cleared]; //消去した行数に合わせた効果音を鳴らす
    printnumber6(0, 22, 7, lines);
}

void gameinit(void) {
    //リセット後1回だけ呼ばれる初期化

    //カラーパレット定義
    set_palette(COLOR_WALL, 200, 200, 200); //壁の色
    set_palette(COLOR_LBLOCK, 0, 255, 165); //L形ブロック色
    set_palette(COLOR_BITMAP, 0xde, 0xb0, 0xc4); //背景画像の色
    set_palette(COLOR_BRICK, 0x22, 0xb2, 0x22); //背景画像の色

    if (*(unsigned int *) FLASHPAGE == -1) { //フラッシュメモリにハイスコアが書き込まれているかチェック
        highscore = 1000;
        writehighscore();
    } else highscore = *(unsigned int *) FLASHPAGE;
    score = 0;
}

void gameinit2(void) {
    //ゲームスタートボタン押下後に呼ばれる初期化
    int i;
    unsigned char y;
    const unsigned char *p;
    score = 0;
    level = 0;
    fallspeed = 60;
    clearscreen();
    locate(0, 15, COLOR_BRICK);
    for (i = 0; i < 12 * 30; i++) printchar2(CODE_BRICK);

    // 背景表示
    p = bitmap1;
    for (y = 0; y <= 13; y++) {
        locate(0, y, COLOR_BITMAP);
        for (i = 0; i <= 10; i++) printchar2(*p++);
        p += 13;
    }
    p = bitmap1 + 16;
    for (y = 0; y <= 13; y++) {
        locate(23, y, COLOR_BITMAP);
        for (i = 23; i <= 29; i++) printchar2(*p++);
        p += 17;
    }

    // 得点表示枠を描画
    printchar(0, 14, COLOR_FRAME, 0x04);
    for (i = 1; i <= 8; i++) printchar2(0x07);
    printchar2(0x08);
    for (y = 15; y <= 25; y++) {
        printchar(0, y, COLOR_FRAME, 0x05);
        printstr(1, y, COLOR_FRAME, "        ");
        printchar2(0x05);
    }
    printchar(0, 26, COLOR_FRAME, 0x06);
    for (i = 1; i <= 8; i++) printchar2(0x07);
    printchar2(0x09);

    printstr(2, 15, 7, "SCORE");
    printstr(1, 18, 7, "HI-SCORE");
    printstr(2, 21, 7, "LINES");
    printstr(2, 24, 7, "LEVEL");

    //NEXT表示枠を描画
    printchar(24, 16, COLOR_FRAME, 0x04);
    for (i = 1; i <= 4; i++) printchar2(0x07);
    printchar2(0x08);
    for (y = 17; y <= 22; y++) {
        printchar(24, y, COLOR_FRAME, 0x05);
        printstr(25, y, COLOR_FRAME, "    ");
        printchar2(0x05);
    }
    printchar(24, 23, COLOR_FRAME, 0x06);
    for (i = 1; i <= 4; i++) printchar2(0x07);
    printchar2(0x09);
    printstr(25, 18, 7, "NEXT");

    locate(11, 0, COLOR_WALL);
    for (i = 11; i <= 22; i++) printchar2(CODE_WALL);
    for (y = 1; y <= 23; y++) {
        printchar(11, y, COLOR_WALL, CODE_WALL);
        for (i = 12; i <= 21; i++) printchar2(' ');
        printchar(22, y, COLOR_WALL, CODE_WALL);
    }
    locate(11, 24, COLOR_WALL);
    for (i = 11; i <= 22; i++) printchar2(CODE_WALL);
    displayscore();
    next = rand() % 7;
    printnext(); //NEXTの場所に次のブロック表示
    sounddatap = soundDong[0] + SOUNDDONGLENGTH - 1;
}

void gameinit3(void) {
    //各レベルごとに呼ばれる初期化
    char x, y;
    lines = 0; //消去ライン数クリア
    level++;

    //落下速度更新
    if (fallspeed <= 8) fallspeed = 3;
    else fallspeed -= 5;

    //ゲームエリアの初期化
    for (x = 0; x < 12; x++) {
        for (y = 0; y < 25; y++) {
            if (x == 0 || x == 11 || y == 24) {
                board[y][x] = COLOR_WALL;
            } else {
                board[y][x] = COLOR_SPACE;
            }
        }
    }
    startmusic((unsigned char *) musicdatap[(level - 1) % (sizeof musicdatap / sizeof musicdatap[0])]); //各レベルの音楽開始
    keyold = ~KEYPORT & (KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT);
    srand(gcount);
}

void gameover(void) {
    //ゲームオーバー
    printstr(13, 13, 7, "GAME OVER");
    wait60thsecmusic(60 * 5); //5秒ウェイト
    stopmusic();
    if (highscore>*(unsigned int *) FLASHPAGE) writehighscore(); //フラッシュにハイスコア書き込み
}

void title(void) {
    //タイトル画面表示
    unsigned char x, y, c;
    const unsigned char *p;
    clearscreen();

    //背景画像表示
    p = bitmap1;
    for (y = 0; y < 14; y++) {
        locate(3, y + 3, COLOR_BITMAP);
        for (x = 0; x < 24; x++) {
            printchar2(*p++);
        }
    }

    //タイトルロゴ表示
    c = 0x0a;
    locate(10, 4, 4);
    for (x = 1; x <= 10; x++) printchar2(c++);
    locate(10, 5, 4);
    for (x = 1; x <= 10; x++) printchar2(c++);

    printstr(5, 0, 5, "SCORE");
    printstr(18, 0, 5, "HI-SCORE");
    printnumber6(5, 1, 7, score);
    printnumber6(20, 1, 7, highscore);

    printstr(3, 19, 5, "LEFT   \x5c");
    printstr(3, 20, 5, "RIGHT  \x5b");
    printstr(3, 21, 5, "ROTATE \x1e");
    printstr(3, 22, 5, "DOWN   \x1f");
    printstr(17, 19, 7, "\x5eKENKEN");
    printstr(13, 20, 7, "WITH COLOR-TEXT");
    printstr(13, 21, 7, "VIDEO OUT SYSTEM");
    printstr(15, 22, 7, "FOR PIC32MX");

    printstr(6, 25, 6, "PUSH START BUTTON");
    while (1) {
        if (startkeycheck(600)) return;
    }
}

void game(void) {
    // gamestatus
    // 0:ゲームスタート
    // 1:新ブロック出現
    // 2:ブロック落下中
    // 3:ステージクリア
    // 4:ゲームオーバー
    gameinit2();
    gamestatus = 0;
    while (gamestatus < 4) {
        gameinit3();
        displaylevel(); //レベル表示
        gamestatus = 1;
        while (gamestatus < 3) {
            if (newblock()) gamestatus = 4; //新ブロック出現、ゲームオーバーチェック
            else gamestatus = 2;
            drawcount = 0;
            while (gamestatus == 2) {
                while (drawcount == 0) asm("wait"); //アイドルにして割り込み待ち
                musicTask();
                drawcount = 0;
                eraseblock(); //ブロック消去
                moveblock(); //ブロック移動、着地完了チェック
                putblock(); //ブロック配置
                if (gamestatus == 1) { //ブロック着地完了の場合
                    linecheck(); //ライン完成チェック、完成ライン消去
                    if (lines >= SCENECLEARLINE) gamestatus = 3;
                }
                displayscore();
                show(); //board配列の内容を画面出力
                gcount++;
            }
        }
    }
    gameover();
}

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

    unsigned int timing[2];
    unsigned int time;
    int pos[2] = {0};
    const music_dat_t *part;
    //  mktone(1,1,1,1);
    //  mktone(1,1,1,0);

    gameinit(); //ゲーム全体初期化
    mktone(1, 1, 1, 1);
    mktone(1, 1, 1, 0);
    while (1) {
        title(); //タイトル画面、スタートボタンで戻る
        game(); //ゲームメインループ
    }
}

void musicTask(void) {
    unsigned int time;
    unsigned int i;
    static unsigned int timing[2];
    static unsigned int pos[2];
    music_dat_t *part;
    time = gettime();
    static int isinitialized;
    if(!isinitialized){isinitialized=1;
    for(i=0;i<2;i++){
          part = i?part1:part2;
      timing[i] = part[pos[i]].time*12;
    }
    }
    
    for (i = 0; i < 2; i++) {
        part = i ? part2 : part1;
        if (time>-SAMPLING_FREQ / 40 + timing[i]) {
            mktone( part[pos[i]].key, part[pos[i]].len * 1000 * 25 / SAMPLING_FREQ/* *3/4 */ - 10, part[pos[i]].vel + 120, i);
            pos[i]++;
            timing[i] = part[pos[i]].time * 25;
        }
        if (part[pos[i]].time == -1) {
            pos[0] = 0;
            pos[1] = 0;
            settime(0);
            timing[0] = 3000;
            timing[1] = 3000;
        }
    }

    audiotask();
}

void audiotask(void) {
    static uint prevtrans = 1;
    uint8_t *buff;

#ifdef SIMMODE
    buff = &sounddata[0];
#else
    buff = NULL; //&sounddata[0];
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
        soundTask(buff);
    }
}