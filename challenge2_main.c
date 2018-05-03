#pragma config(Sensor, S1,     sonar_sensor,   sensorEV3_Ultrasonic)
#pragma config(Sensor, S3,     right_light_sensor, sensorEV3_Color)
#pragma config(Sensor, S4,     left_light_sensor, sensorLightActive)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**********Includes**********/
#include "timeProducer.c"
#include "speedProducer.c"
#include "sonarProducer.c"
#include "lightDataProducer_alt.c"

/**********Defined Variables**********/
#define LEFT_SENSOR S4
#define RIGHT_SENSOR S3
#define LEFT_MOTOR motorA
#define RIGHT_MOTOR motorB

/**********Defined Constants*********/
#define WAIT_HALF_SEC 500
#define WAIT_FULL_SEC 1000
#define WAIT_TWO_SEC 2000

#define RAND_CHANCE_LOWER 36
#define RAND_CHANCE_UPPER 70

/**********Enums**********/
typedef enum Direction {
					  DIR_LEFT,
						DIR_RIGHT,
						DIR_STRAIGHT};

typedef enum Turn {
					 TURN_LEFT,
				   TURN_RIGHT,
				   TURN_AROUND};

typedef enum Sensor {
					 SENSOR_NONE = -1,
					 SENSOR_LEFT,
					 SENSOR_RIGHT,
					 SENSOR_BOTH,
					 SENSOR_SONAR};

typedef enum Speed {
				  SPEED_LOW = -15,
					SPEED_HIGH = -40,
					SPEED_REVERSE = 25,
					SPEED_TURN_FORWARD = -30,
					SPEED_TURN_BACKWARD = 30};

/**********Private Constants**********/
static const int MAX_ROLL = 95;
static const int INTERRUPT_CHECK_MS = 100;
static const int INTERRUPT_CHECK_MS2 = 80;

/**********Global Variables***********/
static int thresholdLeft;
static int thresholdRight;
static int lineFollow;
static int objectDetect;

/**********Private Function Prototypes**********/
static void walk();
static Direction chooseDirection();
static void chooseTurnDirection();
static void traverse(Direction dir);
static void veer(Direction dir, int forwardSpeed, int speed);
static void turn(Turn turn);
static void reverse();
static void stopMotors();
//static int sensorBump();
static int sensorDetect();
static void calibrate();
static void rightSensorTest();
static void leftSensorTest();
static void bothSensorTest();
int getLightSensorData(Sensor theSensor);
static void createLeftThreshold(int white);
static void createRightThreshold(int white);
static void moveTowardsObject();


/* Main starting point of the program. */
task main() {

	//startTask(processLightData);
	lineFollow = 0;
	calibrate();
	//bothSensorTest();


	// Starts the population of the time buffer in its own thread.
	startTask(populateTimes);
	startTask(populateSpeeds);
	startTask(populateSonarValues);



	// The loop for the robot's movement.
	while(true) {
		walk();
	}
}

/* Drunken sailor movement function */
static void walk() {
	Direction dir = chooseDirection();
	traverse(dir);
}

/* Chooses a veer direction: left, Right, or straight. */
static Direction chooseDirection() {
	int randomNumber = random(MAX_ROLL - 1) + 1;
	if (randomNumber < RAND_CHANCE_LOWER) {
		return DIR_LEFT;
	} else if (randomNumber > RAND_CHANCE_LOWER && randomNumber < RAND_CHANCE_UPPER) {
		return DIR_RIGHT;
	} else {
		return DIR_STRAIGHT;
	}
}

/* Chooses a turn direction: left, right, or around. */
static void chooseTurnDirection() {
	int randomNumber = random(MAX_ROLL - 1) + 1;
	if (randomNumber < RAND_CHANCE_LOWER) {
		turn(TURN_LEFT);
	} else if (randomNumber > RAND_CHANCE_LOWER && randomNumber < RAND_CHANCE_UPPER) {
		turn(TURN_RIGHT);
	} else {
		turn(TURN_AROUND);
	}
}

/* Causes the robot to traverse for a specific time grabbed from
	 the timeBuffer located in producer.c */
