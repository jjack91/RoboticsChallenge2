/**********Private Constants**********/
static const int BUFFER_SIZE = 10;
static const int TIME_INTERVAL_MS = 600;
static const int TIME_MIN_MS = 500;

/**********Private Function Prototypes**********/
static int getRandom();
static void incrementIndex(int *index);

/**********Function Prototypes**********/
int getTime();

/**********Private Variables**********/
static int timeBuffer[BUFFER_SIZE];
static int producerIndex = 0;
static int consumerIndex = 0;


/* Task to populate times into a buffer that tell the robot
   how long to move in a certain direction. */
task populateTimes() {

	// Prime the pump by inserting the first value.
	timeBuffer[producerIndex] = getRandom();
	incrementIndex(&producerIndex);

	// Populates the buffer full of time values.
	while(true) {
		if (producerIndex != consumerIndex) {
			timeBuffer[producerIndex] = getRandom();
			incrementIndex(&producerIndex);
		} else {
			// Time slice is aborted to free up CPU.
			abortTimeslice();
		}
	}
}

/* Retrieves a time from the timeBuffer. This function increments theTitheTimer
	 the consumer index after retrieval. */
int getTime() {
	int time = timeBuffer[consumerIndex];
	incrementIndex(&consumerIndex);
	return time;
}

/* Increments the given index by 1. The given index will not
	 exceed BUFFER_SIZE. */
static void incrementIndex(int *index) {
	*index = ++*index % BUFFER_SIZE;
}

/* Returns a random number. The range is TIME_MAX + 1.
	 This function eliminates a return of 0 while
	 keeping the returned random consistent with
	 TIME_MAX. */
static int getRandom() {
	return random(TIME_INTERVAL_MS - 1) + TIME_MIN_MS + 1;
}
