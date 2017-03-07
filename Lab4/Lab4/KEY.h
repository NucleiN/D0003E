/*
 * KEY.h
 *
 * Created: 17/02/23 15:46:43
 *  Author: Daniel
 */ 


#ifndef KEY_H_
#define KEY_H_

#include <avr/io.h>
#include "TinyTimber.h"
#include "PWM.h"
#include "GUI.h"

#define init_KEY(_GUI1, _GUI2) {initObject(), NULL, 0, 0, 0, 1, _GUI1, _GUI2}
typedef struct{
	Object super;
	GUI *guiSel;
	int keyCount;
	int keyPressed;
	int dir;
	int enterEn;
	GUI *GUI1;
	GUI *GUI2;
} KEY;

int KEY_pwmSel(KEY *k, int arg);
int KEY_pwmInc(KEY *k, int arg);
int _KEY_loop(KEY *k, int arg);

#define KEY_scrollUp(_KEY) \
	PWM_deltaFreq(&((_KEY)->guiSel->p), 1); \
	(_KEY)->dir = 1; \
	if((_KEY)->keyCount == 0){ \
		(_KEY)->keyCount = 1; \
		AFTER(MSEC(1000),_KEY,_KEY_loop,1); \
	}

#define KEY_scrollDown(_KEY) \
	PWM_deltaFreq(&((_KEY)->guiSel->p), -1); \
	(_KEY)->dir = -1; \
	if((_KEY)->keyCount == 0){ \
		(_KEY)->keyCount = 1; \
		AFTER(MSEC(1000),_KEY,_KEY_loop,1); \
	}

#define KEY_scrollStop(_KEY) \
	(_KEY)->dir = 0

#endif /* KEY_H_ */