#include <avr/io.h>
#include "tinythreads.h"

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

void BTN_init(void){
	PORTB = (1 << PINB7);
}

uint16_t charNrStencils[10] = {0x1551, 0x0110, 0x1E11, 0x1B11, 0x0B50, 0x1B41, 0x1F41, 0x0111, 0x1F51, 0x1B51};
void writeChar(char c, int pos){
	if(pos < 0 && pos > 5) return;
	uint8_t ch = c;
	ch-= 0x30;

	uint8_t shift = pos%2 ? 0x04 : 0x00;
	uint8_t mask = pos%2 ? 0x0F : 0xF0;
	uint16_t stencil = charNrStencils[ch];

	uint8_t *memPtr = &LCDDR0;
	memPtr+= pos/2;
	for(int i = 0; i < 4; i++){
		*memPtr&= mask;
		*memPtr|= (stencil & 0x0F) << shift;
		stencil>>= 0x04;
		memPtr+= 0x05;
	}
}

int is_prime(long nr){
	for(unsigned long i = 2; i*i < nr; i++)
	if(!(nr%i))
	return 0;
	return 1;
}

void printAt(long num, int pos) {
	int pp = pos;
	writeChar( (num % 100) / 10 + '0', pp);
	pp++;
	writeChar( num % 10 + '0', pp);
}

void blinkz(void){
	for(;;){
		while(tim1_cnt() < 10);
		tim1_rst();
		LCDDR13^= 0x01;
	}
}

void birdUp(void){
	uint8_t cnt = 0;
	for(;;){
		printAt(cnt,4);
		while(PINB & (0x1<<PINB7));
		cnt++;
		while(!(PINB & (0x1<<PINB7)));
	}
}

void computePrimes(int pos) {
	long n;

	for(n = 1; ; n++) {
		if (is_prime(n)) {
			printAt(n, pos);
			//yield();
		}
	}
}

int main() {
	CLK_init();
	LCD_init();
	BTN_init();
	
	spawn(birdUp, 0);
	spawn(blinkz, 0);
	
	computePrimes(0);
}
