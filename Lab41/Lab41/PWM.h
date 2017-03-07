/*
 * PWM.h
 *
 * Created: 17/02/23 15:02:58
 *  Author: Daniel
 */

#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include "TinyTimber.h"

#define init_PWM(_reg, _pin) {initObject(), 0, _reg, _pin, 1}
typedef struct {
    Object super;
    int freq;
    uint8_t *reg;
    uint8_t toggle;
} PWM;

int PWM_init_pulseLoop(PWM *p, int arg);

int _PWM_deltaFreq(PWM *p, int arg);

int _PWM_getFreq(PWM *p, int arg);

int _PWM_setFreq(PWM *p, int arg);

#define PWM_deltaFreq(_PWM, _delta) \
ASYNC(_PWM, _PWM_deltaFreq, _delta)

#define PWM_getFreq(_PWM) \
SYNC(_PWM, _PWM_getFreq, 0)

#define PWM_setFreq(_PWM, _set) \
ASYNC(_PWM, _PWM_setFreq, _set)

#endif /* PWM_H_ */