static void traverse(Direction dir) {
	int time = getTime();
	int speed = getSpeed();
	veer(dir, SPEED_HIGH, SPEED_LOW);

	// Duration of traversing with sensor checks.
	for(int i = 0; i < time; i += INTERRUPT_CHECK_MS) {
		int status = sensorDetect();
		displayBigTextLine(4, "ThreshL = %d", thresholdLeft);
		displayBigTextLine(6, "ThreshR = %d", thresholdRight);
		displayBigTextLine(8, "LookL = %d", SensorValue[S4]);
		displayBigTextLine(10, "LookR = %d", SensorValue[S3]);
		if (status >= 1) {
			if(status < 4)
			{
				lineFollow = status;
			}
			return;
		}
		else if(status == 0 && lineFollow != 0)
		{

			status = 0;
			turn(TURN_RIGHT);
			stopMotors();
			status = sensorDetect();
			if(status >= 1)
			{
				lineFollow = 0;
				return;
			}
			turn(TURN_AROUND);
			stopMotors();
			status = sensorDetect();
			lineFollow = 0;
			if(status >= 1)
			{
				return;
			}
			turn(TURN_RIGHT);
			stopMotors();
			playSound(soundBeepBeep);
			lineFollow = 0;
		}

		setLEDColor(ledGreen);
		lineFollow = 0;
		sleep(INTERRUPT_CHECK_MS);
		displayBigTextLine(10, "");
	}
}

/* Sets the wheel speeds to cause a veer (or go straight). */
static void veer(Direction dir, int forwardSpeed, int speed) {
	switch (dir) {
		case (DIR_LEFT) :
			motor(LEFT_MOTOR) = speed;
			motor(RIGHT_MOTOR) = forwardSpeed;
			break;
		case (DIR_RIGHT) :
			motor(LEFT_MOTOR) = forwardSpeed;
			motor(RIGHT_MOTOR) = speed;
			break;
		case (DIR_STRAIGHT) :
			motor(LEFT_MOTOR) = forwardSpeed;
			motor(RIGHT_MOTOR) = forwardSpeed;
			break;
		default : // Invalid direction.
			return;
	}
}

/* Function for turning the robot based off of the supplied turn. */
static void turn(Turn turn) {
	switch (turn) {
		case (TURN_LEFT) :
			motor(LEFT_MOTOR) = SPEED_TURN_BACKWARD;
			motor(RIGHT_MOTOR) = SPEED_TURN_FORWARD;
			for(int i = 0; i < WAIT_HALF_SEC; i += INTERRUPT_CHECK_MS)
			{
				if (sensorDetect() >= 1) {
					//lineFollow = status;
					return;
				}
				sleep(INTERRUPT_CHECK_MS);
			}
			break;
		case (TURN_RIGHT) :
			motor(LEFT_MOTOR) = SPEED_TURN_FORWARD;
			motor(RIGHT_MOTOR) = SPEED_TURN_BACKWARD;
			for(int i = 0; i < WAIT_HALF_SEC; i += INTERRUPT_CHECK_MS)
			{
				if (sensorDetect() >= 1) {
					//lineFollow = status;
					return;
				}
				sleep(INTERRUPT_CHECK_MS);
			}
			break;
		case (TURN_AROUND) :
			motor(LEFT_MOTOR) = SPEED_TURN_BACKWARD;
			motor(RIGHT_MOTOR) = SPEED_TURN_FORWARD;
			for(int i = 0; i < WAIT_FULL_SEC; i += INTERRUPT_CHECK_MS)
			{
				if (sensorDetect() >= 1) {
					//lineFollow = status;
					return;
				}
				sleep(INTERRUPT_CHECK_MS);
			}
			break;
		default : // Invalid turn.
			return;
	}
}

/* Causes the robot to reverse. */
static void reverse() {
	motor(LEFT_MOTOR) = SPEED_REVERSE;
	motor(RIGHT_MOTOR) = SPEED_REVERSE;
	sleep(WAIT_HALF_SEC);
}

/* Stops all movement of the robot. */
static void stopMotors() {
	motor(LEFT_MOTOR) = 0;
	motor(RIGHT_MOTOR) = 0;
	sleep(WAIT_HALF_SEC);
}

///* Interupt for checking if a sensor was bumped. */
///* Deprecated for Challenge 2. Keep for reference.*/
//static int sensorBump() {

//	Sensor sensor = SENSOR_NONE;

//	// Check if the left sensor was bumped.
//	if (SensorValue(LEFT_SENSOR) ==  1) {
//		sleep(10);
//		// Checks if the right sensor was bumped after 10 ms which
//		// counts as both sensors being bumped.
//		if (SensorValue(RIGHT_SENSOR) ==  1) {
//			sensor = SENSOR_BOTH;
//		} else { // Just the left sensor was bumped.
//			sensor = SENSOR_LEFT;
//		}
//	// Check if the right sensor was bumped.
//	} else if (SensorValue(RIGHT_SENSOR) ==  1) {
//		sleep(10);
//		// Checks if the left sensor was bumped after 10 ms which
//		// counts as both sensors being bumped.
//		if (SensorValue(LEFT_SENSOR) ==  1) {
//			sensor = SENSOR_BOTH;
//		} else { // Just the right sensor was bumped.
//			sensor = SENSOR_RIGHT;
//		}
//	}

