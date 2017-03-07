/*
 * codeUtils.h
 *
 * Created: 17/02/23 15:19:37
 *  Author: Daniel
 */

#ifndef CODEUTILS_H_
#define CODEUTILS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void LCD_init(void);

void BTN_init(void);

void writeChar(char c, int pos);

void clearChar(int pos);

void printAt(int num, int pos);

void clearAt(int pos);

#endif /* CODEUTILS_H_ */