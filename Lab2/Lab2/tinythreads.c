#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "tinythreads.h"

#define NULL            0
#define DISABLE()       cli()
#define ENABLE()        sei()
#define STACKSIZE       80
#define NTHREADS        4
#define SETSTACK(buf,a) *((unsigned int *)(buf)+8) = (unsigned int)(a) + STACKSIZE - 4; \
                        *((unsigned int *)(buf)+9) = (unsigned int)(a) + STACKSIZE - 4

void BTN_init(void);
void TIM_init(void);

struct thread_block {
    void (*function)(int);   // code to run
    int arg;                 // argument to the above
    thread next;             // for use in linked lists
    jmp_buf context;         // machine state
    char stack[STACKSIZE];   // execution stack space
};

struct thread_block threads[NTHREADS];

struct thread_block initp;

thread freeQ   = threads;
thread readyQ  = NULL;
thread current = &initp;

int initialized = 0;

static void initialize(void) {
    int i;
    for (i=0; i<NTHREADS-1; i++)
        threads[i].next = &threads[i+1];
    threads[NTHREADS-1].next = NULL;

	BTN_init();
	TIM_init();
	
    initialized = 1;
}

static void enqueue(thread p, thread *queue) {
    p->next = NULL;
    if (*queue == NULL) {
        *queue = p;
    } else {
        thread q = *queue;
        while (q->next)
            q = q->next;
        q->next = p;
    }
}

static thread dequeue(thread *queue) {
    thread p = *queue;
    if (*queue) {
        *queue = (*queue)->next;
    } else {
        // Empty queue, kernel panic!!!
        while (1) ;  // not much else to do...
    }
    return p;
}

static void dispatch(thread next) {
    if (setjmp(current->context) == 0) {
        current = next;
        longjmp(next->context,1);
    }
}

void spawn(void (* function)(int), int arg) {
    thread newp;

    DISABLE();
    if (!initialized) initialize();

    newp = dequeue(&freeQ);
    newp->function = function;
    newp->arg = arg;
    newp->next = NULL;
    if (setjmp(newp->context) == 1) {
        ENABLE();
        current->function(current->arg);
        DISABLE();
        enqueue(current, &freeQ);
        dispatch(dequeue(&readyQ));
    }
    SETSTACK(&newp->context, &newp->stack);

    enqueue(newp, &readyQ);
    ENABLE();
}

void yield(void) {
	DISABLE();
	enqueue(current, &readyQ);
	dispatch(dequeue(&readyQ));
	ENABLE();
}

void lock(mutex *m) {
	DISABLE();
	if(m->locked == 0){
		m->locked = 1;
	}else{
		enqueue(current, &(m->waitQ));
		dispatch(dequeue(&readyQ));
	}
	ENABLE();
}

void unlock(mutex *m) {
	DISABLE();
	if(m->waitQ == NULL){
		m->locked = 0;
	}else{
		enqueue(current, &readyQ);
		dispatch(dequeue(&(m->waitQ)));
	}
	ENABLE();
}

void TIM_init(void){
	//		CTC				PSC 1024
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);
	//		8MHz/1024/20
	OCR1A = 391;
	//		Interrupt En
	TIMSK1 = (1<<OCIE1A);
	TCNT1 = 0;
}

void BTN_init(void){
	//		IT EN pins
	EIMSK = (1<<PCIE1);
	//		IT EN pin 15
	PCMSK1 = (1<<PCINT15);
	//		Pullup
	PORTB = (1<<PINB7);
}

ISR(PCINT1_vect) { // BTN IE
	if(!(PINB & (1<<PINB7)))
		yield();
}

ISR(TIMER1_COMPA_vect){ // TIM IE
	yield();
}