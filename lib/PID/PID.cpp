#include <PID.h>

PID::PID(float p_gain, float i_gain, float d_gain)
{
    pid_output = 0.0;
    max_pid_output = 0.0;
    pid_reverse = false;
    SetPID(p_gain, i_gain, d_gain);
}

void PID::SetPID(float p_gain, float i_gain, float d_gain)
{
    pid_p_gain = p_gain;
    pid_i_gain = i_gain;
    pid_d_gain = d_gain;
}

void PID::SetMaxOutput(float max_output)
{
    max_pid_output = max_output;
}

void PID::SetReverse(bool reverse)
{
    pid_reverse = reverse;
}

float PID::Compute(float pid_input, float pid_setpoint)
{
    pid_error_temp = pid_input - pid_setpoint;
    
    if(pid_output > 10.0 || pid_output < -10.0) pid_error_temp += pid_output * 0.015 ;

    pid_i_mem += pid_i_gain * pid_error_temp;
    if(pid_i_mem > max_pid_output) pid_i_mem = max_pid_output;
    else if(pid_i_mem < -max_pid_output)pid_i_mem = -max_pid_output;

    pid_output = pid_p_gain * pid_error_temp + pid_i_mem + pid_d_gain * (pid_error_temp - pid_last_d_error);

    if(pid_output > max_pid_output) pid_output = max_pid_output;
    else if(pid_output < -max_pid_output) pid_output = -max_pid_output;

    pid_last_d_error = pid_error_temp;

    if (pid_reverse) return -pid_output;
    else return pid_output;
}

void PID::Reset()
{
    pid_output = 0.0;
    pid_i_mem = 0.0;
}