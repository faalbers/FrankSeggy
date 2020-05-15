#include <Arduino.h>
#include <AngleSensor101.h>
#include <StepperSpeed101.h>
#include <PID.h>
#include <PPM.h>

// PID update loop time in micro seconds, 4000µs = 250Hz
#define LOOP_TIME     4000

#define TARANIS_PITCH     2
#define TARANIS_TURN      3
#define TARANIS_SWITCH    4
#define PITCH_SPEED_MULT  0.01

// Variables used in PID loop
unsigned long lastMicros;
unsigned int averageCounter;
float pidOutput, aps, pitch, turn, averageAPS;
bool angleReset;

// Create control objects
AngleSensor101 seggySensor; // This object updates and reads sensor values
StepperSpeed101 seggySpeed; // This object controls the Steppers using speed values
PID seggyPID;               // This object handels the PID stabilization system
PPM seggyPPM;               // This object handles the PPM controller reading

void setup() {
  //PID settings
  seggyPID.SetPID(15.0, 1.5, 30.0); // P, I and D settings
  seggyPID.SetMaxOutput(400.0);     // Set maximupm PID output value, both positive and negative
  seggyPID.SetReverse(true);        // The PID output is reversed to the input
  
  // Start the Sensors
  seggySensor.Start();

  // Start the Stepper controller
  seggySpeed.Start();

  // Start the PPM reading system. for soe reason it has to start after I start
  // the Stepper system
  seggyPPM.Start();

  // State of the angle reset swith
  angleReset = false;
  averageAPS = 0.0;
  averageCounter = 0;
}

void loop() {
  // used to time the PID loop
  lastMicros = micros();

  // We only do the PID look when it's between minimum and maximum angles
  // Thes are defined in the AngleSensor101 library
  if ( seggySensor.IsAngleActive() ) {
    // Get Pitch
    pitch = seggyPPM.GetChannel(TARANIS_PITCH, true);
    
    // Get the pitch angle from the Pitch channel on the controller
    pitch = seggyPPM.GetChannel(TARANIS_PITCH, true, 0.0);
    
    // Get sensor angle and offset pitch angle to move angle setpoint
    pidOutput = seggyPID.Compute(seggySensor.GetAngle(), PITCH_SPEED_MULT * -pitch );

    // Set a dead band on the PID controller output for zero transition
    if(abs(pidOutput) < 2.0) pidOutput = 0.0;

    // Use PID output for stepper speed
    aps = pidOutput * 4.0;

    // Trying to fix drift when stopped by flipping switch
    if ( seggyPPM.GetChannel(TARANIS_SWITCH, true) > 10.0 ) {
      if ( averageCounter < 5 ) {
        averageAPS -= pidOutput;
        averageCounter++;
      } else {
        averageAPS /= 5.0;
        seggySensor.DriftAngle(averageAPS * 0.002);
        averageAPS = 0.0;
        averageCounter = 0;
      }
    }

    // Get turn offsets from the Turn channel on the controller
    turn = seggyPPM.GetChannel(TARANIS_TURN, true, 0.0) * 0.4;
    // Set stepper speeds in Angel Per Seconds and make sure the Steppers are active
    seggySpeed.SetAPS(aps+(turn), aps-(turn));
    seggySpeed.SetActive(true);
    
  } else {
    // We went beyoun the min/max angles of the sensor
    // We reset the PID controller turn off the steppers
    seggyPID.Reset();
    seggySpeed.SetActive(false);
  }

  // Used to start PID
  delayMicroseconds(LOOP_TIME - micros() + lastMicros);

  // Update sensors. For some reason this does not work when I set it in the
  // beginning of the loop
  seggySensor.Update(0.000001 * (micros() - lastMicros));

}