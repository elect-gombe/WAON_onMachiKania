#include "audio.h"
#include <stdio.h>


/*
 *          *                                
 *         * ** 
 *        *    **                                 
 *       *       ******                                  
 *      *              ******                           
 *     *                     *                      
 *    *                       *
 *      (a)  (d)      (s)    (r)                       
 */

#define WAVE_TABLE_SIZE_N2 6

const int8_t _tri_wavetable[1<<WAVE_TABLE_SIZE_N2]={
120,112,104,96,88,80,72,64,56,48,40,32,24,16,8,0,-8,-16,-24,-32,-40,-48,-56,-64,-72,-80,-88,-96,-104,-112,-120,-128,-120,-112,-104,-96,-88,-80,-72,-64,-56,-48,-40,-32,-24,-16,-8,0,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,127
};

const int8_t _sq_wavetable[1<<WAVE_TABLE_SIZE_N2]={
-32.75,-74.421875,-87.81640625,-97.8623046875,-105.396728515625,-111.047546386719,-115.285659790039,-118.464244842529,-120.848183631897,-122.636137723923,-123.977103292942,-124.982827469707,-125.73712060228,-126.30284045171,-126.727130338782,-127.045347754087,-127.284010815565,-127.463008111674,-127.597256083755,-127.697942062817,-127.773456547112,-127.830092410334,-127.872569307751,-127.904426980813,-127.92832023561,-127.946240176707,-127.959680132531,-127.969760099398,-127.977320074548,-127.982990055911,-127.987242541933,-64.2404319064501,-16.4303239298376,19.4272570526218,46.3204427894664,66.4903320920998,81.6177490690748,92.9633118018061,101.472483851355,107.854362888516,112.640772166387,116.23057912479,118.922934343593,120.942200757694,122.456650568271,123.592487926203,124.444365944652,125.083274458489,125.562455843867,125.9218418829,126.191381412175,126.393536059131,126.545152044349,126.658864033261,126.744148024946,126.80811101871,126.856083264032,126.892062448024,126.919046836018,126.939285127014,126.95446384526,126.965847883945,127,127
};

#define FREQMUL_N2 8
const unsigned int key_to_freq_table[]={
    33,35,38,40,42,45,47,50,53,56,60,63,67,71,75,80,84,89,95,100,106,113,119,126,134,142,150,159,169,179,189,201,213,225,239,253,268,284,301,319,338,358,379,401,425,451,477,506,536,568,601,637,675,715,758,803,851,901,955,1011,1072,1135,1203,1274,1350,1430,1515,1606,1701,1802,1909,2023,2143,2271,2406,2549,2700,2861,3031,3211,3402,3604,3819,4046,4286,4541,4811,5098,5401,5722,6062,6422,6804,7209,7638,8092,8573,9083,9623,10195,10801,11444,12124,12845,13609,14418,15275,16184,17146,18165,19246,20390,21602,22887,24248,25690,27217,28836,30551,32367,34292,36331,38491,40780,43205,45774,48496,51380
};

#define ENVE_EXP_TABLE_N2 7
#define ENVE_EXP_TABLE_MAX_N2 8
const unsigned char enve_exp_table[]={
    250,239,229,219,210,201,193,184,177,169,162,155,148,142,136,130,125,119,114,109,105,100,96,92,88,84,81,77,74,71,68,65,62,60,57,55,52,50,48,46,44,42,40,39,37,35,34,32,31,30,28,27,26,25,24,23,22,21,20,19,18,18,17,16,15,15,14,14,13,12,12,11,11,10,10,10,9,9,8,8,8,7,7,7,6,6,6,6,5,5,5,5,5,4,4,4,4,4,4,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1
};


const enve_t enve1={
    .a_vel = 6000,
    .d_vel = 20,
    .s_vel = 15,
    .r_vel = 6000,
    .a_time = 10,
    .ad_time = 250,
};

const enve_t enve2={
    .a_vel = 6000,
    .d_vel = 0,
    .s_vel = 0,
    .r_vel = 6000,
    .a_time = 10,
    .ad_time = 250,
};

volatile sound_t sound
[SOUND_CH];
int soundtiming[SOUND_CH];

unsigned int soundtime;

sound_t next_sound[SOUND_CH]={0};

