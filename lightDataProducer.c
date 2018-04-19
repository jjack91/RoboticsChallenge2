#define READING_COUNT 10
#define BUFFER_SIZE 10
#define TOLERANCE_THRESHOLD 3

typedef enum Sensor {
					 SENSOR_NONE = -1,
					 SENSOR_LEFT,
					 SENSOR_RIGHT,
					 SENSOR_BOTH};

static int leftSensorRawReadings[READING_COUNT];
static int rightSensorRawReadings[READING_COUNT];
static int leftSensorAverageDarkBuffer[BUFFER_SIZE];
static int leftSensorAverageLightBuffer[BUFFER_SIZE];
static int rightSensorAverageDarkBuffer[BUFFER_SIZE];
static int rightSensorAverageLightBuffer[BUFFER_SIZE];

static int leftSensorLightIndex_LightAve = 0; // Index for the buffer containing average light-type values for left sensor
static int leftSensorLightIndex_DarkAve = 0; // Index for the buffer containing average dark-type values for left sensor
static int leftSensorLightIndex_rawReading = 0; // Index for the buffer containing unaveraged values for left sensor
static int rightSensorLightIndex_LightAve = 0; // Index for the buffer containing average light-type values for right sensor
static int rightSensorLightIndex_DarkAve = 0; // Index for the buffer containing average dark-type values for right sensor
static int rightSensorLightIndex_rawReading = 0; // Index for the buffer containing unaveraged values for right sensor

//static int processLightData();
static int readLightSensor();
int getLightSensorData(Sensor theSensor);

/*
 * This task is responsible for handling reading in data into the buffers and then averaging
 * The results.
 */
task processLightData()
{

}

/*
 * This function will be called periodically to read raw data from both sensors.
 */
static int readLightSensor()
{

}

/*
 * This will tell the calling thread what the requested sensor is currently looking at.
 * Input: theSensor = The light sensor we want to know the status of (left, right, or both)
 * Output: the number of sensors currently looking at black
 */
int getLightSensorData(Sensor theSensor)
{

}
