#pragma once
#include <iostream>
#include "Path.h"
#include "Grid.h"
#include <vector>
#include <memory>
#include "Node.h"
#include "Calculations.h"
#include "Timer.h"
#include <chrono>
#include "TransientState.h"
#include "SteadyState.h"
#include "Memory.h"
#include <future>
#include "Data.h"
#include "PID.h"
#define _USE_MATH_DEFINES
#include <cmath>

class AutomaticControlSystem
{
public:
	AutomaticControlSystem(const Grid& grid, const Data& data) : data(data), ts(grid, data),
		pid(data), e(0), u(0), yIn_sin(data.automaticControlSystem.identification.y0), t(1), pi(std::acos(-1))
	{
		const auto& c = data.conditions;
		const auto& a = data.automaticControlSystem;
		if (data.peltierModules.peltierModules.size() != 0 && a.identification.f0 != 0)
		{
			mode = Mode::pm;

			for (auto& pm : this->data.peltierModules.peltierModules)
			{
				pm.T1 = data.conditions.T0;
				pm.T2 = data.conditions.T0;
			}
		}
		else if (c.boundaryConditions1.size() == 6 && a.identification.f0 != 0)
		{
			mode = Mode::bc1;
		}
		else if (c.boundaryConditions2.size() != 0 && a.identification.f0 != 0)
		{
			mode = Mode::bc2;
		}
		else if (c.boundaryConditions3.size() != 0 && a.identification.f0 != 0)
		{
			mode = Mode::bc3;
		}
		else
		{
			mode = Mode::notSet;
		}
	}

	void calculate_control(double yIn)
	{
		switch (mode)
		{
		case Mode::bc1:
			for (int i = 0; i < data.conditions.boundaryConditions1.size(); i++)
			{
				if (data.conditions.boundaryConditions1[i].control)
					data.conditions.boundaryConditions1[i].T = yIn;
			}
			break;
		case Mode::bc2:
			for (int i = 0; i < data.conditions.boundaryConditions2.size(); i++)
			{
				if (data.conditions.boundaryConditions2[i].control)
					data.conditions.boundaryConditions2[i].qs = yIn;
			}
			break;
		case Mode::bc3:
			for (int i = 0; i < data.conditions.boundaryConditions3.size(); i++)
			{
				if (data.conditions.boundaryConditions3[i].control)
					data.conditions.boundaryConditions3[i].T = yIn;
			}
			break;
		case Mode::pm:
			calculate_pm(yIn);
			break;
		}
		ts.calculate(data);
	}

	void calculate_sin()
	{
		sin();
		calculate_control(yIn_sin);
	}

	void control_pm(double yIn)
	{
		calculate_pm(yIn);
		ts.calculate(data);
	}

	void calculate_acs_discrete(double eIn)
	{
		e = eIn;
		pid.calculate(e);
		u = pid.out;
		control_pm(u);
	}

	void yIn_const()
	{
		yIn_sin = data.automaticControlSystem.identification.y0;
	}

	void sin()
	{
		auto& ident = data.automaticControlSystem.identification;
		double phi = std::fmod(2 * pi * ident.f0 * t * data.time.dt, 2 * pi);
		if (ident.f0 * t >= 1)
		{
			ident.f0 = ident.f0 * 2;
			t = 0;
		}
		yIn_sin = ident.Am * std::sin(phi) + ident.y0;
		t = t + data.time.dt;
	}

	void sin2()
	{
		auto& ident = data.automaticControlSystem.identification;
		double f = ident.f0 * 2;
		if (f * t >= 1)
		{
			ident.Am = -ident.Am;
			if (ident.Am > 0)
				ident.f0 = ident.f0 * 2;
			t = 0;
		}
		yIn_sin = ident.Am + ident.y0;
		t = t + data.time.dt;
	}

	void calculate_pm(double I)
	{
		for (auto& pm : data.peltierModules.peltierModules)
		{
			for (auto const& ni : ts.grid.nodes)
			{
				if ((ni->x >= pm.range.x1 - DBL_EPSILON && ni->x <= pm.range.x2 + DBL_EPSILON) &&
					(ni->y >= pm.range.y1 - DBL_EPSILON && ni->y <= pm.range.y2 + DBL_EPSILON) &&
					(ni->z >= pm.range.z1 - DBL_EPSILON && ni->z <= pm.range.z2 + DBL_EPSILON)
					)
				{
					pm.T2 = ts.T[ni->id];
					break;
				}
			}
			if (pm.T2 > 473) I = 0;
			if (pm.T2 < 243) I = 0;
			pm.Q = pm.alpha * pm.T2 * I - pm.K * (pm.T2 - pm.T1) - I * I * pm.R;
			//pm.Q = pm.alpha * pm.T2 * I;
		}
	}

	TransientState ts;
	double e;
	double u;
	double yIn_sin;
	const double pi;
	enum class Mode
	{
		bc1, bc2, bc3, pm, notSet
	}mode;
private:
	int t;
	Data data;
	PID pid;
};

