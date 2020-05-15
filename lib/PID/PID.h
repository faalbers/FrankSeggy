#ifndef PID_v1_h
#define PID_v1_h

class PID
{
    public:
        PID(float=0.0, float=0.0, float=0.0);
        void SetPID(float, float, float);
        void SetMaxOutput(float);
        void SetReverse(bool);
        float Compute(float, float);
        void Reset();
    
    private:
        float   pid_output, pid_error_temp;
        float   pid_i_mem, pid_last_d_error;
        float   pid_p_gain, pid_i_gain, pid_d_gain;
        float   max_pid_output;
        bool    pid_reverse;

};

#endif // PID_v1_h