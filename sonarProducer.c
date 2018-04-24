/* Private Constants */
static const int MAX_DISTANCE = 90; // in cm 
static const int SONAR_BUFFER_SIZE = 50;
static const int AVG_CHECK_SIZE = 10;

/* Private Function Prototypes */
static void incrementSonarIndex(int *index);
static int getNextDistance();

/* Public Function Prototypes */
int isObjectFound();
int getDistance();

/* Private Variables */
static int sonarDistanceBuffer[SONAR_BUFFER_SIZE];
static int sonarProducerIndex = 0;
static int sonarConsumerIndex = 0;
static int objectFound = 0;
static int currentDistance = 0;


/* Tasks to populate the distances retrieved from the sonar sensor
   into a buffer that tells the robot how close they are to an object
   if they are within about 3 feet of it. */
task populateSonarValues() {
    
  while(true) {        
    currentDistance = getNextDistance(); // get noise-filtered distance
    // when object detected
    if (currentDistance <= MAX_DISTANCE) {
      objectFound = 1;
      // ensure currentDistance is less than previous distance where possible
      if (sonarProducerIndex > 0 {
        while (currentDistance-- > sonarDistanceBuffer[sonarProducerIndex - 1]) {}
      }
      // load up the buffer
      sonarDistanceBuffer[sonarProducerIndex] = currentDistance;
      incrementSonarIndex(&sonarProducerIndex);
    }
    
    // no object detected
    if (currentDistance > MAX_DISTANCE) {
      objectFound = 0;
      // purge buffer, no object detected 
      sonarDistanceBuffer[0] = -1
      sonarProducerIndex = 0;
      sonarConsumerIndex = 0;
    }
    
    // dont try to populate buffer if we havent consumed 
    if (sonarProducerIndex == sonarConsumerIndex) {
      abortTimeSlice();
    }
  }

}

/* Increments the given index by 1. The given index will not
	 exceed BUFFER_SIZE. */
static void incrementIndex(int *index) {
	*index = ++*index % BUFFER_SIZE;
}

/* Gets the next normalized distance measurement from the
   sonar sensor. */
staic int getNextDistance() {
  int distance = 0;
  for (int i = 0; i < AVG_CHECK_SIZE; i++) {
    distance += SensorValue[sonar1];
  }
  return distance / AVG_CHECK_SIZE;
}


/* [Possibly Deprecated] */
/* Checks whether or not an object is found.
   Returns 1 if found, 0 otherwise. */ 
int isObjectFound() {
  return objectFound;
}

/* Provides the next normalized distance from the 
   distance buffer and increments the proper pointers */
int getDistance() {
  if (objectFound == 1) {
    int distance = sonarDistanceBuffer[sonarConsumerIndex];
    incrementSonarIndex(&sonarConsumerIndex);
    return distance;
  } else {
    return -1;
  }
}







