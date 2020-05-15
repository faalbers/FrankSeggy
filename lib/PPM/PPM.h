#ifndef PPM_v1_h
#define PPM_v1_h

// FRSKY D4R-II PPM settings
#define PPM_READ_PIN            10      // PPM signal input pin
#define PPM_CHANNEL_COUNT       8       // Amount of channels to read in in PPM train
#define PPM_START_TRIGGER_TIME  500     // (PPM_HIGHPULSE_MIN_TIME - 400)/2 + 400
#define PPM_STOP_TRIGGER_TIME   2000    // (PPM_HIGHPULSE_MIN_TIME - 400)/2 + 400
#define PPM_HIGHPULSE_MIN_TIME  600     // PPM_HIGHPULSE_MAX_TIME + 400
#define PPM_HIGHPULSE_MID_TIME  1100
#define PPM_HIGHPULSE_MAX_TIME  1600

class PPMCBH
{
    public:
        PPMCBH();
        unsigned long GetChannel(int);

        void PPMCB(); // interrupt callback
    
    private:
        volatile unsigned long  microsAtLastPulse = 0;
        volatile unsigned int   channel = 0;
        volatile unsigned long  *validValues = NULL;
};

class PPM
{
    public:
        PPM();
        void Start();
        float GetChannel(int, bool=false, float=10.0);

    private:
        PPMCBH *ppmCBH; // PPM callback handler
        void (*interruptCBPtr)(); // Interupt callback function pointer

        float channelValue;
};

#endif // PPM_v1_h