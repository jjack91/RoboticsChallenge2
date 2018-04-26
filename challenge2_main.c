#pragma config(Sensor, S1,     sonar_sensor,   sensorEV3_Ultrasonic)
#pragma config(Sensor, S3,     right_light_sensor, sensorEV3_Color)
#pragma config(Sensor, S4,     left_light_sensor, sensorLightActive)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**********Includes**********/
#include "timeProducer.c"
#include "speedProducer.c"
#include "lightDataProducer.c"
#include "sonarDataProducer.c"

/**********Defined Variables**********/
#define LEFT_SENSOR S4
#define RIGHT_SENSOR S3
#define SONAR_SENSOR S1
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

/*typedef enum Sensor {
					 SENSOR_NONE = -1,
					 SENSOR_LEFT,
					 SENSOR_RIGHT,
					 SENSOR_BOTH};
*/
typedef enum Speed {
				  SPEED_LOW = -15,
					SPEED_HIGH = -40,
					SPEED_REVERSE = 25,
					SPEED_TURN_FORWARD = -50,
					SPEED_TURN_BACKWARD = 50};

/**********Private Constants**********/
static const int MAX_ROLL = 95;
static const int INTERRUPT_CHECK_MS = 100;

/**********Global Variables***********/
static int COLOR_BLACK_L;
static int COLOR_WHITE_L;
static int COLOR_BLACK_R;
static int COLOR_WHITE_R;

/**********Private Function Prototypes**********/
static void walk();
static Direction chooseDirection();
static void chooseTurnDirection();
static void traverse(Direction dir);
static void veer(Direction dir, int speed);
static void turn(Turn turn);
static void reverse();
static void stopMotors();
static int sensorBump();
static int sensorDetect();
static void calibrate();


/* Main starting point of the program. */
task main() {

	calibrate();

	// Starts the population of the time buffer in its own thread.
	startTask(populateTimes);
	startTask(populateSpeeds);
	startTask(populateSonarValues)
	//startTask(processLightData);


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
	veer(dir, speed);

	// Duration of traversing with sensor checks.
	for(int i = 0; i < time; i += INTERRUPT_CHECK_MS) {
		if (sensorDetect() == 1) {
			return;
		}
		setLEDColor(ledGreen);
		sleep(INTERRUPT_CHECK_MS);
		displayBigTextLine(10, "");
	}
}

/* Sets the wheel speeds to cause a veer (or go straight). */
static void veer(Direction dir, int speed) {
	switch (dir) {
		case (DIR_LEFT) :
			motor(LEFT_MOTOR) = speed;
			motor(RIGHT_MOTOR) = SPEED_HIGH;
			break;
		case (DIR_RIGHT) :
			motor(LEFT_MOTOR) = SPEED_HIGH;
			motor(RIGHT_MOTOR) = speed;
			break;
		case (DIR_STRAIGHT) :
			motor(LEFT_MOTOR) = SPEED_HIGH;
			motor(RIGHT_MOTOR) = SPEED_HIGH;
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
			sleep(WAIT_HALF_SEC);
			break;
		case (TURN_RIGHT) :
			motor(LEFT_MOTOR) = SPEED_TURN_FORWARD;
			motor(RIGHT_MOTOR) = SPEED_TURN_BACKWARD;
			sleep(WAIT_HALF_SEC);
			break;
		case (TURN_AROUND) :
			motor(LEFT_MOTOR) = SPEED_TURN_BACKWARD;
			motor(RIGHT_MOTOR) = SPEED_TURN_FORWARD;
			sleep(WAIT_FULL_SEC);
			break;
		default : // Invalid turn.
			return;
	}
}

/* Causes the robot to reverse. */
static void reverse() {
	motor(LEFT_MOTOR) = SPEED_REVERSE;
	motor(RIGHT_MOTOR) = SPEED_REVERSE;
	sleep(WAIT_FULL_SEC);
}

/* Stops all movement of the robot. */
static void stopMotors() {
	motor(LEFT_MOTOR) = 0;
	motor(RIGHT_MOTOR) = 0;
	sleep(WAIT_TWO_SEC);
}

