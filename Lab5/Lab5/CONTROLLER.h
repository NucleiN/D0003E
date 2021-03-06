#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <avr/io.h>
#include "TinyTimber.h"

/*Sensor bits*/
#define NBCASA 0	/*Northbound car arrival sensor activated*/
#define NBBESA 1	/*Northbound bridge entry sensor activated*/
#define SBCASA 2	/*Southbound car arrival sensor activated*/
#define SBBESA 3	/*Southbound bridge entry sensor activated*/

/*Light status bits*/
#define NBGLS 0		/*Northbound green light status*/
#define NBRLS 1		/*Northbound red light status*/
#define SBGLS 3		/*Southbound green light status*/
#define SBRLS 4		/*Southbound red light status*/

typedef struct {
	int NBOB;	/*Northbound on bridge*/
	int SBOB;	/*Southbound on bridge*/
};


#endif /* CONTROLLER_H_ */