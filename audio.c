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

#define SQ_TABLE_N2 5

const int8_t _sq_table[1<<SQ_TABLE_N2]={
    100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,-100,-100,-100,-100,-100,-100,-100,-100,-100,-100,-100,-100,-100,-100,-100,-100,
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

const wavetable_t sq_wavetable={
    .table = _sq_table,
    .sizeofbuf_n2 = SQ_TABLE_N2,
};


const enve_t enve1={
    .a_vel = 10000,
    .d_vel = 50,
    .s_vel = 30,
    .r_vel = 8000,
    .a_time = 6,
    .ad_time = 25,
};

sound_t sound[SOUND_CH];

void calcsound(sound_t *s,unsigned char *buff);

void mktone(unsigned int keyofTone,unsigned int len,unsigned int vel,size_t idx){
    sound[idx].active = 1;
    sound[idx].len = len;
    sound[idx].theta = 0;
    sound[idx].wt = &sq_wavetable;
    sound[idx].tone_freq = key_to_freq_table[keyofTone]>>(8-sound[idx].wt->sizeofbuf_n2);
    sound[idx].time = 0;
    sound[idx].enve = &enve1;
    sound[idx].enve_val = 0;
    sound[idx].vel = vel;
}

void soundTask(unsigned char *buff){
    size_t i;
    for(i=0;i<SOUND_CH;i++){
        if(sound[i].active == 1)
      calcsound(&sound[i],buff);
    }
}

void calcsound(sound_t *s,unsigned char *buff){
    unsigned int idx;
    unsigned int i;
    unsigned int enve;
    unsigned int time_div_n;
    
    for(i=0;i<SIZEOFSOUNDBF >> 1;){
        time_div_n = (s->time&(0xFF>>(8-ENV_INTR_N2-1)));
        if(time_div_n==0){
            calcEnve(s);
        }
        s->time++;
        s->theta += s->tone_freq;
        s->theta &= (0xFFFF >> (16-OVERSAMPLING_N2-s->wt->sizeofbuf_n2));

        idx = (s->theta >> (OVERSAMPLING_N2));
        enve = (s->enve_val *(time_div_n) + (16- time_div_n)*s->prev_enve_val);
        enve = enve_exp_table[(1<<ENVE_EXP_TABLE_N2)-((enve * s->vel) >> (ENV_INTR_N2 + SOUND_VEL_MAX_N2+ENVE_MAX_N2-ENVE_EXP_TABLE_N2))];
        buff[i] += (enve * s->wt->table[idx]) >> ENVE_EXP_TABLE_MAX_N2;
        i++;

        s->time++;
        s->theta += s->tone_freq;
        s->theta &= (0xFFFF >> (16-OVERSAMPLING_N2-s->wt->sizeofbuf_n2));

        idx = (s->theta >> (OVERSAMPLING_N2));
        buff[i] += (enve * s->wt->table[idx]) >> ENVE_EXP_TABLE_MAX_N2;
        i++;
    }
}

int calcEnve(sound_t *s){
    const enve_t *e = s->enve;
    unsigned int time = s->time>>ENV_INTR_N2;

    s->prev_enve_val = s->enve_val;

    if(time < e->a_time){
        s->enve_val += e->a_vel;
    }else if(time < e->ad_time){
        s->enve_val -= e->d_vel;
    }else if(time > s->len){
        s->enve_val -= e->r_vel;
    }else{
        s->enve_val -= e->s_vel;
    }
    
    if(s->enve_val > (0x1<<31)){
        s->enve_val = 0;
    }
    
    return s->enve_val;
}

