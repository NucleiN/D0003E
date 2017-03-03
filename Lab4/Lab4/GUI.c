/*
 * GUI.c
 *
 * Created: 17/02/23 15:43:43
 *  Author: Daniel
 */ 

#include "GUI.h"

int _GUI_print(GUI *g, int arg){
	if(g->writeEn || arg){
		printAt(PWM_getFreq(&(g->p)), g->pos);
		g->writeEn = 1;
	}
	return 0;
}

int _GUI_clear(GUI *g, int arg){
	clearAt(g->pos);
	g->writeEn = 0;
	return 0;
}

int _GUI_blinkLoop(GUI *g, int arg){
	if(arg == g->blinkCount){
		g->blinkCount++;
		if(arg%2){
			GUI_clear(g);
			AFTER(MSEC(300), g, _GUI_blinkLoop, arg+1);
		}else{
			GUI_fprint(g);
			AFTER(MSEC(700), g, _GUI_blinkLoop, arg+1);
		}
	}
	return 0;
}

int _GUI_getAltFreq(GUI *g, int arg){
	return g->altFreq;
}

int _GUI_setAltFreq(GUI *g, int arg){
	g->altFreq = arg;
	return 0;
}