void addNextSound(unsigned int time,unsigned int keyofTone,unsigned int len,unsigned int vel,size_t idx){
    next_sound[idx].active = 1;
    next_sound[idx].len = len;
//    next_sound[idx].theta = 0;
    next_sound[idx].tone_freq = key_to_freq_table[keyofTone]>>(8-WAVE_TABLE_SIZE_N2);
    next_sound[idx].time = 0;
    next_sound[idx].enve_val = 0;
    next_sound[idx].prev_enve_exp_val = 0;
    next_sound[idx].vel = vel;
    if(idx!=2){
        next_sound[idx].enve = &enve1;
        next_sound[idx].wt = &_sq_wavetable;
    }else{
        next_sound[idx].wt = &_tri_wavetable;
        next_sound[idx].enve = &enve2;
        next_sound[idx].vel+=20;
    }
    
    soundtiming[idx]=time;
}
unsigned int gettime(void){return soundtime;}
unsigned int settime(int t){soundtime = t;}

void calcsound(unsigned int idx,unsigned char *buff);

void mktone(unsigned int keyofTone,unsigned int len,unsigned int vel,size_t idx){
    sound[idx].active = 1;
    sound[idx].len = len;
//    sound[idx].theta = 0;
    sound[idx].tone_freq = key_to_freq_table[keyofTone]>>(8-WAVE_TABLE_SIZE_N2);
    sound[idx].time = 0;
    sound[idx].enve_val = 0;
    sound[idx].prev_enve_exp_val = 0;
    sound[idx].vel = vel;
    if(idx==0){
        sound[idx].wt = &_sq_wavetable;
        sound[idx].enve = &enve1;
    }else{
        sound[idx].wt = &_tri_wavetable;
        sound[idx].enve = &enve2;
    }
}

void soundTask(unsigned char *buff){
    size_t i;
    for(i=0;i<SOUND_CH;i++){
        if(sound[i].active == 1)
        calcsound(i,buff);
    }
    soundtime+=SIZEOFSOUNDBF>>1;
}

void calcsound(unsigned int id,unsigned char *buff){
    sound_t *s = &sound[id];
    unsigned int idx;
    unsigned int i;
    unsigned int enve;
    unsigned int time_div_n;
    
    for(i=0;i<SIZEOFSOUNDBF >> 1;){
        time_div_n = (s->time&(0xFF>>(8-ENVE_INTR_N2-1)));
        if(time_div_n==0){
            calcEnve(s);
        }
        if((soundtime+i) == (soundtiming[id])&&next_sound[id].active){
            sound[id] = next_sound[id];
        }
        s->time++;
        s->theta += s->tone_freq;
        s->theta &= (0xFFFF >> (16-OVERSAMPLING_N2-WAVE_TABLE_SIZE_N2));

        idx = (s->theta >> (OVERSAMPLING_N2));
        enve = (s->enve_exp_val *(time_div_n) + (ENV_INTR_TIME - time_div_n)*s->prev_enve_exp_val);
//        enve=16*256;
        buff[i] += (enve * s->wt[idx]) >> ENVE_EXP_TABLE_MAX_N2+ENVE_INTR_N2;
        i++;

        s->time++;
        s->theta += s->tone_freq;
        s->theta &= (0xFFFF >> (16-OVERSAMPLING_N2-WAVE_TABLE_SIZE_N2));

        idx = (s->theta >> (OVERSAMPLING_N2));
        buff[i] += (enve * s->wt[idx]) >> (ENVE_EXP_TABLE_MAX_N2+ENVE_INTR_N2);
        i++;
    }
}

int calcEnve(sound_t *s){
    const enve_t *e = s->enve;
    unsigned int time = s->time>>ENVE_INTR_N2;

    s->prev_enve_exp_val = s->enve_exp_val;

    if(time > s->len){
        s->enve_val -= e->r_vel;
    }else if(time < e->a_time){
        s->enve_val += e->a_vel;
    }else if(time < e->ad_time){
        s->enve_val -= e->d_vel;
    }else{
        s->enve_val -= e->s_vel;
    }
    
    if(s->enve_val > (0x1<<31)){
        s->enve_val = 0;
//        s->active = 0;
    }
    
    s->enve_exp_val = enve_exp_table[((1<<ENVE_EXP_TABLE_N2)-1) - ((s->enve_val * s->vel) >> (SOUND_VEL_MAX_N2 + ENVE_MAX_N2-ENVE_EXP_TABLE_N2))];

    return s->enve_exp_val;
}
//        enve = enve_exp_table[((1<<ENVE_EXP_TABLE_N2)-1)-((enve * s->vel) >> (ENV_INTR_N2 + SOUND_VEL_MAX_N2+ENVE_MAX_N2-ENVE_EXP_TABLE_N2))];

