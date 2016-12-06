#define _SUPPRESS_PLIB_WARNING
// TETRIS�i�L�����N�^�[�x�[�X�j Rev.1.0 2013/8/22
// �@with �e�L�X�gVRAM�R���|�W�b�g�o�̓V�X�e�� for PIC32MX1xx/2xx by K.Tanaka
// rev.2 PS/2�L�[�{�[�h�V�X�e���ɑΉ�

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

//sounddata�z��@�Ⴂ�h?3�I�N�^�[�u���̎����J�E���^�l�APR3�ɏ������ނƉ����ݒ肳���
unsigned char sounddata[SIZEOFSOUNDBF] = {0};

const music_dat_t part1[] = {
#include "humen.h"
};
const music_dat_t part2[] = {
    {240,1920,68,100},
{2160,1440,69,100},
{3600,240,69,100},
{3840,240,68,100},
{4080,1920,64,100},
{6000,1920,69,100},
{7920,1440,74,100},
{9360,240,72,100},
{9600,240,71,100},
{9840,1440,69,100},
{11280,240,71,100},
{11520,240,69,100},
{11760,1200,68,100},
{12960,120,69,100},
{13080,120,68,100},
{13200,240,64,100},
{13440,240,68,100},
{13680,1920,69,100},
{17520,1440,69,100},
{18960,240,71,100},
{19200,240,69,100},
{19440,1200,68,100},
{20640,120,69,100},
{20760,120,68,100},
{20880,240,64,100},
{21120,240,68,100},
{21360,1920,69,100},
{23280,1920,68,100},
{25200,1440,69,100},
{26640,240,69,100},
{26880,240,68,100},
{27120,1920,64,100},
{29040,1920,69,100},
{30960,1440,74,100},
{32400,240,72,100},
{32640,240,71,100},
{32880,1440,69,100},
{34320,240,71,100},
{34560,240,69,100},
{34800,1200,68,100},
{36000,120,69,100},
{36120,120,68,100},
{36240,240,64,100},
{36480,240,68,100},
{36720,1920,69,100},
{40560,1440,69,100},
{42000,240,71,100},
{42240,240,69,100},
{42480,1200,68,100},
{43680,120,69,100},
{43800,120,68,100},
{43920,240,64,100},
{44160,240,68,100},
{44400,1920,69,100},
{48240,1440,69,100},
{49680,240,71,100},
{49920,240,69,100},
{50160,1200,68,100},
{51360,120,69,100},
{51480,120,68,100},
{51600,240,64,100},
{51840,240,68,100},
{52080,1920,69,100},

};

const music_dat_t part3[] = {
    {23280,240,52,100},
{23520,240,59,100},
{23760,240,64,100},
{24000,240,68,100},
{24240,240,64,100},
{24480,240,59,100},
{24720,240,52,100},
{24960,240,59,100},
{25200,240,57,100},
{25440,240,60,100},
{25680,240,64,100},
{25920,240,60,100},
{26160,240,69,100},
{26400,240,60,100},
{26640,240,64,100},
{26880,240,60,100},
{27120,240,56,100},
{27360,240,59,100},
{27600,240,64,100},
{27840,240,68,100},
{28080,240,64,100},
{28320,240,59,100},
{28560,240,56,100},
{28800,240,59,100},
{29040,240,52,100},
{29280,240,57,100},
{29520,240,60,100},
{29760,240,64,100},
{30000,480,60,100},
{30480,480,57,100},
{30960,240,65,100},
{31200,240,57,100},
{31440,240,60,100},
{31680,240,65,100},
{31920,120,67,100},
{32040,120,69,100},
{32160,240,67,100},
{32400,240,65,100},
{32640,240,62,100},
{32880,240,64,100},
{33120,240,52,100},
{33360,240,57,100},
{33600,240,64,100},
{33840,240,60,100},
{34080,240,57,100},
{34320,240,52,100},
{34560,240,57,100},
{34800,240,59,100},
{35040,240,52,100},
{35280,240,56,100},
{35520,240,59,100},
{35760,240,64,100},
{36000,240,59,100},
{36240,240,56,100},
{36480,240,59,100},
{36720,240,52,100},
{36960,240,57,100},
{37200,240,60,100},
{37440,240,64,100},
{37680,480,60,100},
{38160,480,57,100},
{38640,240,52,100},
{38880,240,57,100},
{39120,240,60,100},
{39360,240,57,100},
{39600,240,64,100},
{39840,240,57,100},
{40080,240,60,100},
{40320,240,57,100},
{40560,240,52,100},
{40800,240,57,100},
{41040,240,60,100},
{41280,240,52,100},
{41520,240,64,100},
{41760,240,60,100},
{42000,240,57,100},
{42240,240,52,100},
{42480,240,52,100},
{42720,240,59,100},
{42960,240,62,100},
{43200,240,59,100},
{43440,240,68,100},
{43680,240,59,100},
{43920,240,62,100},
{44160,240,59,100},
{44400,1920,60,100},
{46320,240,52,100},
{46560,240,57,100},
{46800,240,60,100},
{47040,240,57,100},
{47280,240,64,100},
{47520,240,57,100},
{47760,240,60,100},
{48000,240,57,100},
{48240,240,52,100},
{48480,240,57,100},
{48720,240,60,100},
{48960,240,52,100},
{49200,240,64,100},
{49440,240,60,100},
{49680,240,57,100},
{49920,240,52,100},
{50160,240,52,100},
{50400,240,59,100},
{50640,240,62,100},
{50880,240,59,100},
{51120,240,68,100},
{51360,240,59,100},
{51600,240,62,100},
{51840,240,59,100},
{52080,1920,60,100},

};


