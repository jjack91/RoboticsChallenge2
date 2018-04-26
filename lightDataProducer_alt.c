typedef enum Sensor {
					 SENSOR_NONE = -1,
					 SENSOR_LEFT = 0,
					 SENSOR_RIGHT = 1,
					 SENSOR_BOTH = 2};

#define LEFT_MOTOR motorA
#define RIGHT_MOTOR motorB




static int leftLooking;
static int rightLooking;

static void readLightSensor();


task processLightData()
{

	leftLooking = SensorValue[S4];
	rightLooking = SensorValue[S3];


}
