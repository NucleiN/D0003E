/*
 * codeUtils.c
 *
 * Created: 17/02/23 15:19:22
 *  Author: Daniel
 */

#include "codeUtils.h"

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

void BTN_init(void) {
    EIMSK |= (1 << PCIE1) | (1 << PCIE0);

    PCMSK1 |= (1 << PCINT15) | (1 << PCINT14) | (1 << PCINT12);
    PORTB |= (1 << PINB7) | (1 << PINB6) | (1 << PINB4);

    PCMSK0 |= (1 << PCINT2) | (1 << PCINT3);
    PORTE |= (1 << PINE3) | (1 << PINE2);
}

static uint16_t charNrStencils[10] = {0x1551, 0x0110, 0x1E11, 0x1B11, 0x0B50, 0x1B41, 0x1F41, 0x0111, 0x1F51, 0x1B51};

void writeChar(char c, int pos) {
    if (pos < 0 && pos > 5) return;
    cli();
    uint8_t ch = c;
    ch -= 0x30;

    uint8_t shift = pos % 2 ? 0x04 : 0x00;
    uint8_t mask = pos % 2 ? 0x0F : 0xF0;
    uint16_t stencil = charNrStencils[ch];

    uint8_t *memPtr = &LCDDR0;
    memPtr += pos / 2;
    for (int iq = 0; iq < 4; iq++) {
        *memPtr &= mask;
        *memPtr |= (stencil & 0x0F) << shift;
        stencil >>= 0x04;
        memPtr += 0x05;
    }
    sei();
}

void clearChar(int pos) {
    if (pos < 0 && pos > 5) return;
    cli();
    uint8_t mask = pos % 2 ? 0x0F : 0xF0;

    uint8_t *memPtr = &LCDDR0;
    memPtr += pos / 2;
    for (int i = 0; i < 4; i++) {
        *memPtr &= mask;
        memPtr += 0x05;
    }
    sei();
}

void printAt(int num, int pos) {
    int pp = pos;
    writeChar(((num / 10) % 10) + 0x30, pp);
    pp++;
    writeChar((num % 10) + 0x30, pp);
}

void clearAt(int pos) {
    clearChar(pos);
    clearChar(pos + 1);
}