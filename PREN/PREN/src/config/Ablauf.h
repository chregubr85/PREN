/*
 * Ablauf.h
 *
 * Created: 27.03.2014 15:51:10
 *  Author: Christian
 */ 


#ifndef ABLAUF_H_
#define ABLAUF_H_


#define INIT_R1_IDX PIO_PA16_IDX
#define INIT_R2_IDX PIO_PA24_IDX
#define INIT_R1 PIO_PA16
#define INIT_R2 PIO_PA24
#define INIT_Z_IDX	PIO_PA23_IDX
#define INIT_Z PIO_PA23
#define ZYLINDER_ZACHSE PIO_PA2_IDX
#define ZYLINDER_STACK PIO_PA6_IDX

/* Define Maximalvalues */
#define MAXSTEPS		200000000
#define MAXVALUE_ENC_R1	2650
#define MAXVALUE_ENC_R2 3000//TODO
#define MAXVALUE_ENC_Z	-13390//TODO

/* Define Positions */
#define STARTPOSITION_Z 1377 // Encoderwert = -2.5 * Startposition_z (-6885 = -2.5 * 1377)
#define KINECTPOSITION	500	//TODO
#define PLACE_TOWER_R1	500 //TODO
#define PLACE_TOWER_R2	500 //TODO
#define PLACE_TOWER_Z	500 //TODO
#define TOWER_PLACED	500 //TODO r etwas zurück


uint32_t initialPosition(void);
bool startPosition(void);
bool gotoPositonKinect(void);
void getCube(uint32_t steps_r1, uint32_t steps_r2, uint32_t steps_z);
bool placeTower(void);
void ISR_INIT(uint32_t id, uint32_t mask);


#endif /* ABLAUF_H_ */