/* Interupt for checking if a sensor was bumped. */
/* Deprecated for Challenge 2. Keep for reference.*/
static int sensorBump() {

	Sensor sensor = SENSOR_NONE;

	// Check if the left sensor was bumped.
	if (SensorValue(LEFT_SENSOR) ==  1) {
		sleep(10);
		// Checks if the right sensor was bumped after 10 ms which
		// counts as both sensors being bumped.
		if (SensorValue(RIGHT_SENSOR) ==  1) {
			sensor = SENSOR_BOTH;
		} else { // Just the left sensor was bumped.
			sensor = SENSOR_LEFT;
		}
	// Check if the right sensor was bumped.
	} else if (SensorValue(RIGHT_SENSOR) ==  1) {
		sleep(10);
		// Checks if the left sensor was bumped after 10 ms which
		// counts as both sensors being bumped.
		if (SensorValue(LEFT_SENSOR) ==  1) {
			sensor = SENSOR_BOTH;
		} else { // Just the right sensor was bumped.
			sensor = SENSOR_RIGHT;
		}
	}

	// Finds out which sensor(s) was/were bumped.
	switch (sensor) {
		case (SENSOR_BOTH) :
			playSound(soundBeepBeep);
			reverse();
			stopMotors();
			chooseTurnDirection();
			return 1;
		case (SENSOR_LEFT) :
			reverse();
			turn(TURN_RIGHT);
			return 1;
		case (SENSOR_RIGHT) :
			reverse();
			turn(TURN_LEFT);
			return 1;
		default : // SENSOR_NONE
			return 0;
	}
}

static int sensorDetect() {

	Sensor sensor = SENSOR_NONE;
	int leftSensor = getLightSensorData(SENSOR_LEFT);
	int rightSensor = getLightSensorData(SENSOR_RIGHT)

	// Check if the left sensor was bumped.
	if (leftSensor && rightSensor) {
		sensor = SENSOR_BOTH;
	// Check if the right sensor was bumped.
	} else if (rightSensor) {
		sensor = SENSOR_RIGHT
	} else if (leftSensor) {
		sensor = SENSOR_LEFT
	}

	// Finds out which sensor(s) was/were bumped.
	switch (sensor) {
		case (SENSOR_BOTH) :
		//	playSound(soundBeepBeep);
		//	reverse();
		//	stopMotors();
		//	chooseTurnDirection();
			setLEDColor(ledRedFlash);
			veer(DIR_STRAIGHT, SPEED_LOW);
			//sleep(500);
			return 1;
		case (SENSOR_LEFT) :
		//	reverse();
		//	turn(TURN_RIGHT);
			setLEDColor(ledOrangePulse);
			veer(DIR_LEFT, 0);
			//turn(TURN_LEFT);
			//sleep(500);
			return 1;
		case (SENSOR_RIGHT) :
		//	reverse();
		//	turn(TURN_LEFT);
			setLEDColor(ledRed);
			veer(DIR_RIGHT, 0);
			//turn(TURN_RIGHT);
			//sleep(500);
			return 1;
		default : // SENSOR_NONE
			return 0;
	}
}

static void calibrate()
{
	displayBigTextLine(4, "Place light");
	displayBigTextLine(6, "sensors over");
	displayBigTextLine(8, "BLACK");

	sleep(1000);
	displayBigTextLine(1, "5");
	sleep(1000);
	displayBigTextLine(1, "4");
	sleep(1000);
	displayBigTextLine(1, "3");
	sleep(1000);
	displayBigTextLine(1, "2");
	sleep(1000);
	displayBigTextLine(1, "1");
	sleep(1000);
	displayBigTextLine(1, "0 ... Reading...");

	primeBuffer();

	displayBigTextLine(4, "");
	displayBigTextLine(6, "");
	displayBigTextLine(8, "");

	sleep(300);
	startTask(processLightData);
	displayBigTextLine(1, "");
	displayBigTextLine(4, "");
	displayBigTextLine(6, "");
	displayBigTextLine(8, "");

	motor(LEFT_MOTOR) = SPEED_LOW;
	motor(RIGHT_MOTOR) = SPEED_LOW;

	while(getLightSensorData(SENSOR_BOTH) > 0)
	{
		//spin-wait
		sleep(100);
	}
	displayBigTextLine(1, "Running");
}
