
static int rightHigh = 0;
static int leftHigh = 0;
static int rightLow = 100;
static int leftLow = 100;
static int rightThreshold = 50;
static int leftThreshold = 50;

static int isLeftDark = 0;
static int isRightDark = 0;

void adjustRange(int right, int left);
int getSensorIsDark(int type);

task processLightData() {

    while (true) {
        int rightSensorValue = SensorValue[S3];
        int leftSensorValue = SensorValue[S4];
    
        adjustRange(rightSensorValue, leftSensorValue);

        if (rightSensorValue < rightThreshold) {
            isRightDark = 1;
        } else {
            isRightDark = 0;
        }

        if (leftSensorValue < leftThreshold) {
            isLeftDark = 1;
        } else {
            isLeftDark = 0;
        }
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
    return isLeftDark * 2 + isRightDark;
}

