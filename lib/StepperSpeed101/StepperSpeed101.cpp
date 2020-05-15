#include <Arduino.h>
#include <StepperSpeed101.h>
#include <CurieTimerOne.h>

// Create stepper speed object to pass data to interrupt
StepperSpeed101CBH StepperSpeed101CBHO;

// Wrapper for the interrupt to pass to the CurieTimerOne
static void StepperSpeed101CBWrapper()
{
  StepperSpeed101CBHO.StepperSpeed101CB();
}

// Default constructer of interrupt class
StepperSpeed101CBH::StepperSpeed101CBH()
{
  lpulseCount = 0;
  rpulseCount = 0;
  lcounter = 0;
  rcounter = 0;
}

void StepperSpeed101CBH::SetPulseCount(unsigned int LPulseCount, unsigned int RPulseCount)
{
  lpulseCount = LPulseCount;
  rpulseCount = RPulseCount;
}

// Interrupt call back to create the puls train that controlls the stepper speeds
void StepperSpeed101CBH::StepperSpeed101CB()
{
  // At a certain point the slow turning is changed to total stop
  if ( lcounter > SS_MAX_PULSECOUNT ) lcounter = 0;
  // Puls timing based on pulse time counts of SS_INTERRUPT_TIME
  if ( lcounter == 0 || lpulseCount < lcounter) {
    lcounter = lpulseCount;
    digitalWrite(SS_LSTEP_PIN, LOW);
  } else if ( lcounter == 1 )
    digitalWrite(SS_LSTEP_PIN, HIGH);
  lcounter--;

  // At a certain point the slow turning is changed to total stop
  if ( rcounter > SS_MAX_PULSECOUNT ) rcounter = 0;
  // Puls timing based on pulse time counts of SS_INTERRUPT_TIME
  if ( rcounter == 0  || lpulseCount < lcounter) {
    rcounter = rpulseCount;
    digitalWrite(SS_RSTEP_PIN, LOW);
  } else if ( rcounter == 1 )
    digitalWrite(SS_RSTEP_PIN, HIGH);
  rcounter--;
}

// Default constructor of the StepperSpeed101 Class
StepperSpeed101::StepperSpeed101()
{
  ss101CBH = &StepperSpeed101CBHO; // Object address that contains the interrupt call back
  interruptCBPtr = &StepperSpeed101CBWrapper;
  // Puls timing to speed conversion used for angle per time
  pulseconvertion = SS_ERROR_CALIBRATION * 30000000 / (SS_INTERRUPT_TIME * SS_PULSES_PER_ROTATION);
}

void StepperSpeed101::Start()
{
  // Setup all pins
  pinMode(SS_LSTEP_PIN,OUTPUT);
  pinMode(SS_RSTEP_PIN,OUTPUT);
  pinMode(SS_LDIR_PIN,OUTPUT);
  pinMode(SS_RDIR_PIN,OUTPUT);
  pinMode(SS_SLEEP_PIN, OUTPUT);
  SetDir(true, true); // Default direction is forward
  digitalWrite(SS_SLEEP_PIN,LOW); // Make sure steppers are not active yet
  CurieTimerOne.start(SS_INTERRUPT_TIME, interruptCBPtr); // Pass interrup call back to timer
}

void StepperSpeed101::SetDir(bool Ldir, bool Rdir) {
  digitalWrite(SS_LDIR_PIN,!Ldir);
  digitalWrite(SS_RDIR_PIN,Rdir);
}

// Set speed using Rotations Per Minute. Pos and Negaticve for rotation direction
void StepperSpeed101::SetRPM(float Lrpm, float Rrpm) {
  SetAPS(6.0 * Lrpm, 6.0 * Rrpm);
}

// Set speed using Rotations Per Second. Pos and Negaticve for rotation direction
void StepperSpeed101::SetRPS(float Lrps, float Rrps) {
  SetAPS(360.0 * Lrps, 360.0 * Rrps);
}

// Set speed using Angle Per Second. Pos and Negaticve for rotation direction
void StepperSpeed101::SetAPS(float LanglePerSecond, float RanglePerSecond) {
  // Limit to max speed
  LanglePerSecond = max(-SS_MAX_APS, min(SS_MAX_APS, LanglePerSecond));
  RanglePerSecond = max(-SS_MAX_APS, min(SS_MAX_APS, RanglePerSecond));
  SetDir( LanglePerSecond > 0.0, RanglePerSecond > 0.0 );
  lpulsecount = abs(pulseconvertion / (LanglePerSecond / 6.0));
  rpulsecount = abs(pulseconvertion / (RanglePerSecond / 6.0));
  ss101CBH->SetPulseCount(lpulsecount, rpulsecount);
}

// Set activation of steppers
void StepperSpeed101::SetActive( bool active )
{
  digitalWrite(SS_SLEEP_PIN, active);
}
