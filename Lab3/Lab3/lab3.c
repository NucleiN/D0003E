/*
 * Lab3.c
 *
 * Created: 2017-02-14 15:21:29
 * Author : Daniel Björk & Leo Ozolins-Carlson
 */ 

#include <avr/io.h>

int isPrime(unsigned long nr);
void writeLong(unsigned long nr);

//****************************************//
//**********        INIT        **********//
//****************************************//
void CLK_init(void) {
	//EN Write to PSC reg
	CLKPR = 0x80;
	//PSC 0
	CLKPR = 0x00;
}
void TIM_init(void) {
	//256 PSC
	TCCR1B = (1 << CS12);
}
void BTN_init(void) {
	PORTB = (1 << PINB7);
}
void LCD_init(void) {
	//		Enable LCD | LPow Waveform
	LCDCRA = (1 << LCDEN) | (1 << LCDAB);
	//		Async clksrc | 1/4 Duty | 25 segments
	LCDCRB = (1 << LCDCS) | (1 << LCDMUX1) | (1 << LCDMUX0) | (1 << LCDPM2) | (1 << LCDPM1) | (1 << LCDPM0);
	//		divide by 8
	LCDFRR = (1 << LCDCD2) | (1 << LCDCD1) | (1 << LCDCD0);
	//		CC Max 3.35V
	LCDCCR = (1 << LCDCC3) | (1 << LCDCC2) | (1 << LCDCC1) | (1 << LCDCC0);
}
//****************************************//
//**********        LCD         **********//
//****************************************//
uint16_t charNrStencils[10] = {
	0x1551,
	0x0110,
	0x1E11,
	0x1B11,
	0x0B50,
	0x1B41,
	0x1F41,
	0x0111,
	0x1F51,
	0x1B51
};
void writeChar(char c, int pos) {
	if (pos < 0 && pos > 5) return;
	uint8_t ch = c;
	ch -= 0x30;

	uint8_t shift = pos % 2 ? 0x04 : 0x00;
	uint8_t mask = pos % 2 ? 0x0F : 0xF0;
	uint16_t stencil = charNrStencils[ch];

	uint8_t * memPtr = & LCDDR0;
	memPtr += pos / 2;
	for (int i = 0; i < 4; i++) { * memPtr &= mask; * memPtr |= (stencil & 0x0F) << shift;
		stencil >>= 0x04;
		memPtr += 0x05;
	}
}
void clearChar(int pos) {
	if (pos < 0 && pos > 5) return;

	uint8_t mask = pos % 2 ? 0xF0 : 0x0F;

	uint8_t * memPtr = & LCDDR0;
	for (int i = 0; i < 4; i++) { * memPtr &= mask;
		memPtr += 0x05;
	}
}
void writeLong(unsigned long nr) {
	for (int i = 0;
	(i < 6); i++) {
		if (nr > 0) {
			uint8_t tnr = nr % 10;
			writeChar(tnr + 48, 5 - i);
			nr /= 10;
			} else {
			clearChar(i);
		}
	}
}
//****************************************//
//**********       PRIME        **********//
//****************************************//
void prime() {
	for (unsigned long i = 1;; i++)
	if (isPrime(i)) {
		writeLong(i);
		for (unsigned int j = 0; j < 65000; j++);
		for (unsigned int j = 0; j < 65000; j++);
	}
}
int isPrime(unsigned long nr) {
	for (unsigned long i = 2; i * i < nr; i++)
	if (!(nr % i))
	return 0;
	return 1;
}
void primes(unsigned long * nr) {
	for (unsigned long i = 2;
	(i * i) < ( * nr); i++) {
		if (!(( * nr) % i)) {
			( * nr) ++;
			return;
		}
	}
	writeLong( * nr);
	( * nr) ++;
}
//****************************************//
//**********       BLINK        **********//
//****************************************//
void blink() {
	while (1) {
		while (TCNT1 <= 0x3D09);
		LCDDR13 ^= 0x01;
		TCNT1 = 0x00;
	}
}
void blinks() {
	if (TCNT1 >= 0x3D09) {
		LCDDR13 ^= 0x01;
		TCNT1 = 0x00;
	}
}
//****************************************//
//**********       BUTTON       **********//
//****************************************//
void button() {
	while (1) {
		LCDDR18 = (LCDDR18 & 0x01) ? 0x00 : 0x01;
		LCDDR8 = ((~LCDDR18) & 0x01);
		while (PINB & (0x1 << PINB7));
		while (!(PINB & (0x1 << PINB7)));
	}
}
void buttons(int * state) {
	if ((PINB & (0x1 << PINB7)) && * state) {
		LCDDR18 = (LCDDR18 & 0x01) ? 0x00 : 0x01;
		LCDDR8 = (~LCDDR18) & 0x01; * state = 0x0;
		} else if (!((PINB & (0x1 << PINB7)) || * state)) { * state = 0x1;
	}
}