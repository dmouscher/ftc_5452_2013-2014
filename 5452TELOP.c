#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     light,          sensorLightActive)
#pragma config(Sensor, S3,     stopper,        sensorTouch)
#pragma config(Sensor, S4,     ir,             sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     motorUL,       tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorUR,       tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     arm,           tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     flag,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     motorDL,       tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     motorDR,       tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Servo,  srvo_S1_C2_1,    armleft,              tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    armright,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    pinballleft,          tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    pinballright,         tServoStandard)
#pragma config(Servo,  srvo_S1_C2_5,    flagservo,            tServoStandard)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//*************************************************************************************//
//*                                   5452TELOP                                       *//
//*************************************************************************************//

// 5452 Teleop Program
// Revision History
// Rev 1	05-NOV-2013	First try out code
// Rev 2	07-NOV-2013	Add Kill Switch routine when hanging to prevent stress on servos
//                    Code use at BarringtonHS scrimmage
// Rev 3	19-NOV-2013	Add control for Flag Servo
//                    Remove Kill Switch routine (no longer needed)
//                    Add Arm motor touch switch sensing to stop Arm motor in Rest pos.
//										Code use at IL IIT Qualifying Regional
// Rev 4	16-JAN-2014	Add servo initializing routine
//										Reduce proportional drive motor speed control to 80%
// Rev 5  11-FEB-2014 Added slow button to help drivers increase persision
//
// Rev 6  ??-???-2014 Retired number, but not the honorable jersey number kind of retired
//
// Rev 7  27-FEB-2014 Remapped slowmode
// 										Made a seperate instance of slowmode for each driver
//                    Maybe fixed the pinball problem, maybe made it worse
//
// Rev 8  02-MAR-2014 Rewrote to "work" with Mecanum wheels
//										Stuffed motor code into a subroutine
//										Added more subroutines because #yolo
//*************************************************************************************//

		//|| 1 /---------\ 2
		//|| | |
		//|| | |
		//|| | |
		//|| | |
		//|| 3 \---------/ 4

#include "JoystickDriver.c"

int xisgn(int x1);
int ysign(int y1);
void motormove(int UL, int UR, int DL, int DR, int speedmod);



