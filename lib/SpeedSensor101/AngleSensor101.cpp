#include <AngleSensor101.h>
#include <CurieIMU.h>

AngleSensor101::AngleSensor101()
{
    gyroActive = false;
    az = 1.0; // Making sure gyroactive does not trigger in first loop
    angle = 0.0;
}

void AngleSensor101::Start()
{
  if (CurieIMU.begin()) {
    // Settings for Gyro and Accelerometer. Check defines for settings
    CurieIMU.setGyroRate(AS101_SENSOR_RATE);
    CurieIMU.setAccelerometerRate(AS101_SENSOR_RATE);
    CurieIMU.setGyroRange(AS101_GYRO_RANGE);
    CurieIMU.setAccelerometerRange(AS101_ACC_RANGE);

    // Calibrate gyro and accelerometer
    CurieIMU.autoCalibrateGyroOffset();
    CurieIMU.autoCalibrateAccelerometerOffset(X_AXIS, 0);
    CurieIMU.autoCalibrateAccelerometerOffset(Y_AXIS, 0);
    CurieIMU.autoCalibrateAccelerometerOffset(Z_AXIS, 1);
  }
}

// Update current angle based on Gyro and Accelometer
void AngleSensor101::Update(float looptime)
{
    // Read angle of Z-up based on accelerometer vector
    angle_acc = (asin(max(-1.0,min(1.0,az))) * 57.2958) + AS101_ACC_CALIBRATION_ANGLE;

    if (gyroActive) {
        // If Gyro is active between Min and Max angles, update angle with gyro based on given looptime
        angle += gx * looptime;
        angle = angle * 0.9996 + angle_acc * 0.0004;  // Correct the drift of the gyro angle with the accelerometer angle

        // If angle goes beyon Min or Max of UNLOCK angles, deactivate angle updating by gyro
        if (angle < -AS101_UNLOCK_ANGLE || angle > AS101_UNLOCK_ANGLE) {
            gyroActive = false;
            angle = 0;
        }
    } else {
        // Make sure we start the gyro when angle is between Min and Max values of LOCK angles
        // Also ake sure we only start when the thing is not rotating too fast
        if (abs(gx) < 4.0) {
            if ( angle_acc > -AS101_LOCK_ANGLE && angle_acc < AS101_LOCK_ANGLE ) {
                gyroActive = true;
                angle = angle_acc;
            }
        }
    }
    // Read values for next update
    CurieIMU.readAccelerometerScaled(ax, ay, az);
    CurieIMU.readGyroScaled(gx, gy, gz);
}

float AngleSensor101::GetAngle()
{
    return angle;
}

void AngleSensor101::SetAngle( float Angle )
{
    angle = Angle;
}

void AngleSensor101::DriftAngle( float Angle )
{
    angle += Angle;
}

// Get speed in Angle Per Second
float AngleSensor101::GetAPS()
{
    return gx;
}

// Check if ange update is active
bool AngleSensor101::IsAngleActive()
{
    return gyroActive;
}
