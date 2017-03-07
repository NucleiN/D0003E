/*
 * GUI.h
 *
 * Created: 17/02/23 15:42:32
 *  Author: Daniel
 */

#ifndef GUI_H_
#define GUI_H_

#include "TinyTimber.h"
#include "codeUtils.h"
#include "PWM.h"

#define init_GUI(_pwm, _pos) {initObject(), _pwm, _pos, 1, 0, 0}
typedef struct {
    Object super;
    PWM p;
    int pos;
    int writeEn;
    int altFreq;
    int blinkCount;
} GUI;

int _GUI_print(GUI *g, int arg);

int _GUI_clear(GUI *g, int arg);

int _GUI_blinkLoop(GUI *g, int arg);

int _GUI_setAltFreq(GUI *g, int arg);

int _GUI_getAltFreq(GUI *g, int arg);

#define GUI_init(_GUI) \
    GUI_fprint(_GUI); \
    ASYNC(&((_GUI)->p), PWM_init_pulseLoop, 0)

#define GUI_print(_GUI) \
    ASYNC(_GUI, _GUI_print, 0)

#define GUI_fprint(_GUI) \
    ASYNC(_GUI, _GUI_print, 1)

#define GUI_clear(_GUI) \
    ASYNC(_GUI, _GUI_clear, 0)

#define GUI_stopBlink(_GUI) \
    (_GUI)->blinkCount = 0; \
    GUI_fprint(_GUI)

#define GUI_startBlink(_GUI) \
    (_GUI)->blinkCount = 0; \
    ASYNC(_GUI, _GUI_blinkLoop, 0)

#define GUI_getAltFreq(_GUI) \
    SYNC(_GUI, _GUI_getAltFreq, 0)

#define GUI_setAltFreq(_GUI, _set) \
    ASYNC(_GUI, _GUI_setAltFreq, _set)

#endif /* GUI_H_ */