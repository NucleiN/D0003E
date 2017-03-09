#include <avr/io.h>
#include <avr/interrupt.h>
#include "TinyTimber.h"

#include "PWM.h"
#include "GUI.h"
#include "KEY.h"

static GUI GUI1 = init_GUI(init_PWM(&PORTE, PINE4), 0);
static GUI GUI2 = init_GUI(init_PWM(&PORTE, PINE6), 4);
static KEY KEYN = init_KEY(&GUI1, &GUI2);

void launch(Object *o, int arg) {
	GUI_init(&GUI1);
	GUI_init(&GUI2);
}

int main() {
	sei();  //Enables interrupts 

	LCD_init();
	BTN_init();

	INSTALL(&KEYN, KEY_pwmInc, IRQ_PCINT1);
	INSTALL(&KEYN, KEY_pwmSel, IRQ_PCINT0);

	return TINYTIMBER(NULL, launch, 0);
}