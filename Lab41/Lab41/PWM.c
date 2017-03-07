/*
 * PWM.c
 *
 * Created: 17/02/23 15:01:08
 *  Author: Daniel
 */

#include "PWM.h"

int PWM_init_pulseLoop(PWM *p, int arg) {
    if (p->freq) {
        *(p->reg) ^= (1 << p->toggle);
        AFTER(USEC(500000 / (p->freq)), p, PWM_init_pulseLoop, 0);
    } else {
        *(p->reg) &= ~(1 << p->toggle);
        AFTER(SEC(1), p, PWM_init_pulseLoop, 0);
    }
    return 0;
}

int _PWM_deltaFreq(PWM *p, int arg) {
    arg += p->freq;
    if (arg <= 0)
        p->freq = 0;
    else if (arg > 99)
        p->freq = 99;
    else
        p->freq = arg;
    return 0;
}

int _PWM_getFreq(PWM *p, int arg) {
    return p->freq;
}

int _PWM_setFreq(PWM *p, int arg) {
    if (arg > 99)
        p->freq = 99;
    else if (arg <= 0)
        p->freq = 0;
    else
        p->freq = arg;
    return 0;
}