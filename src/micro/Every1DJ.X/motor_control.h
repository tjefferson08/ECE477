/* 
 * File:   motor_control.h
 * Author: team2
 *
 * Created on April 7, 2014, 11:23 AM
 */

#ifndef MOTOR_CONTROL_H
#define	MOTOR_CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

void sendMotorControl(int state);
void initializeMotor();


#ifdef	__cplusplus
}
#endif

#endif	/* MOTOR_CONTROL_H */

