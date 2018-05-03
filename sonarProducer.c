#define SONAR_SENSOR_VAL S1
/* Private Constants */
static const int MAX_DISTANCE = 110; // in cm
static const int MIN_DISTANCE = 14; // in cm
static const int AVG_CHECK_SIZE = 10; // sample size to filter out noise

/* Private Function Prototypes */
static int sonar_getNextDistance(); // get filtered reading from sensors

/* Public Function Prototypes */
int sonar_isObjectFound(); // if object is within range [min, max]
int sonar_getDistance(); // current saved distance from filtered readings
float sonar_getProportion(); // gets the proportion of (current distance - MIN_DIST) / (MAX_DIST - MIN DIST)

/* Private Variables */
static int objectFound = 0;
static int currentDistance = 0;


/* Tasks to populate the distances retrieved from the sonar sensor
   into a buffer that tells the robot how close they are to an object
   if they are within about 3 feet of it. */
task populateSonarValues()
{
  while(true)
  {
    displayBigTextLine(4, "SONAR SENSOR:");
    currentDistance = sonar_getNextDistance(); // get noise-filtered distance
    displayBigTextLine(6, "DISTANCE: %d", currentDistance);
    // when object detected
    if (currentDistance <= MAX_DISTANCE && currentDistance >= MIN_DISTANCE)
    {
      objectFound = 1;
      displayBigTextLine(8, "OBJECT: FOUND");
    }
    else
    {
      objectFound = 0;
      displayBigTextLine(8, "OBJECT: NOT FOUND");
    }
}
}

/* Gets the next normalized distance measurement from the
   sonar sensor. */
static int sonar_getNextDistance() {
  int distance = 0;
  for (int i = 0; i < AVG_CHECK_SIZE; i++) {
    distance += SensorValue[SONAR_SENSOR_VAL];
  }
  return distance / AVG_CHECK_SIZE;
}

/* Checks whether or not an object is found.
   Returns 1 if found, 0 otherwise. */
int sonar_isObjectFound() {
  return objectFound;
}

/* Provides the next normalized distance from the
   distance buffer and increments the proper pointers */
int sonar_getDistance() {
  if (objectFound == 1)
  {
    return currentDistance;
  }
  else
  {
    return -1;
  }
}

/* Provides the current % error from the min distance mark that the 
 current distance is via the formula:
 (CurrentDist - Min) / (Max - Min)
 so at most you can have (Max - Min) / (Max - Min) = 1
 and at least you can have (Min - Min) / (Max - Min) = 0*/
float sonar_getProportion() {
	return currentDistance * 1.0 / MAX_DISTANCE;
}
