#pragma config(Sensor, S1,     sonar_sensor,   sensorEV3_Ultrasonic)
#pragma config(Sensor, S3,     right_light_sensor, sensorEV3_Color)
#pragma config(Sensor, S4,     left_light_sensor, sensorLightActive)
#pragma config(Motor,  motorB,          motorLeft,     tmotorEV3_Large, PIDControl, reversed, driveLeft, encoder)
#pragma config(Motor,  motorA,          motorRight,    tmotorEV3_Large, PIDControl, reversed, driveRight, encoder)

static int rightHigh = 0;
static int leftHigh = 0;
static int rightLow = 100;
static int leftLow = 100;
static int rightThreshold = 50;
static int leftThreshold = 50;

static int isLeftDark_test = 0;
static int isRightDark_test = 0;

task processLightData_test();
void checkSensors();
void adjustRange(int rightSensorValue, int leftSensorValue);
int getSensorIsDark();

task main()
{
	startTask(processLightData_test);
	
	while (true) {
		checkSensors();
	}
}

task processLightData_test() {
    while (true) {
        int rightSensorValue = SensorValue[S3];
        int leftSensorValue = SensorValue[S4];

        adjustRange(rightSensorValue, leftSensorValue);

        if (rightSensorValue < rightThreshold) {
            isRightDark_test = 1;
        } else {
            isRightDark_test = 0;
        }

        if (leftSensorValue < leftThreshold) {
            isLeftDark_test = 1;
        } else {
            isLeftDark_test = 0;
        }
        displayBigTextLine(4, "L H: %d L: %d ", leftHigh, leftLow);
				displayBigTextLine(6, "R H: %d L: %d ", rightHigh, rightLow);
    }
}

void adjustRange(int rightSensorValue, int leftSensorValue) {

    if (rightSensorValue > rightHigh) {
        rightHigh = rightSensorValue;
    }

    if (rightSensorValue < rightLow) {
        rightLow = rightSensorValue;
    }

    if (leftSensorValue > leftHigh) {
        leftHigh = leftSensorValue;
    }

    if (leftSensorValue < leftLow) {
        leftLow = leftSensorValue;
    }

    rightThreshold = ( rightHigh + rightLow ) / 2;
    leftThreshold = ( leftHigh + leftLow ) / 2;

}

// 0 if neither, 1 if right, 2 if left, 3 if both
int getSensorIsDark() {
    return isLeftDark_test * 2 + isRightDark_test;
}

void checkSensors() {
	int senVal = getSensorIsDark();

	switch (senVal) {
		case (1) :
			displayBigTextLine(8, "RIGHT DARK");
			displayBigTextLine(12, "LEFT LIGHT");
			setLEDColor(ledRedPulse);
			setMotor(motorRight, 5);
			setMotor(motorLeft, 30);
			break;
		case (2) :
			displayBigTextLine(8, "LEFT DARK");
			displayBigTextLine(12, "RIGHT LIGHT");
			setLEDColor(ledOrangePulse);
			setMotor(motorLeft, 5);
			setMotor(motorRight, 30);
			break;
		case (3) :
			displayBigTextLine(8, "BOTH DARK");
			setLEDColor(ledGreenPulse);
			setMotor(motorLeft, 10);
			setMotor(motorRight, 10);
			break;
		default :
			displayBigTextLine(8, "LEFT LIGHT");
			displayBigTextLine(12, "RIGHT LIGHT");
			setMotor(motorLeft,15);
			setMotor(motorRight, 15);
			setLEDColor(ledOff);
			break;
	}
}
