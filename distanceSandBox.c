#pragma config(Sensor, S1,     sonar4,         sensorEV3_Ultrasonic)
#pragma config(Sensor, S4,     ,               sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          motorRight,    tmotorEV3_Large, PIDControl, reversed, driveRight, encoder)
#pragma config(Motor,  motorB,          motorLeft,     tmotorEV3_Large, PIDControl, reversed, driveLeft, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	// Distance to maintain to the target (30 cm)
	const int distanceToMaintain = 91;
	int distancesSum = 0;

	int currentDistance = 0;

	motor[motorLeft] = 30;
	motor[motorRight] = 30;
	int size = 0;
	while(true)
	{

		// Read the sensor
		currentDistance = SensorValue[sonar4];
		displayCenteredBigTextLine(4, "Dist: %3d cm", currentDistance);


			if (size < 100){

				distancesSum += currentDistance;
				size++;

			}else {

				int mean = distancesSum/size;
				displayCenteredBigTextLine(4, "mean: %d", mean);
				distancesSum = 0;
				size = 0;

					if (mean <= 90 && mean >= 10)
					{
						motor[motorLeft] = mean;
						motor[motorRight] = mean;

					} else if (mean <10) {

						motor[motorLeft] = 0;
						motor[motorRight] = 0;


					} else {

						motor[motorLeft] = 30;
						motor[motorRight] = 30;

					}
		}

		sleep(10);

	}
}
