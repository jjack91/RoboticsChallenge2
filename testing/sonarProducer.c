#include <math.h>

/* Constants */
static const int MAX_DISTANCE = 90; // in cm 
static const int SONAR_BUFFER_SIZE = 50;

/* */
static void incrementSonarIndex(int *index);

/* */
int isObjectFound();
int getDistance();

/* Private Variables */
static int sonarDistanceBuffer[SONAR_BUFFER_SIZE];
static int sonarProducerIndex = 0;
static int sonarConsumerIndex = 0;
static int objectFound = 0;
static int currentDistance = 0;


task populateSonarValues() {
    
  while(true) {        
    currentDistance = SensorValue[sonar1];
    // when object detected
    if (currentDistance <= MAX_DISTANCE) {
      objectFound = 1;
      // ensure currentDistance is less than previous distance where possible
      if (sonarProducerIndex > 0 {
        while (currentDistance-- < sonarDistanceBuffer[sonarProducerIndex - 1]) {}
      }
      // load up the buffer
      sonarDistanceBuffer[sonarProducerIndex] = currentDistance;
      incrementSonarIndex(&sonarProducerIndex);
    }
    
    // no object detected
    if (currentDistance > MAX_DISTANCE) {
      objectFound = 0;
      // purge buffer, no object detected   
      sonarProducerIndex = 0;
      sonarConsumerIndex = 0;
    }
    
    // dont try to populate buffer if we havent consumed 
    if (sonarProducerIndex == sonarConsumerIndex) {
      abortTimeSlice();
    }
  }

}

static void incrementIndex(int *index) {
	*index = ++*index % BUFFER_SIZE;
}

int isObjectFound() {
  return objectFound;
}

int getDistance() {
  int distance = sonarDistanceBuffer[sonarConsumerIndex];
  incrementSonarIndex(&sonarConsumerIndex);
  return ;
}







