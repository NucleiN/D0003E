/*
 * KEY.c
 *
 * Created: 17/02/23 15:46:31
 *  Author: Daniel
 */ 

#include "KEY.h"

int KEY_pwmSel(KEY *k, int arg){
	if(!(PINE & (1<<PINE3))){
		//RIGHT
		if(k->guiSel == k->GUI2){
			GUI_stopBlink(k->guiSel);
			k->guiSel = NULL;
		}else{
			k->guiSel = k->GUI2;
			GUI_stopBlink(k->GUI1);
			GUI_startBlink(k->guiSel);
		}
	}else if(!(PINE & (1<<PINE2))){
		//LEFT
		if(k->guiSel == k->GUI1){
			GUI_stopBlink(k->guiSel);
			k->guiSel = NULL;
		}else{
			k->guiSel = k->GUI1;
			GUI_stopBlink(k->GUI2);
			GUI_startBlink(k->guiSel);
		}
	}
	return 0;
}

int _KEY_allowEnter(KEY *k, int arg){
	k->enterEn = 1;
	return 0;
}

int KEY_pwmInc(KEY *k, int arg){
	if(k->guiSel == NULL)
	return;
	GUI *guin = (k->guiSel);
	PWM *pwmn = &(guin->p);
	if(!(PINB & (1<<PINB7))){
		KEY_scrollDown(k);
		}else if(!(PINB & (1<<PINB6))){
		KEY_scrollUp(k);
		}else if(!(PINB & (1<<PINB4))){
		//ENTER
		if(!k->enterEn)
		return;
		k->enterEn = 0;
		AFTER(MSEC(500), k, _KEY_allowEnter, 0);
		if(PWM_getFreq(pwmn) == 0){
			PWM_setFreq(pwmn, GUI_getAltFreq(guin));
			GUI_setAltFreq(guin, 0);
			//pwmn->freq = guin->altFreq;
			//guin->altFreq = 0;
			}else{
			GUI_setAltFreq(guin, PWM_getFreq(pwmn));
			PWM_setFreq(pwmn, 0);
			//guin->altFreq = pwmn->freq;
			//pwmn->freq = 0;
		}
		
		}else if((PINB & (1<<PINB7)) && (PINB & (1<<PINB6))){
		KEY_scrollStop(k);
	}
	GUI_print(guin);
	return 0;
}

int _KEY_loop(KEY *k, int arg){
	if(k->keyCount == arg){
		arg++;
		k->keyCount = arg;
		GUI *guin = (k->guiSel);
		PWM *pwmn = &(guin->p);
		int tfreq = PWM_getFreq(pwmn);
		if((tfreq == -1) || (k->dir == 0)){
			k->dir = 0;
			k->keyCount = 0;
			return;
		}
		tfreq+= k->dir;
		PWM_setFreq(pwmn,tfreq);
		GUI_print(guin);
		AFTER(MSEC(800+(200*arg)), k, _KEY_loop, arg);
	}
	return 0;
}