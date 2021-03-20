#include "pid.h"
#include <stdio.h>

double measured_output, control_signal;
double reference = 0.0;
double Kp = 0.0;  // Ganho Proporcional
double Ki = 0.0;  // Ganho Integral
double Kd = 0.0;  // Ganho Derivativo
int T = 1.0;      // Período de Amostragem (ms)
unsigned long last_time;
double total_error, previous_error = 0.0;
int control_signal_MAX = 100.0;
int control_signal_MIN = -100.0;

void pid_configure_constants(double Kp_, double Ki_, double Kd_){
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
}

void pid_update_reference(float reference_){
    reference = (double) reference_;
}

double pid_control(double measured_output){

    double erro = reference - measured_output;

    total_error += erro; // Acumula o erro (Termo Integral)

    if (total_error >= control_signal_MAX) 
    {
        total_error = control_signal_MAX;
    }
    else if (total_error <= control_signal_MIN) 
    {
        total_error = control_signal_MIN;
    }

    double delta_error = erro - previous_error; // Diferença entre os erros (Termo Derivativo)

    control_signal = Kp*erro + (Ki*T)*total_error + (Kd/T)*delta_error; // PID calcula sinal de controle

    if (control_signal >= control_signal_MAX) 
    {
        control_signal = control_signal_MAX;
    }
    else if (control_signal <= control_signal_MIN) 
    {
        control_signal = control_signal_MIN;
    }

    previous_error = erro;

    return control_signal;
}
