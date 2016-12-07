/* 
 * File:   audio.h
 * Author: gombe
 *
 * Created on 2016/12/02, 20:25
 */

#ifndef AUDIO_H
#define	AUDIO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
    
typedef struct{
    int a_vel;
    int d_vel;
    int s_vel;
    int r_vel;
    int a_time;
    int ad_time;
}enve_t;

typedef struct{
    const int8_t *table;
    const uint8_t sizeofbuf_n2;
}wavetable_t;

typedef struct{
  const enve_t *enve;
  const int8_t *wt;
  unsigned int enve_val;
  unsigned int prev_enve_exp_val;
  unsigned int enve_exp_val;
  unsigned int time;
  unsigned int len;
  unsigned int tone_freq;
  unsigned int vel;
  unsigned int theta;
  unsigned int active;
}sound_t;

typedef struct{
  unsigned int time;
  uint16_t len;
  uint8_t key;
  uint8_t vel;
}music_dat_t;


#define OVERSAMPLING 512
#define OVERSAMPLING_N2 9

#define ENVE_MAX 60000
#define ENVE_MAX_N2 16

#define ENV_INTR_TIME 32
#define ENVE_INTR_N2 5

#define SOUND_CH 3

#define SOUND_VEL_MAX 128
#define SOUND_VEL_MAX_N2 7

#define SIZEOFSOUNDBF 2000

void mktone(unsigned int keyofTone,unsigned int len,unsigned int vel,size_t idx);

#ifdef	__cplusplus
}
#endif

#endif	/* AUDIO_H */

