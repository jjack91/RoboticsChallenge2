

static int leftIndex = 0;
static int rightIndex = 0;
static int leftReadings[10];
static int rightReadings[10];
static int leftAve = 0;
static int rightAve = 0;
static int firstRun = 1;

static int readLightSensor();


task processLightData()
{

	for(;;)
	{
		leftReadings[leftIndex] = SensorValue[S4];
		rightReadings[rightIndex] = SensorValue[S3];

		leftIndex = ++leftIndex % 10;
		rightIndex = ++rightIndex % 10;



		if(leftIndex > 8)
		{
			int i;
			leftAve = 0;
			for(i = 0; i < 10; i++)
			{
				leftAve += leftReadings[i];
			}
			leftAve = leftAve/10;




			rightAve = 0;
			for(i = 0; i < 10; i++)
			{
				rightAve += rightReadings[i];
			}
			rightAve = rightAve/10;
		}
	}


}

static int readLightSensor(int theSensor)
{
	if(theSensor == 1)
	{
		return leftAve;
	}
	else if(theSensor == 2)
	{
		return rightAve;
	}
	else
	{
		return 100;
	}
}
