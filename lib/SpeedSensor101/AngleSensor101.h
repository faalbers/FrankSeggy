#ifndef ANGLESENSOR101_v1_h
#define ANGLESENSOR101_v1_h

#define AS101_SENSOR_RATE           400  // Sensor rate in Hz
#define AS101_GYRO_RANGE            500 // Gyro rate in degrees/sec
#define AS101_ACC_RANGE             2   // Accelerometer rate in G
#define AS101_ACC_CALIBRATION_ANGLE 3.0 // Accelerometer calibration angle
#define AS101_LOCK_ANGLE            1.0 // angle lock to Accelerometer
#define AS101_UNLOCK_ANGLE          70.0 // angle lock to Accelerometer

class AngleSensor101
{
    public:

        AngleSensor101();
        void Start();
        void Update(float); // Update sensor
        float GetAngle();
        void SetAngle(float);
        void DriftAngle(float);
        float GetAPS(); // Get Angle Per Second
        bool IsAngleActive();
    
    private:
        bool    gyroActive;
        float   angle, angle_acc;
        float   gx, gy, gz;           // gyrometer values
        float   ax, ay, az;           // accelerometer values

};

#endif // ANGLESENSOR101_v1_h