//	// Finds out which sensor(s) was/were bumped.
//	switch (sensor) {
//		case (SENSOR_BOTH) :
//			playSound(soundBeepBeep);
//			reverse();
//			stopMotors();
//			chooseTurnDirection();
//			return 1;
//		case (SENSOR_LEFT) :
//			reverse();
//			turn(TURN_RIGHT);
//			return 1;
//		case (SENSOR_RIGHT) :
//			reverse();
//			turn(TURN_LEFT);
//			return 1;
//		default : // SENSOR_NONE
//			return 0;
//	}
//}

static int sensorDetect() {

	Sensor sensor = SENSOR_NONE;
	if (objectDetect == 1) {
		return 0;
	}


	// Check if the left sensor was bumped.
	if (sonar_isObjectFound() == 1 && sonar_getDistance() >= 5) {
		lineFollow = 0;
		sleep(10);
		if (sonar_isObjectFound() == 1) {
			lineFollow = 0;
			sensor = SENSOR_SONAR;
		}
	} else if (getLightSensorData(SENSOR_LEFT) == 1) {
		sleep(10);
		// Checks if the right sensor was bumped after 10 ms which
		// counts as both sensors being bumped.
		if (getLightSensorData(SENSOR_RIGHT) == 1) {
			sensor = SENSOR_BOTH;
		} else { // Just the left sensor was bumped.
			sensor = SENSOR_LEFT;
		}
	// Check if the right sensor was bumped.
	} else if (getLightSensorData(SENSOR_RIGHT) == 1) {
		sleep(10);
		// Checks if the left sensor was bumped after 10 ms which
		// counts as both sensors being bumped.
		if (getLightSensorData(SENSOR_LEFT) == 1) {
			sensor = SENSOR_BOTH;
		} else { // Just the right sensor was bumped.
			sensor = SENSOR_RIGHT;
		}
	}

	// Finds out which sensor(s) was/were bumped.
	switch (sensor) {
		case (SENSOR_BOTH) :
		//	playSound(soundBeepBeep);
		//	reverse();
		//	stopMotors();
		//	chooseTurnDirection();
			setLEDColor(ledRedFlash);
			veer(DIR_STRAIGHT, -25, SPEED_LOW);
			lineFollow = 1;
			sleep(100);
			return 1;
		case (SENSOR_LEFT) :
		//	reverse();
		//	turn(TURN_RIGHT);
			setLEDColor(ledOff);
			veer(DIR_LEFT, -25, 15);
			lineFollow = 1;
			//turn(TURN_LEFT);
			sleep(100);
			return 2;
		case (SENSOR_RIGHT) :
		//	reverse();
		//	turn(TURN_LEFT);
			setLEDColor(ledRed);
			veer(DIR_RIGHT, -25, 15);
			//turn(TURN_RIGHT);
			lineFollow = 1;
			sleep(100);
			return 3;
		case (SENSOR_SONAR) :
			setLEDColor(ledGreenFlash);
			stopMotors();
			lineFollow = 0;
			objectDetect = 1;
			moveTowardsObject();
			stopMotors();
			sleep(100);
			return 4;
		default : // SENSOR_NONE
			return 0;
	}
}

static void calibrate()
{
	displayBigTextLine(4, "Place light");
	displayBigTextLine(6, "sensors over");
	displayBigTextLine(8, "WHITE");

	/*sleep(1000);
	displayBigTextLine(1, "5");
	sleep(1000);
	displayBigTextLine(1, "4");
	sleep(1000);
	displayBigTextLine(1, "3");
	sleep(1000);
	displayBigTextLine(1, "2");
	sleep(1000);
	displayBigTextLine(1, "1");
	sleep(1000);*/
	displayBigTextLine(1, "0 ... Reading...");
	int whiteL = readLightSensor(1);
	int whiteR = readLightSensor(2);
	createLeftThreshold(whiteL);
	motor(LEFT_MOTOR) = SPEED_TURN_BACKWARD;
	motor(RIGHT_MOTOR) = SPEED_TURN_FORWARD;
	sleep(WAIT_FULL_SEC);
	createRightThreshold(whiteR);
	reverse();

	displayBigTextLine(1, "Running");
	displayBigTextLine(4, "");
	displayBigTextLine(6, "");
	displayBigTextLine(8, "");
}

