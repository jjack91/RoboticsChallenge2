static int rightHigh = 0;
static int leftHigh = 0;
static int rightLow = 100;
static int leftLow = 100;
static int rightThreshold = 50;
static int leftThreshold = 50;

static int isLeftDark_test = 0;
static int isRightDark_test = 0;

void adjustRange(int rightSensorValue, int leftSensorValue);
int getSensorIsDark();

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
