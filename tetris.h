// 入力ボタンのポート、ビット定義
#define KEYPORT PORTB
#define KEYUP 0x0400
#define KEYDOWN 0x0080
#define KEYLEFT 0x0100
#define KEYRIGHT 0x0200
#define KEYSTART 0x0800
#define KEYFIRE 0x4000

#define SCENECLEARLINE 20 //ステージクリアの消去ライン数

//各種キャラクターコード定義
#define CODE_BLOCK 0x01
#define CODE_WALL 0x02
#define CODE_BRICK 0x03
#define CODE_SPACE 0
#define CODE_CLEARBLOCK 0x61

//カラー定義
#define COLOR_IBLOCK 2
#define COLOR_JBLOCK 1
#define COLOR_LBLOCK 8
#define COLOR_ZBLOCK 4
#define COLOR_SBLOCK 3
#define COLOR_OBLOCK 6
#define COLOR_TBLOCK 5
#define COLOR_WALL 9
#define COLOR_BITMAP 10
#define COLOR_BRICK 11
#define COLOR_FRAME 4
#define COLOR_SPACE 0
#define COLOR_CLEARBLOCK 7

#define SOUNDDONGLENGTH 7

//ハイスコア書き込みページ（1024バイト単位で消去される）
#define FLASHPAGE 0x9d007c00

//_Block構造体定義
typedef struct {
	//各ブロックの位置は、(0,0)と残り3個分の相対位置で定義
	char x1,y1;
	char x2,y2;
	char x3,y3;
	char color;//色
	char rot; //回転可能回数、これを越えると初期位置に戻す
} _Block;

//_Music構造体定義
typedef struct {
	unsigned char *p; //曲配列の演奏中の位置
	unsigned char *startp; //曲配列のリピート位置
	unsigned char count; //発音中の音カウンタ
	unsigned short pr; //発音中の音（タイマ周期）
	unsigned char stop; //0:演奏中、1:終了
} _Music;