static void rightSensorTest()
{
	int white = SensorValue[S3];
	motor(LEFT_MOTOR) = SPEED_LOW;
	motor(RIGHT_MOTOR) = SPEED_LOW;

	while(SensorValue[S3] >= white/2)
	{
		sleep(100);
	}
	motor(LEFT_MOTOR) = 0;
	motor(RIGHT_MOTOR) = 0;
}

static void leftSensorTest()
{
	int white = SensorValue[S4];
	motor(LEFT_MOTOR) = SPEED_LOW;
	motor(RIGHT_MOTOR) = SPEED_LOW;

	while(SensorValue[S4] >= white/2)
	{
		sleep(100);
	}
	motor(LEFT_MOTOR) = 0;
	motor(RIGHT_MOTOR) = 0;
}

static void bothSensorTest()
{
	int whiteL = SensorValue[S4];
	int whiteR = SensorValue[S3];
	motor(LEFT_MOTOR) = SPEED_LOW;
	motor(RIGHT_MOTOR) = SPEED_LOW;

	while(SensorValue[S4] >= whiteL-5 || SensorValue[S3] >= whiteR-5)
	{
		sleep(100);
	}
	motor(LEFT_MOTOR) = 0;
	motor(RIGHT_MOTOR) = 0;
}

int getLightSensorData(Sensor theSensor)
{
	int leftReadings[10];

	if(theSensor == SENSOR_RIGHT)
	{
		if(SensorValue[S4] < thresholdLeft)
		{
			return 1;
		}
		else
		{
			return 0;
		}

	}
	else if(theSensor == SENSOR_LEFT)
	{
		if(SensorValue[S3] < thresholdRight)
		{
			return 1;
		}
		else
		{
			return 0;
		}

	}
	else if(theSensor == SENSOR_BOTH)
	{
		if((SensorValue[S4] < thresholdLeft) && (SensorValue[S3] < thresholdRight))
		{
			return 2;
		}
		else
		{
			return 0;
		}

	}
	else
	{
		displayBigTextLine(10, "Sensor return Error");
		return 0;
	}
}

static void createLeftThreshold(int white)
{
	//white = SensorValue[S4];
	motor(LEFT_MOTOR) = SPEED_LOW;
	motor(RIGHT_MOTOR) = SPEED_LOW;
	int min = 100;
	int max = 0;
	int i = 0;
	while(i < 50)
	{
		sleep(100);
		int reading = SensorValue[S4];
		if(reading > max)
		{
			max = reading;
		}
		if(reading < min)
		{
			min = reading;
		}
		displayBigTextLine(4, "Max = %d", max);
		displayBigTextLine(6, "Min = %d", min);
		i++;
	}
	motor(LEFT_MOTOR) = 0;
	motor(RIGHT_MOTOR) = 0;
	//sleep(1000);
	//int black = SensorValue[S4];
	thresholdLeft = (max + min)/2;
}

static void createRightThreshold(int white)
{
	//white = SensorValue[S3];
	motor(LEFT_MOTOR) = SPEED_LOW;
	motor(RIGHT_MOTOR) = SPEED_LOW;

	int min = 100;
	int max = 0;
	int i = 0;
	while(i < 50)
	{
		sleep(100);
		int reading = SensorValue[S3];
		if(reading > max)
		{
			max = reading;
		}
		if(reading < min)
		{
			min = reading;
		}
		displayBigTextLine(4, "Max = %d", max);
		displayBigTextLine(6, "Min = %d", min);
		i++;
	}
	motor(LEFT_MOTOR) = 0;
	motor(RIGHT_MOTOR) = 0;
	//sleep(1000);
	//int black = SensorValue[S3];
	thresholdRight = (max + min)/4;
}

static void moveTowardsObject()
{
	while (sonar_isObjectFound() != 0) {
		int proportional_speed = (int) (SPEED_HIGH * 2 * sonar_getProportion());
		displayBigTextLine(12, "SPEED: %d", proportional_speed);

		motor(LEFT_MOTOR) = proportional_speed;
		motor(RIGHT_MOTOR) = proportional_speed;
	}
	stopMotors();
	sleep(WAIT_FULL_SEC);
	sleep(WAIT_FULL_SEC);
	reverse();
	motor(LEFT_MOTOR) = SPEED_HIGH;
	motor(RIGHT_MOTOR) = -SPEED_HIGH;
	sleep(WAIT_FULL_SEC);
	motorStop();
	objectDetect = 0;
	lineFollow = 0;

}
