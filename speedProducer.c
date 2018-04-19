/**********Private Constants**********/
static const int SPEED_BUFFER_SIZE = 10;
static const int SPEED_INTERVAL = 25;
static const int SPEED_MIN = 45 ;

/**********Private Function Prototypes**********/
static int getRandomSpeed();
static void incrementSpeedIndex(int *index);

/**********Function Prototypes**********/
int getSpeed();

/**********Private Variables**********/
static int speedBuffer[SPEED_BUFFER_SIZE];
static int speedProducerIndex = 0;
static int speedConsumerIndex = 0;


/* Task to populate times into a buffer that tell the robot
   how long to move in a certain direction. */
task populateSpeeds() {

	// Prime the pump by inserting the first value.
	speedBuffer[speedProducerIndex] = getRandomSpeed();
	incrementSpeedIndex(&speedProducerIndex);

	// Populates the buffer full of time values.
	while(true) {
		if (speedProducerIndex != speedConsumerIndex) {
			speedBuffer[speedProducerIndex] = getRandomSpeed();
			incrementSpeedIndex(&speedProducerIndex);
		} else {
			// Time slice is aborted to free up CPU.
			abortTimeslice();
		}
	}
}

/* Retrieves a speed from the speedBuffer. This function increments
	 the consumer index after retrieval. */
int getSpeed() {
	int speed = speedBuffer[speedConsumerIndex];
	incrementSpeedIndex(&speedConsumerIndex);
	return speed;
}

/* Increments the given index by 1. The given index will not
	 exceed BUFFER_SIZE. */
static void incrementSpeedIndex(int *index) {
	*index = ++*index % SPEED_BUFFER_SIZE;
}

/* Returns a random number. The range is TIME_MAX + 1.
	 This function eliminates a return of 0 while
	 keeping the returned random consistent with
	 TIME_MAX. */
static int getRandomSpeed() {
	int speed = random(SPEED_INTERVAL - 1) + SPEED_MIN + 1;
	speed = speed * -1;
	return speed;
}
