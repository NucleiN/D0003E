#include <avr/io.h>
#include <avr/interrupt.h>
#include "TinyTimber.h"

void CLK_init(void){
	//EN Write to PSC reg
	CLKPR = 0x80;
	//PSC 0
	CLKPR = 0x00;
}

void LCD_init(void){
	//		Enable LCD | LPow Waveform
	LCDCRA = (1<<LCDEN) | (1<<LCDAB);
	//		Async clksrc | 1/4 Duty | 25 segments
	LCDCRB = (1<<LCDCS) | (1<<LCDMUX1) | (1<<LCDMUX0) | (1<<LCDPM2) | (1<<LCDPM1) | (1<<LCDPM0);
	//		divide by 8
	LCDFRR = (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);
	//		CC Max 3.35V
	LCDCCR = (1<<LCDCC3) | (1<<LCDCC2) | (1<<LCDCC1) | (1<<LCDCC0);
}

void TIM_init(void){
	//		CTC				PSC 1024
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);
	//		8MHz/1024/20
	OCR1A = 3906;
	//		Interrupt En
	TIMSK1 = (1<<OCIE1A);
	TCNT1 = 0;
}

void BTN_init(void){
	//		IT EN pins
	EIMSK|= (1<<PCIE1) | (1<<PCIE0);
	//		IT EN pin 15
	PCMSK1|= (1<<PCINT15);
	PCMSK1|= (1<<PCINT14);
	
	PCMSK0|= (1<<PCINT2);
	PCMSK0|= (1<<PCINT3);
	
	PCMSK1|= (1<<PCINT12);
	//		Pullup
	PORTB|= (1<<PINB7);
	PORTB|= (1<<PINB6);
	
	PORTE|= (1<<PINE3);
	PORTE|= (1<<PINE2);
	
	PORTB|= (1<<PINB4);
}

static uint16_t charNrStencils[10] = {0x1551, 0x0110, 0x1E11, 0x1B11, 0x0B50, 0x1B41, 0x1F41, 0x0111, 0x1F51, 0x1B51};
void writeChar(char c, int pos){
	if(pos < 0 && pos > 5) return;
	cli();
	uint8_t ch = c;
	ch-= 0x30;

	uint8_t shift = pos%2 ? 0x04 : 0x00;
	uint8_t mask = pos%2 ? 0x0F : 0xF0;
	uint16_t stencil = charNrStencils[ch];

	uint8_t *memPtr = &LCDDR0;
	memPtr+= pos/2;
	for(int iq = 0; iq < 4; iq++){
		*memPtr&= mask;
		*memPtr|= (stencil & 0x0F) << shift;
		stencil>>= 0x04;
		memPtr+= 0x05;
	}
	sei();
}
void clearChar(int pos){
	if(pos < 0 && pos > 5) return;

	uint8_t mask = pos%2 ? 0x0F : 0xF0;

	uint8_t *memPtr = &LCDDR0;
	for(int i = 0; i < 4; i++){
		*memPtr&= mask;
		memPtr+= 0x05;
	}
}

void printAt(int num, int pos) {
	int pp = pos;
	writeChar(((num/10) % 10)+0x30, pp);
	pp++;
	writeChar((num % 10)+0x30, pp);
}

void clearAt(int pos){
	clearChar(pos);
	clearChar(pos+1);
}


typedef struct {
	Object super;
	Timer timer;
	int freq;
	uint8_t pos;
	uint8_t *reg;
	uint8_t toggle;
} PWM;

PWM PWM1 = {initObject(), initTimer(),22,0,&PORTE, PINE4};
PWM PWM2 = {initObject(), initTimer(),1,4,&PORTE, PINE6};
PWM *PWMSEL;

void pwmsel(PWM **pwmn){
	PWM *p = *pwmn;
	if(!(PINE & (1<<PINE3))){
		//RIGHT
		PWMSEL = &PWM2;
	}else if(!(PINE & (1<<PINE2))){
		//LEFT
		PWMSEL = &PWM1;
	}
}
void pwminc(PWM **pwmn){
	PWM *p = *pwmn;
	if(!(PINB & (1<<PINB7))){
		if(p->freq > 0)
			p->freq--;
	}else if(!(PINB & (1<<PINB6))){
		if(p->freq < 99)
			p->freq++;
	}else if(!(PINB & (1<<PINB4))){
		//ENTER
		PWMSEL = NULL;
	}
	
	printAt(p->freq, p->pos);
}

void pwmloop(PWM *p, int arg){
	PORTE= 0;
	//PWM *p = *pwmn;
	if(p->freq){
		PORTE^= (1 << p->toggle);
		AFTER(MSEC(500/(p->freq)), p, pwmloop,0);
	}else{
		PORTE&= ~(1 << p->toggle);
		AFTER(SEC(1), p, pwmloop,0);
	}
}

void launch(Object * n, int a){
	PORTE|= (1 << PINE4);
}

int main() {
	sei();
	CLK_init();
	LCD_init();
	BTN_init();
	TIM_init();
	printAt((&PWM1)->freq, (&PWM1)->pos);
	printAt((&PWM2)->freq, (&PWM2)->pos);
	INSTALL(&PWMSEL, pwminc, IRQ_PCINT1);
	INSTALL(&PWMSEL, pwmsel, IRQ_PCINT0);
	//AFTER(SEC(2),&PWM2, pwmloop,0);
	T_RESET(&((&PWM1)->timer));
	AFTER(MSEC(500),&PWM1, pwmloop,0);
	T_RESET(&((&PWM2)->timer));
	return TINYTIMBER(&PWMSEL,launch,0);
}
/*
ISR(TIMER1_COMPA_vect){ // TIM IE

}
ISR(PCINT1_vect) { // BTN IE
	if(!(PINB & (1<<PINB7))){
		yield();
	}
}*/