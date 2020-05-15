#ifndef STEPPERSPEED101_v1_h
#define STEPPERSPEED101_v1_h

#define SS_INTERRUPT_TIME       20    // Stepper Interrupt time in micro seconds
#define SS_PULSES_PER_ROTATION  3200  // Amount of pulses needed per rotation
#define SS_MAX_PULSECOUNT       1600  // Maximum puldecounts higher counts will stop stepper
#define SS_MAX_APS              1500  // Maximum angles per second
#define SS_ERROR_CALIBRATION    1.1   // Timing error callibration
#define SS_LSTEP_PIN            2     // Stepper left step pin
#define SS_LDIR_PIN             3     // Stepper left direction pin
#define SS_RSTEP_PIN            4     // Stepper right step pin
#define SS_RDIR_PIN             8     // Stepper right direction pin
#define SS_SLEEP_PIN            7     // Sleep pin

class StepperSpeed101CBH
{
  public:
    StepperSpeed101CBH();
    void SetPins(int, int);
    void SetPulseCount(unsigned int, unsigned int);

    void StepperSpeed101CB(); // interrupt callback

  private:
    volatile unsigned int lpulseCount, rpulseCount;
    volatile unsigned int lcounter, rcounter;

};

class StepperSpeed101
{
  public:
    StepperSpeed101();
    void SetDir(bool, bool);        // Set direction on both steppers
    void SetRPM(float, float);       // Set Rotations Per Minute on both steppers
    void SetRPS(float, float);       // Set Rotations Per Second on both steppers
    void SetAPS(float, float);       // Set Angle Per Second
    void SetLeftAPS(float);   // Set Angle Per Second on left steppers
    void SetRightAPS(float);  // Set Angle Per Second on right steppers
    void SetActive(bool);     // Turn activation of Steppers on and off

    void Start();             // Start the Stepper controllers


  private:
    float lpulsecount, rpulsecount;
    unsigned int pulseconvertion;
    StepperSpeed101CBH *ss101CBH; // StepperSpeed101 callback handler
    void (*interruptCBPtr)();     // Interupt callback function pointer
};

#endif // STEPPERSPEED101_v1_h