//�O�t���N���X�^�� with PLL (16�{)
#pragma config PMDL1WAY = OFF, IOL1WAY = OFF
#pragma config FPLLIDIV = DIV_1, FPLLMUL = MUL_16, FPLLODIV = DIV_1
#pragma config FNOSC = PRIPLL, FSOSCEN = OFF, POSCMOD = XT, OSCIOFNC = OFF
#pragma config FPBDIV = DIV_1, FWDTEN = OFF, JTAGEN = OFF, ICESEL = ICS_PGx1

unsigned char *cursor; //�J�[�\���ʒu
unsigned char cursorc; //�J�[�\���F
unsigned char board[25][12]; //�u���b�N��z�u����z��
unsigned int score, highscore; //���_�A�n�C�X�R�A
unsigned int gcount = 0; //�J�E���^�A�����̎�Ɏg�p
unsigned short keyold; //�O��L�[���͏�ԁi���s�[�g���͖h�~�p�j
char downkeyrepeat; //���L�[�̃��s�[�g����
unsigned char next; //���̃u���b�N�̎��
unsigned char fallspeed, fallcount; //�u���b�N�̗������x�A�J�E���^
unsigned char level; //���݂̃��x��
unsigned char gamestatus;
// gamestatus
// 0:�Q�[���X�^�[�g
// 1:�V�u���b�N�o��
// 2:�u���b�N������
// 3:�X�e�[�W�N���A
// 4:�Q�[���I�[�o�[

unsigned char lines; //�����������C���ݐϐ�
const unsigned int scorearray[] = {40, 100, 300, 1200}; //���������������C�����ɂ�链�_

//�u���b�N�̌`��A�F�A�����̏����l��`
const _Block block[7] = {
    {1, 0, -1, 0, -2, 0, COLOR_IBLOCK, 1}, //I
    {1, 0, -1, 0, -1, -1, COLOR_JBLOCK, 3}, //J
    {1, 0, 1, -1, -1, 0, COLOR_LBLOCK, 3}, //L
    {1, 0, 0, -1, -1, -1, COLOR_ZBLOCK, 1}, //Z
    {0, -1, 1, -1, -1, 0, COLOR_SBLOCK, 1}, //S
    {0, -1, -1, 0, -1, -1, COLOR_OBLOCK, 0}, //O
    {1, 0, 0, -1, -1, 0, COLOR_TBLOCK, 3} //T
};

_Block falling; //���ݗ������̃u���b�N�̍\����
unsigned char blockx, blocky, blockangle, blockno; //���ݗ������̃u���b�N�̍��W�A�����A���

_Music music; //���t���̉��y�\����
const unsigned short *sounddatap; //�u���b�N���n���ʉ��z��̈ʒu�A���t���̉��y��肱�����D��


//musicdata�z��@���K,����,���K,����,�E�E�E�E�E�@�Ō�ɉ��K254�ŋȏI���A253�ōŏ��փ��s�[�g
//				���K�@0:�h?12:��̃h?24:���̏�̃h�@�@255:�x��
//				�����@60����n�b

