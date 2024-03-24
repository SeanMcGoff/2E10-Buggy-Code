#include "pid.h"

double Pid::Input;
double Pid::Output;
double Pid::SetPoint = 10;
bool Pid::enabled = false;

PID Pid::buggyPID = PID(&Pid::Input, &Pid::Output, &Pid::SetPoint, Pid::KP, Pid::KI, Pid::KD, REVERSE);