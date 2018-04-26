#define READING_COUNT 10
#define BUFFER_SIZE 10
#define TOLERANCE_THRESHOLD 5
#define true 1
#define false 0


typedef enum Sensor {
					 SENSOR_NONE = -1,
					 SENSOR_LEFT = 0,
					 SENSOR_RIGHT = 1,
					 SENSOR_BOTH = 2};

static void readLightSensor();
int getLightSensorData(Sensor theSensor);
static void primeBuffer();

static int leftSensorRawReadings[READING_COUNT];
static int rightSensorRawReadings[READING_COUNT];
static int leftSensorAverageDarkBuffer[BUFFER_SIZE];
static int leftSensorAverageLightBuffer[BUFFER_SIZE];
static int rightSensorAverageDarkBuffer[BUFFER_SIZE];
static int rightSensorAverageLightBuffer[BUFFER_SIZE];

static int isLeftDark = 1;
static int isRightDark = 1;

static int leftSensorLightIndex_LightAve = 0; // Index for the buffer containing average light-type values for left sensor
static int leftSensorLightIndex_DarkAve = 0; // Index for the buffer containing average dark-type values for left sensor
static int leftSensorLightIndex_rawReading = 0; // Index for the buffer containing unaveraged values for left sensor
static int rightSensorLightIndex_LightAve = 0; // Index for the buffer containing average light-type values for right sensor
static int rightSensorLightIndex_DarkAve = 0; // Index for the buffer containing average dark-type values for right sensor
static int rightSensorLightIndex_rawReading = 0; // Index for the buffer containing unaveraged values for right sensor

//static int processLightData();


/*
 * This task is responsible for handling reading in data into the buffers and then averaging
 * The results.
 */
task processLightData()
{
	for(;;)
	{
		displayBigTextLine(4, "Reading...");
		readLightSensor();
		if(leftSensorLightIndex_rawReading == (READING_COUNT - 1))
		{
			int leftAverage = 0;
			int i;
			for(i = 0; i < READING_COUNT - 1; i++)
			{
				leftAverage += leftSensorRawReadings[i];
			}
			leftAverage = leftAverage / READING_COUNT;
			displayBigTextLine(8, "Looking at %d", leftAverage);
			displayBigTextLine(6, "Last Dark Avg: %d", leftSensorAverageDarkBuffer[leftSensorLightIndex_DarkAve]);
			displayBigTextLine(12, "Status: L=%d R=%d", isLeftDark, isRightDark);
      // if average value from raw readings is within range : 0 to current avg value from buffer + TOLERANCE 
      // store the average in the dark buffer
			if(leftAverage <= leftSensorAverageDarkBuffer[leftSensorLightIndex_DarkAve]+TOLERANCE_THRESHOLD
				/*&& leftSensorAverageDarkBuffer[leftSensorLightIndex_DarkAve] >= leftAverage-TOLERANCE_THRESHOLD*/)
			{

				leftSensorAverageDarkBuffer[(leftSensorLightIndex_DarkAve+1) % BUFFER_SIZE] = leftAverage;
				leftSensorLightIndex_DarkAve = ++leftSensorLightIndex_DarkAve % BUFFER_SIZE;
				isLeftDark = 1; 
			}
			else // otherwise store it in the light avg buffer
			{
				leftSensorAverageLightBuffer[leftSensorLightIndex_LightAve] = leftAverage;
				leftSensorLightIndex_LightAve = ++leftSensorLightIndex_LightAve % BUFFER_SIZE;
				isLeftDark = 0;
			}
			displayBigTextLine(8, "");
			displayBigTextLine(6, "");

			leftSensorLightIndex_rawReading = 0;
		}
		if(rightSensorLightIndex_rawReading == (READING_COUNT - 1))
		{
			int rightAverage = 0;
			int i;
			for(i = 0; i < READING_COUNT - 1; i++)
			{
				rightAverage += rightSensorRawReadings[i];
			}
			rightAverage = rightAverage / READING_COUNT;

			if(/*rightSensorAverageDarkBuffer[rightSensorLightIndex_DarkAve] >= rightAverage-TOLERANCE_THRESHOLD
				&&*/  rightAverage <= rightSensorAverageDarkBuffer[rightSensorLightIndex_DarkAve]+TOLERANCE_THRESHOLD)
			{
				rightSensorAverageDarkBuffer[(rightSensorLightIndex_DarkAve+1) % BUFFER_SIZE] = rightAverage;
				rightSensorLightIndex_DarkAve = ++rightSensorLightIndex_DarkAve % BUFFER_SIZE;
				isRightDark = 1;
			}
			else
			{
				rightSensorAverageLightBuffer[rightSensorLightIndex_LightAve] = rightAverage;
				rightSensorLightIndex_LightAve = ++rightSensorLightIndex_LightAve % BUFFER_SIZE;
				isRightDark = 0;
			}

			rightSensorLightIndex_rawReading = 0;
		}
		else
		{
			displayBigTextLine(4, "");
			abortTimeslice();
		}


	}
}

/*
 * This function will be called periodically to read raw data from both sensors.
 */
static void readLightSensor()
{
	leftSensorRawReadings[leftSensorLightIndex_rawReading] = SensorValue[S4];
	rightSensorRawReadings[rightSensorLightIndex_rawReading] = SensorValue[S3];
	//displayBigTextLine(10, "Index: %d", leftSensorLightIndex_rawReading);
	leftSensorLightIndex_rawReading = ++leftSensorLightIndex_rawReading % READING_COUNT;
	rightSensorLightIndex_rawReading = ++rightSensorLightIndex_rawReading % READING_COUNT;
}

static void primeBuffer()
{
	leftSensorAverageDarkBuffer[0] = SensorValue[S4];
	rightSensorAverageDarkBuffer[0] = SensorValue[S3];
}

/*
 * This will tell the calling thread what the requested sensor is currently looking at.
 * Input: theSensor = The light sensor we want to know the status of (left, right, or both)
 * Output: the number of sensors currently looking at black
 */
int getLightSensorData(Sensor theSensor)
{
	if(theSensor == SENSOR_LEFT)
	{
		//displayBigTextLine(10, "Left is: %d", isLeftDark);
		return isLeftDark;
	}
	else if(theSensor == SENSOR_RIGHT)
	{
		//displayBigTextLine(10, "Right is: %d", isRightDark);
		return isRightDark;
	}
	else if(theSensor == SENSOR_BOTH)
	{
		displayBigTextLine(10, "Both is: %d", (isRightDark + isLeftDark));
		return isLeftDark + isRightDark;
	}
	else
	{
		displayBigTextLine(10, "Sensor return Error");
		return 0;
	}
}