//�R���u�`�J
const unsigned char musicdata1[] = {
    9, 20, 4, 10, 5, 10, 7, 20, 5, 10, 4, 10, 2, 30, 5, 10, 9, 20, 7, 10, 5, 10,
    4, 30, 5, 10, 7, 20, 9, 20, 5, 20, 2, 18, 255, 2, 2, 30, 255, 10,
    10, 30, 12, 10, 14, 20, 12, 10, 10, 10, 9, 30, 5, 10, 9, 20, 7, 10, 5, 10,
    4, 30, 5, 10, 7, 20, 9, 20, 5, 20, 2, 18, 255, 2, 2, 30, 255, 10,
    9, 30, 8, 10, 9, 30, 8, 10, 9, 20, 14, 20, 9, 20, 7, 10, 5, 10,
    4, 30, 5, 10, 7, 20, 9, 20, 5, 20, 2, 18, 255, 2, 2, 30, 255, 10,
    253
};

//�J�`���[�V��
const unsigned char musicdata2[] = {
    9, 30, 11, 10, 12, 30, 9, 10, 12, 9, 255, 1, 12, 10, 11, 10, 9, 10, 11, 20, 4, 20,
    11, 30, 12, 10, 14, 30, 11, 10, 14, 9, 255, 1, 14, 10, 12, 10, 11, 10, 9, 30, 255, 10,
    16, 20, 21, 20, 19, 20, 21, 10, 19, 10, 17, 20, 16, 10, 14, 10, 16, 20, 9, 20,
    255, 10, 17, 20, 14, 10, 16, 30, 12, 10, 11, 10, 4, 10, 12, 10, 11, 10, 9, 30, 255, 10,
    253
};

//�g���C�J
const unsigned char musicdata3[] = {
    9, 10,
    14, 29, 255, 1, 14, 9, 255, 1, 14, 9, 255, 1, 14, 10, 13, 10, 14, 10, 16, 30, 13, 10, 9, 20, 255, 10, 9, 10,
    17, 20, 14, 20, 5, 20, 7, 9, 255, 1, 7, 10, 9, 60, 255, 10, 9, 10,
    14, 30, 16, 10, 17, 10, 16, 10, 14, 10, 9, 10, 7, 30, 10, 10, 14, 20, 16, 10, 14, 10,
    9, 30, 10, 10, 9, 10, 7, 10, 4, 10, 5, 10, 2, 60, 255, 10,
    253
};

//��T��
const unsigned char musicdata4[] = {
    9, 10, 12, 10,
    16, 9, 255, 1, 16, 9, 255, 1, 16, 10, 14, 10, 16, 9, 255, 1, 16, 9, 255, 1, 16, 10, 14, 10, 16, 10, 14, 10, 12, 30, 255, 10, 16, 10, 14, 10,
    16, 20, 14, 10, 12, 10, 14, 20, 12, 10, 11, 10, 12, 10, 11, 10, 9, 30, 255, 10, 9, 10, 12, 10,
    16, 9, 255, 1, 16, 9, 255, 1, 16, 10, 14, 10, 16, 9, 255, 1, 16, 9, 255, 1, 16, 10, 14, 10, 16, 10, 14, 10, 12, 30, 255, 10, 16, 10, 14, 10,
    16, 9, 255, 1, 16, 10, 14, 10, 12, 10, 14, 20, 16, 20, 21, 50, 255, 10,
    253
};

//�J�����J
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

//�P�@�Q�N�@�R
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

// �e���x���̋Ȏw��
const unsigned char *musicdatap[] = {
    musicdata1, musicdata2, musicdata3, musicdata4, musicdata5, musicdata6
};

//�u���b�N���n���̌��ʉ��i���������u���b�N1�s?4�s�p�j
const unsigned short soundDong[5][SOUNDDONGLENGTH] = {
    {40000, 40000, 40000, 40000, 40000, 1, 0}, //�u���b�N���n��
    {40000, 40000, 30000, 30000, 30000, 1, 0}, //1�s�N���A�̉�
    {30000, 30000, 30000, 20000, 20000, 20000, 0}, //2�s�N���A�̉�
    {30000, 30000, 30000, 10000, 10000, 10000, 0}, //3�s�N���A�̉�
    {16000, 14000, 12500, 11000, 9500, 8000, 0} //4�s�N���A�̉�
};

//�w�i�摜�R�[�h ��24�~�c14�L�����N�^�[
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
    // 60����n�b�E�F�C�g�i�r�f�I��ʂ̍ŉ��s�M���o�͏I���܂ő҂j
    drawcount = 0;
    while (drawcount < n) asm("wait");
}

