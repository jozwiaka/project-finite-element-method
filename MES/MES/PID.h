#pragma once
#include "Data.h"
class PID
{
private:
public:
	double kp, ki, kd;
	double Smin, Smax;
	double sum, inPrev;
	double out;
	double Tp;

	PID(double kp, double Ti, double Td, double Smin, double Smax, double Tp)
		: Tp(Tp),
		kp(kp),
		ki(kp* Tp / Ti),
		kd(kp* Td / Tp),
		Smin(Smin),
		Smax(Smax),
		inPrev(0),
		sum(0),
		out(0)
	{
	}

	explicit PID(const Data& data)
		: Tp(data.time.dt),
		kp(data.automaticControlSystem.pid.kp),
		ki(data.automaticControlSystem.pid.kp* data.automaticControlSystem.pid.Tp / data.automaticControlSystem.pid.Ti),
		kd(data.automaticControlSystem.pid.kp* data.automaticControlSystem.pid.Td / data.automaticControlSystem.pid.Tp),
		Smin(data.automaticControlSystem.pid.Smin),
		Smax(data.automaticControlSystem.pid.Smax),
		inPrev(0),
		sum(0),
		out(0)
	{

	}

	void calculate(double in)
	{
		sum += in * ki;
		out = sum + in * kp + (in - inPrev) * kd;
		if (out > Smax)
			out = Smax;
		if (out < Smin)
			out = Smin;
		inPrev = in;
	}
};
