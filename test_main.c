#pragma config(Sensor, S1,     sonar_sensor,   sensorEV3_Ultrasonic)
#pragma config(Sensor, S3,     right_light_sensor, sensorEV3_Color)
#pragma config(Sensor, S4,     left_light_sensor, sensorLightActive)
#pragma config(Motor,  motorA,          motorLeft,     tmotorEV3_Large, PIDControl, reversed, driveLeft, encoder)
#pragma config(Motor,  motorB,          motorRight,    tmotorEV3_Large, PIDControl, reversed, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "lightDataProducer_test.c"


void checkSensors() {
	int senVal = getSensorIsDark();

	switch (senVal) {
		case (1) :
			displayBigTextLine(8, "RIGHT DARK");
			displayBigTextLine(12, "LEFT LIGHT");
			setLEDColor(ledRedPulse);
			setMotor(motorLeft, 50);
			break;
		case (2) :
			displayBigTextLine(8, "LEFT DARK");
			displayBigTextLine(12, "RIGHT LIGHT");
			setLEDColor(ledOrangePulse);
			setMotor(motorRight, 50);
			break;
		case (3) :
			displayBigTextLine(8, "BOTH DARK");
			setLEDColor(ledGreenPulse);
			setMotor(motorLeft, 40);
			setMotor(motorRight, 40);
			break;
		default :
			displayBigTextLine(8, "LEFT LIGHT");
			displayBigTextLine(12, "RIGHT LIGHT");
			setMotor(motorLeft, 30);
			setMotor(motorRight, 30);
			setLEDColor(ledOff);
			break;
	}
}

task main()
{
	startTask(processLightData_test);
	
	while (true) {
		checkSensors();
	}
}