unsigned char startkeycheck(unsigned short n) {
    // 60����n�b�E�F�C�g
    // �X�^�[�g�{�^�����������΂����߂�
    //�@�߂�l�@�X�^�[�g�{�^����������1�A������Ȃ����0
    drawcount = 0;
    while (drawcount < n) {
        musicTask();
        if (~KEYPORT & KEYSTART) return 1;
        asm("wait");
        gcount++;
    }
    return 0;
}

void playmusic1step(void) {
    return;
    //���t���̋Ȃ�1�i�߂�
    if (music.stop) return; //���t�I���ς�
    music.count--;
    if (music.count > 0) {
        PR3 = music.pr;
        return;
    }
    //���̉���炷
    if (*music.p == 254) { //�ȏI��
        music.stop = 1;
        music.pr = 0;
        PR3 = 0;
        return;
    }
    if (*music.p == 253) { //�Ȃ̍ŏ��ɖ߂�
        music.p = music.startp;
    }
    if (*music.p == 255) {
        music.pr = 0;
        PR3 = 0; //�x��
    } else {
        music.pr = sounddata[*music.p]; //�����f�[�^
        PR3 = music.pr;
    }
    music.p++;
    music.count = *music.p; //��������
    music.p++;
}

void playmusic60thsec(void) {
    //60����1�b�E�F�C�g��A���݉��t���̋Ȃ�1�i�߂�
    wait60thsec(1); //60����1�b�E�F�C�g
    //	playmusic1step();
    musicTask();
}

void wait60thsecmusic(unsigned short n) {
    //60����n�b�ԁA�Ȃ����t���Ȃ���E�F�C�g
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
//    PR3 = 0;
}

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

void writehighscore(void) {
}

void printnext(void) {
    //NEXT�G���A�Ɏ��̃u���b�N��\��
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
    //board�z��̓��e����ʂɕ\��
    char x, y;
    for (y = 1; y <= 23; y++) {
        for (x = 1; x <= 10; x++) {
            printchar(11 + x, y, board[y][x], CODE_BLOCK);
        }
    }
}

void displayscore(void) {
    //���_�\��
    printnumber6(0, 16, 7, score);
    printnumber6(0, 19, 7, 114514);
}

char check(_Block *bp, char x, char y) {
    //x,y�̈ʒu��_Block�\����bl�i�|�C���^�n���j�������邩�`�F�b�N
    //�߂�l�@0:������@-1:�����Ȃ�
    if (board[y][x]) return -1;
    if (board[y + bp->y1][x + bp->x1]) return -1;
    if (board[y + bp->y2][x + bp->x2]) return -1;
    if (board[y + bp->y3][x + bp->x3]) return -1;
    return 0;
}

void putblock(void) {
    //board�z��ɗ������̃u���b�N����������
    _Block *bp;
    bp = &falling;
    board[blocky][blockx] = bp->color;
    board[blocky + bp->y1][blockx + bp->x1] = bp->color;
    board[blocky + bp->y2][blockx + bp->x2] = bp->color;
    board[blocky + bp->y3][blockx + bp->x3] = bp->color;
}

void eraseblock(void) {
    //board�z�񂩂痎�����̃u���b�N������
    _Block *bp;
    bp = &falling;
    board[blocky][blockx] = COLOR_SPACE;
    board[blocky + bp->y1][blockx + bp->x1] = COLOR_SPACE;
    board[blocky + bp->y2][blockx + bp->x2] = COLOR_SPACE;
    board[blocky + bp->y3][blockx + bp->x3] = COLOR_SPACE;
}

char newblock(void) {
    //���̃u���b�N�o��
    //�߂�l�F�ʏ�0�A�u���Ȃ����-1�i�Q�[���I�[�o�[�j
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
    printnext(); //NEXT�̏ꏊ�Ɏ��̃u���b�N�\��
    putblock(); //�����J�n�̃u���b�N�z�u
    downkeyrepeat = 0; //���L�[�̃��s�[�g��j�~
    return 0;
}

void displaylevel(void) {
    //���x���\��
    printstr(13, 13, 7, "LEVEL");
    printnumber6(14, 13, 7, level);
    wait60thsecmusic(60 * 3);
    printstr(13, 13, 0, "        ");
    printstr(1, 22, 0, "     ");
    printnumber6(0, 22, 7, lines);
    printnumber6(0, 25, 7, level);
}

