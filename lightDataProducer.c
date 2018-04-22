#define READING_COUNT 10
#define BUFFER_SIZE 10
#define TOLERANCE_THRESHOLD 3
#define true 1
#define false 0


typedef enum Sensor {
					 SENSOR_NONE = -1,
					 SENSOR_LEFT,
					 SENSOR_RIGHT,
					 SENSOR_BOTH};

static void readLightSensor();
int getLightSensorData(Sensor theSensor);

static int leftSensorRawReadings[READING_COUNT];
static int rightSensorRawReadings[READING_COUNT];
static int leftSensorAverageDarkBuffer[BUFFER_SIZE];
static int leftSensorAverageLightBuffer[BUFFER_SIZE];
static int rightSensorAverageDarkBuffer[BUFFER_SIZE];
static int rightSensorAverageLightBuffer[BUFFER_SIZE];

static int isLeftDark = false;
static int isRightDark = false;

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
		readLightSensor();
		if(leftSensorLightIndex_rawReading == (READING_COUNT - 1)
		{
			int leftAverage = 0;
			int i;
			for(i = 0; i < READING_COUNT - 1; i++)
			{
				leftAverage += leftSensorRawReadings[i];
			}
			leftAverage = leftAverage / READING_COUNT;

			if(leftAverage-TOLERANCE_THRESHOLD >= leftSensorAverageDarkBuffer[leftSensorLightIndex_DarkAve-1]
				&& leftAverage+TOLERANCE_THRESHOLD <= leftSensorAverageDarkBuffer[leftSensorLightIndex_DarkAve-1])
			{
				leftSensorAverageDarkBuffer[leftSensorLightIndex_DarkAve] = leftAverage;
				leftSensorLightIndex_DarkAve = ++leftSensorLightIndex_DarkAve % BUFFER_SIZE;
				isLeftDark = true;
			}
			else
			{
				leftSensorAverageLightBuffer[leftSensorLightIndex_LightAve] = leftAverage;
				leftSensorLightIndex_LightAve = ++leftSensorLightIndex_LightAve % BUFFER_SIZE;
				isLeftDark = false;
			}
			
			leftSensorLightIndex_rawReading = 0;
		}
		if(rightSensorLightIndex_rawReading == (READING_COUNT - 1)
		{
			int rightAverage = 0;
			int i;
			for(i = 0; i < READING_COUNT - 1; i++)
			{
				rightAverage += rightSensorRawReadings[i];
			}
			rightAverage = rightAverage / READING_COUNT;

			if(rightAverage-TOLERANCE_THRESHOLD >= rightSensorAverageDarkBuffer[rightSensorLightIndex_DarkAve-1]
				&& rightAverage+TOLERANCE_THRESHOLD <= rightSensorAverageDarkBuffer[rightSensorLightIndex_DarkAve-1])
			{
				rightSensorAverageDarkBuffer[rightSensorLightIndex_DarkAve] = rightAverage;
				rightSensorLightIndex_DarkAve = ++rightSensorLightIndex_DarkAve % BUFFER_SIZE;
				isRightDark = true;
			}
			else
			{
				rightSensorAverageLightBuffer[rightSensorLightIndex_LightAve] = rightAverage;
				rightSensorLightIndex_LightAve = ++rightSensorLightIndex_LightAve % BUFFER_SIZE;
				isRightDark = false;
			}
			
			rightSensorLightIndex_rawReading = 0;
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

	leftSensorLightIndex_rawReading++;
	rightSensorLightIndex_rawReading++;
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
		return isLeftDark;
	}
	else if(theSensor == SENSOR_RIGHT)
	{
		return isRightDark;
	}
	else if(theSensor == SENSOR_BOTH)
	{
		return isLeftDark + isRightDark;
	}
}
