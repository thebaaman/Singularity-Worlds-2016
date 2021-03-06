#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    intakeLineBottom, sensorLineFollower)
#pragma config(Sensor, in2,    intakeLineTop,  sensorLineFollower)
#pragma config(Sensor, in3,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  flywheelEncoder, sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  ultrasonicRight, sensorSONAR_inch)
#pragma config(Sensor, I2C_1,  intakeEncoder,  sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  rightEncoder,   sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  leftEncoder,    sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           rub,           tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           fly1,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           lW3,           tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           lW1,           tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           lW2,           tmotorVex393TurboSpeed_MC29, openLoop, reversed, driveLeft, encoderPort, I2C_3)
#pragma config(Motor,  port6,           fly2,          tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           rW1,           tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           rW3,           tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           rW2,           tmotorVex393TurboSpeed_MC29, openLoop, reversed, driveRight, encoderPort, I2C_2)
#pragma config(Motor,  port10,          chn,           tmotorVex393_HBridge, openLoop, reversed, encoderPort, I2C_1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"
#include "vars.h"
#include "pid.c"
#include "preloads.c"
#include "utilities.c"
#include "autonswitcher.c"
#include "driveStraight.c"
#include "gyro.c"


void pre_auton() {
	SensorType[in3] = sensorNone;
  wait1Msec(1000);
  //Reconfigure Analog Port 3 as a Gyro sensor and allow time for ROBOTC to calibrate it
  SensorType[in3] = sensorGyro;
  wait1Msec(2000);

  stopFlywheelTasks();
	autonSelection();
}

void lcd() {
	clearLCDLine(0);
	clearLCDLine(1);

	displayLCDString(0, 0, "Primary: ");
	sprintf(mainBattery, "%1.2f%c", nImmediateBatteryLevel/1000.0,'V'); //Build the value to be displayed
	displayNextLCDString(mainBattery);

	displayLCDString(1, 0, "Backup: ");
	sprintf(backupBattery, "%1.2f%c", BackupBatteryLevel/1000.0, 'V');    //Build the value to be displayed
	displayNextLCDString(backupBattery);

	wait1Msec(100);
}

void chain() {
	if (vexRT[Btn5U] || vexRT[Btn6UXmtr2])
		motor[chn] = chainSpeed;
	else if (vexRT[Btn5D] || vexRT[Btn6DXmtr2])
		motor[chn] = -chainSpeed;
	else
		motor[chn] = 0;
}

void intake() {
	if (vexRT[Btn6U] || vexRT[Btn5UXmtr2])
		motor[rub] = 127;
	else if (vexRT[Btn6D] || vexRT[Btn5DXmtr2])
		motor[rub] = -127;
	else
		motor[rub] = 0;
}

void arcadeDrive() {
	if (abs(vexRT[Ch3]) > t)
		ch3 = vexRT[Ch3];
	else
		ch3 = 0;

	if (abs(vexRT[Ch1]) > t)
  	ch1 = vexRT[Ch1];
 	else
 		ch1 = 0;

	driveL(ch3 + ch1);
	driveR(ch3 - ch1);
}

void tankDrive() {
	if (abs(vexRT[Ch3]) > t)
		ch3 = vexRT[Ch3];
	else
		ch3 = 0;

	if (abs(vexRT[Ch2]) > t)
  	ch2 = vexRT[Ch2];
 	else
 		ch2 = 0;

	driveL(ch2);
	driveR(ch3);
}

void flywheel() {
	if (vexRT[Btn8L] || vexRT[Btn8LXmtr2]) { // bar
		//Target = 110;
		kp = 0.50;
	  ki = 0.36;
	  kd = 0.0;
	  chainSpeed = 127;

	  switchToBangBang(118);
	}
	if (vexRT[Btn8U] || vexRT[Btn8UXmtr2]) { // one tile
		//Target = 120;
		kp = 0.50;
	  ki = 0.36;
	  kd = 0.0;
	  chainSpeed = 127;

	  switchToBangBang(120);
	}if (vexRT[Btn8R] || vexRT[Btn8RXmtr2]) { // midfield
		//Target = 142;
		kp = 0.70;
	  ki = 0.36;
	  kd = 0.0;
	  chainSpeed = 127;

	  switchToPID(142);
	}

	if (vexRT[Btn7L] || vexRT[Btn7LXmtr2]) { // Corner of square
		//Target = 164;
		kp = 0.70;
	  ki = 0.6;
	  kd = 0.0;
	  chainSpeed = 127;

	  switchToBangBang(164);
	}

	if (vexRT[Btn7U] || vexRT[Btn7UXmtr2]) { // Square
		kp = 0.25;
	  ki = 0.75;
	  kd = 0.0;
		tolerance = 10;
	  chainSpeed = 127;

	  switchToBangBang(182);
	}

	if (vexRT[Btn8D] || vexRT[Btn8DXmtr2]) {
		stopFlywheelTasks();
	}
}

task usercontrol() {
	bLCDBacklight = true;

	startTask(RPM);
	startTask(error);

	while (true) {
		arcadeDrive();
		//tankDrive();
		flywheel();
		intake();
		chain();
		lcd();
	}
}

#include "autonomous.c"