void moveblock(void) {
    //�u���b�N�̗����A�L�[���̓`�F�b�N�ŉ�]�A�ړ�
    //�����ł��Ȃ��ꍇ��gamestatus=1�Ƃ���

    unsigned short k;
    _Block tempblock;
    const _Block *blockp;
    char movedflag;

    movedflag = 0;

    // �{�^���`�F�b�N
    k = ~KEYPORT & (KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT);
    if (keyold != KEYUP && k == KEYUP) { //��{�^���i��]�j
        if (blockangle < falling.rot) { //�����S��90�x��]
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
        } else { //��]�������ʒu�ɖ߂��ꍇ
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
    } else if (keyold != KEYRIGHT && k == KEYRIGHT) { //�E�{�^��
        if (check(&falling, blockx + 1, blocky) == 0) blockx++;
        movedflag = -1;
    } else if (keyold != KEYLEFT && k == KEYLEFT) { //���{�^��
        if (check(&falling, blockx - 1, blocky) == 0) blockx--;
        movedflag = -1;
    } else if (downkeyrepeat && k == KEYDOWN) { //���{�^��
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
        downkeyrepeat = -1; // �V�u���b�N�o�����̉��L�[���s�[�g����
    }
    keyold = k;

    fallcount--;
    if (fallcount == 0) { //���R����
        fallcount = fallspeed;
        if (check(&falling, blockx, blocky + 1)) gamestatus = 1; //���n�����i�Œ�j
        else {
            blocky++;
            movedflag = -1;
        }
    }
    if (movedflag) {
        if (check(&falling, blockx, blocky + 1)) {
            sounddatap = soundDong[0]; //���n��
        }
    }
}

void linecheck(void) {
    //�������C���̃`�F�b�N�Ə����A���_���Z
    char x, y, x2, y2, c, cleared, cleared2;

    //�������郉�C��������Δ����u���b�N�ɕύX
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

    wait60thsecmusic(15); //60����15�b�҂�

    //�����u���b�N�̍s���������āA��ԏ�Ɋl���������_�\��
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

    wait60thsecmusic(15); //60����15�b�҂�

    //���������s�̕��A�S�̂𗎉�������
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

    //���������s���ɍ��킹�ē��_���Z
    score += scorearray[cleared - 1];
    if (score > highscore) {
        highscore = score;
    }
    lines += cleared;
    sounddatap = soundDong[cleared]; //���������s���ɍ��킹�����ʉ���炷
    printnumber6(0, 22, 7, lines);
}

void gameinit(void) {
    //���Z�b�g��1�񂾂��Ă΂�鏉����

    //�J���[�p���b�g��`
    set_palette(COLOR_WALL, 200, 200, 200); //�ǂ̐F
    set_palette(COLOR_LBLOCK, 0, 255, 165); //L�`�u���b�N�F
    set_palette(COLOR_BITMAP, 0xde, 0xb0, 0xc4); //�w�i�摜�̐F
    set_palette(COLOR_BRICK, 0x22, 0xb2, 0x22); //�w�i�摜�̐F

    if (*(unsigned int *) FLASHPAGE == -1) { //�t���b�V���������Ƀn�C�X�R�A���������܂�Ă��邩�`�F�b�N
        highscore = 1000;
        writehighscore();
    } else highscore = *(unsigned int *) FLASHPAGE;
    score = 0;
}

void gameinit2(void) {
    //�Q�[���X�^�[�g�{�^��������ɌĂ΂�鏉����
    int i;
    unsigned char y;
    const unsigned char *p;
    score = 0;
    level = 0;
    fallspeed = 60;
    clearscreen();
    locate(0, 15, COLOR_BRICK);
    for (i = 0; i < 12 * 30; i++) printchar2(CODE_BRICK);

    // �w�i�\��
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

    // ���_�\���g��`��
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

    //NEXT�\���g��`��
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
    printnext(); //NEXT�̏ꏊ�Ɏ��̃u���b�N�\��
    sounddatap = soundDong[0] + SOUNDDONGLENGTH - 1;
}

void gameinit3(void) {
    //�e���x�����ƂɌĂ΂�鏉����
    char x, y;
    lines = 0; //�������C�����N���A
    level++;

    //�������x�X�V
    if (fallspeed <= 8) fallspeed = 3;
    else fallspeed -= 5;

    //�Q�[���G���A�̏�����
    for (x = 0; x < 12; x++) {
        for (y = 0; y < 25; y++) {
            if (x == 0 || x == 11 || y == 24) {
                board[y][x] = COLOR_WALL;
            } else {
                board[y][x] = COLOR_SPACE;
            }
        }
    }
    startmusic((unsigned char *) musicdatap[(level - 1) % (sizeof musicdatap / sizeof musicdatap[0])]); //�e���x���̉��y�J�n
    keyold = ~KEYPORT & (KEYUP | KEYDOWN | KEYLEFT | KEYRIGHT);
    srand(gcount);
}

void gameover(void) {
    //�Q�[���I�[�o�[
    printstr(13, 13, 7, "GAME OVER");
    wait60thsecmusic(60 * 5); //5�b�E�F�C�g
    stopmusic();
    if (highscore>*(unsigned int *) FLASHPAGE) writehighscore(); //�t���b�V���Ƀn�C�X�R�A��������
}

void title(void) {
    //�^�C�g����ʕ\��
    unsigned char x, y, c;
    const unsigned char *p;
    clearscreen();

    //�w�i�摜�\��
    p = bitmap1;
    for (y = 0; y < 14; y++) {
        locate(3, y + 3, COLOR_BITMAP);
        for (x = 0; x < 24; x++) {
            printchar2(*p++);
        }
    }

    //�^�C�g�����S�\��
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
    // 0:�Q�[���X�^�[�g
    // 1:�V�u���b�N�o��
    // 2:�u���b�N������
    // 3:�X�e�[�W�N���A
    // 4:�Q�[���I�[�o�[
    gameinit2();
    gamestatus = 0;
    while (gamestatus < 4) {
        gameinit3();
        displaylevel(); //���x���\��
        gamestatus = 1;
        while (gamestatus < 3) {
            if (newblock()) gamestatus = 4; //�V�u���b�N�o���A�Q�[���I�[�o�[�`�F�b�N
            else gamestatus = 2;
            drawcount = 0;
            while (gamestatus == 2) {
                musicTask();
                while (drawcount == 0) asm("wait"); //�A�C�h���ɂ��Ċ��荞�ݑ҂�
                drawcount = 0;
                eraseblock(); //�u���b�N����
                moveblock(); //�u���b�N�ړ��A���n�����`�F�b�N
                putblock(); //�u���b�N�z�u
                if (gamestatus == 1) { //�u���b�N���n�����̏ꍇ
                    linecheck(); //���C�������`�F�b�N�A�������C������
                    if (lines >= SCENECLEARLINE) gamestatus = 3;
                }
                displayscore();
                show(); //board�z��̓��e����ʏo��
                gcount++;
            }
        }
    }
    gameover();
}

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

    unsigned int timing[2];
    unsigned int time;
    int pos[2] = {0};
    const music_dat_t *part;
    //  mktone(1,1,1,1);
    //  mktone(1,1,1,0);

    gameinit(); //�Q�[���S�̏�����
    mktone(1, 1, 1, 1);
    mktone(1, 1, 1, 0);
    mktone(1, 1, 1, 2);
    while (1) {
        title(); //�^�C�g����ʁA�X�^�[�g�{�^���Ŗ߂�
        game(); //�Q�[�����C�����[�v
    }
}

void musicTask(void) {
    unsigned int time;
    unsigned int i;
    static unsigned int timing[3];
    static unsigned int pos[3];
    music_dat_t *part;
    time = gettime();
    static int isinitialized;
    if(!isinitialized){isinitialized=1;
    for(i=0;i<3;i++){

        part = i ? i==2?part3:part2 : part1;
        timing[i] = part[pos[i]].time*25;
    }
    }
    
    for (i = 0; i < 3; i++) {
        part = i ? i==2?part3:part2 : part1;
        if (time>-SAMPLING_FREQ / 20 + timing[i]) {
            addNextSound(timing[i]&0xFFFFFFFE, part[pos[i]].key, part[pos[i]].len * 1000 * 25 / SAMPLING_FREQ/* *3/4 */ - 10, part[pos[i]].vel + 120, i);
            pos[i]++;
            timing[i] = part[pos[i]].time * 25;
        }
        if (part[pos[i]].time == -1) {
            for(i=0;i<3;i++){
                pos[i]=0;
                part = i ? i==2?part3:part2 : part1;
                timing[i] = part[pos[i]].time*25;
            }
            settime(0);
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