// ���̓{�^���̃|�[�g�A�r�b�g��`
#define KEYPORT PORTB
#define KEYUP 0x0400
#define KEYDOWN 0x0080
#define KEYLEFT 0x0100
#define KEYRIGHT 0x0200
#define KEYSTART 0x0800
#define KEYFIRE 0x4000

#define SCENECLEARLINE 20 //�X�e�[�W�N���A�̏������C����

//�e��L�����N�^�[�R�[�h��`
#define CODE_BLOCK 0x01
#define CODE_WALL 0x02
#define CODE_BRICK 0x03
#define CODE_SPACE 0
#define CODE_CLEARBLOCK 0x61

//�J���[��`
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

//�n�C�X�R�A�������݃y�[�W�i1024�o�C�g�P�ʂŏ��������j
#define FLASHPAGE 0x9d007c00

//_Block�\���̒�`
typedef struct {
	//�e�u���b�N�̈ʒu�́A(0,0)�Ǝc��3���̑��Έʒu�Œ�`
	char x1,y1;
	char x2,y2;
	char x3,y3;
	char color;//�F
	char rot; //��]�\�񐔁A������z����Ə����ʒu�ɖ߂�
} _Block;

//_Music�\���̒�`
typedef struct {
	unsigned char *p; //�Ȕz��̉��t���̈ʒu
	unsigned char *startp; //�Ȕz��̃��s�[�g�ʒu
	unsigned char count; //�������̉��J�E���^
	unsigned short pr; //�������̉��i�^�C�}�����j
	unsigned char stop; //0:���t���A1:�I��
} _Music;