task main()
{
	int deadzone = 6;
	int armpos = 230;
	int pinballleftpos = 0;
	int pinballrightpos = 0;
	int flagpos = 0;
	int speedmodi = 1;
	int speedmodii = 1;
	// Initialize Servo positions
	servo[armleft] = 230;
	servo[armright] = 25;
	wait1Msec(250);
	servo[pinballleft] = 255;
	servo[pinballright] = 0;
	servo[flagservo] = 0;

	waitForStart();

	int x1Val = 0;
	int y1Val = 0;
	int x2Val = 0;
	int y2Val = 0;
	int boundBox = 20; //deadzone value
	int scale = 1; //dont worry about it unless you want slow mode
	int motor1Val = 0;
	int motor2Val = 0;
	int motor3Val = 0;
	int motor4Val = 0;

	while(true)
	{
		getJoystickSettings(joystick);

		if(abs(joystick.joy1_x1) > boundBox)
		{
			x1Val = joystick.joy1_x1 / scale * 127 / 80;
		}
		else
		{
			x1Val = 0;
		}
		if(abs(joystick.joy1_y1) > boundBox)
		{
			y1Val = joystick.joy1_y1 / scale * 127 / 80;
		}
		else
		{
			y1Val = 0;
		}
		if(abs(joystick.joy1_x2) > boundBox)
		{
			x2Val = joystick.joy1_x2 / scale * 127 / 80;
		}
		else
		{
			x2Val = 0;
		}
		if(abs(joystick.joy1_y2) > boundBox)
		{
			y2Val = joystick.joy1_y2 / scale * 127 / 80;
		}
		else
		{
			y2Val = 0;
		}
    //main shit
		motor1Val = -y1Val - x1Val - x2Val;
		motor2Val = y1Val - x1Val - x2Val;
		motor3Val = -y1Val + x1Val - x2Val;
		motor4Val = y1Val + x1Val - x2Val;
		//move motors
		motor[motorUL] = motor1Val;
		motor[motorUR] = motor2Val;
		motor[motorDL] = motor3Val;
		motor[motorDR] = motor4Val;

		//new from 57-72 //These line numbers are hella outdated
		if(joystick.joy2_TopHat == 0 && SensorValue[stopper] != 1)
		{
			motor[arm] = (-85);
		}
		else if(joystick.joy2_TopHat == 4 )//&& SensorValue[stopper] != 1)
		{
			motor[arm] = (85);
		}
		else if(SensorValue[stopper] == 1)
		{
			motor[arm] = 0;
		}
		else
		{
			motor[arm] = 0;
		}

		//refer to "accTest" for failed accseleration(such sp) code

		if(abs(joystick.joy2_x1) > deadzone)
		{
			motor[flag] = (joystick.joy2_x1/speedmodii);
		}
		else
		{
			motor[flag] = 0;
		}

		if(abs(joystick.joy2_y2) > deadzone)
		{
			if(joystick.joy2_y2 < 0 && armpos < 255)
			{
				armpos++;
				wait1Msec(5);
			}

			if(joystick.joy2_y2 > 0 && armpos > 0)
			{
				armpos--;
				wait1Msec(5);
			}
		}
		if(joy2Btn(1)  == 1 && pinballleftpos > 0)
		{
			pinballleftpos--;
		}

		if(joy2Btn(4) == 1 && pinballrightpos > 0)
		{
			pinballrightpos--;
		}

		if(joy2Btn(3) == 1 && pinballleftpos < 128)
		{
			pinballleftpos++;
		}

		if(joy2Btn(2) == 1 && pinballrightpos < 127)
		{
			pinballrightpos++;
		}

		if(joystick.joy2_TopHat == 6 && flagpos > 0)
		{
			flagpos--;
		}

		if(joystick.joy2_TopHat == 2 && flagpos < 255)
		{
			flagpos++;
		}

		if(joy1Btn(7) == 1)
		{
			scale = 1;
		}
		else if(joy1Btn(8) == 1)
		{
			scale = 4;
		}

		if(joy2Btn(7) == 1)
		{
			speedmodii = 1;
		}
		else if(joy2Btn(8) == 1)
		{
			speedmodii = 15;
		}

		/*
		if(joy2Btn(7) == 1)
		{
		flagspeed = 25;
		}

		if(joy2Btn(8) == 1)
		{
		flagspeed = 1;
		}

		if(joy1Btn(2) == 1 && joy1Btn(3) == 1)
		{
		break;
		}

		*/

		servo[armleft] = armpos;
		servo[armright] = (255 - armpos);
		servo[pinballleft] = (255 - pinballleftpos);
		servo[pinballright] = pinballrightpos;
		servo[flagservo] = flagpos;
	}
}

int xsign(int x1) //checks sign of joystick.joy1.x1
{
		if(x1 < 0)
		{
			return -1;
		}
		else
		{
			return 1;
		}
}

int ysign(int y1) //checks sign of joystick.joy1.y1
{
		if(y1 < 0)
		{
			return -1;
		}
		else
		{
			return 1;
		}
}

//xsign and ysign are not entirely necessary, but makes the code look a little neater and prevents DBZ errors (although that shouldn't happen with the way the program is set up)
//If the repeated subroutine calling slows the program, xsign/ysign can be replaced with abs(a)/a

void motormove(int UL, int UR, int DL, int DR, int speedmod) //Let's shove all the motor code into here and pray that nothing breaks.
{
	//limit max speed to 80
	motor[motorUL]	= (UL*127/80/speedmod);
	motor[motorUR]	= (UR*127/80/speedmod);
	motor[motorDL]	= (DL*127/80/speedmod);
	motor[motorDR]	= (DR*127/80/speedmod);
}
