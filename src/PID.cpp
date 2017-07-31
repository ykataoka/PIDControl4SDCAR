#include "PID.h"
#include <iostream>

using namespace std;

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {

  PID::Kp = Kp;
  PID::Ki = Ki;
  PID::Kd = Kd;
  
}

void PID::UpdateError(double cte, double dt) {

  double prev_p_error = p_error;
  p_error = cte;
  i_error = i_error  + cte*dt;
  d_error = (cte - prev_p_error) / dt;
  
}

double PID::TotalError() {
  double totalerror = - (Kp * p_error + Ki * i_error + Kd * d_error);
  return totalerror;
}
