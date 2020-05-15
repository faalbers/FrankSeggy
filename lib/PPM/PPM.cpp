#include <Arduino.h>
#include <PPM.h>

// Create PPM object to pass data to interrupt
PPMCBH PPMCBHO;

// Wrapper for the interrupt to pass to the CurieTimerOne
static void PPMCBWrapper()
{
  PPMCBHO.PPMCB();
}

// Default constructer of interrupt class
PPMCBH::PPMCBH()
{
    validValues = new unsigned long[PPM_CHANNEL_COUNT];
    for ( int i=0; i < PPM_CHANNEL_COUNT; i++ ) validValues[i] = 0;
}

unsigned long PPMCBH::GetChannel( int Channel)
{
    return validValues[Channel];
}

// Interrupt call back to read puls train coming from PPM device
void PPMCBH::PPMCB()
{
    // Calculate time passed between incoming pulse changes
    unsigned long previousMicros = microsAtLastPulse;
    microsAtLastPulse = micros();
    unsigned long time = microsAtLastPulse - previousMicros;

    if ( time > PPM_STOP_TRIGGER_TIME ) {
        // If we had a long pause of the pulse train, restart  channels reading
        channel = 0;
    } else if ( time > PPM_START_TRIGGER_TIME ) {
        // Else , if it's not a smaller the trigger time pulse, read pulse length as
        // value of current channel
        if ( channel < PPM_CHANNEL_COUNT ) validValues[channel] = time;
        channel++;
    }
}

// Default constructor of the PPM Class
PPM::PPM()
{
  ppmCBH = &PPMCBHO; // Object address that contains the interrupt call back
  interruptCBPtr = &PPMCBWrapper;
}

void PPM::Start()
{
  // Setup pulse read pin
  pinMode(PPM_READ_PIN, INPUT_PULLUP);
   // Pass interrup call back to pin trigger on every change, up or down
  attachInterrupt(digitalPinToInterrupt(PPM_READ_PIN), interruptCBPtr, CHANGE);
}

// Get Channel value
// If useMid is true then range from -500 to +500
// If useMid is false then range from 0 to 1000
// Deadband adds a range from - deadband to + deadband if iseMid is true
float PPM::GetChannel( int channel, bool useMid, float deadBand)
{
  if ( ppmCBH->GetChannel(channel) == 0 ) return 0.0;
  channelValue = float(ppmCBH->GetChannel(channel)) - 600.0;
  if ( useMid ) channelValue -= 500.0;
  if ( channelValue < deadBand && channelValue > -deadBand ) channelValue = 0.0;
  return channelValue;
}
