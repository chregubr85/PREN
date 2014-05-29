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
#define MAXSTEPS		20000
#define MAXVALUE_ENC_R1	2480
#define MAXVALUE_ENC_R2 2990
#define MAXVALUE_ENC_Z	13800

/* Define Positions */
#define STARTPOSITION_Z 7200
#define KINECTPOSITION_90	500	//TODO
#define KINECTPOSITION_135	500	//TODO
#define KINECTPOSITION_180	500	//TODO
#define PLACE_TOWER_R1	400 //TODO
#define PLACE_TOWER_R2	400 //TODO
#define PLACE_TOWER_Z	7200 
#define TOWER_PLACED	0


uint32_t initialPosition(void);
bool startPosition(void);
bool gotoPositonKinect(void);
void getCube(uint32_t steps_r1, uint32_t steps_r2, uint32_t steps_z);
bool placeTower(void);
void ISR_INIT(uint32_t id, uint32_t mask);


#endif /* ABLAUF_H_ */