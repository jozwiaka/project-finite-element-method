#pragma once
#include "Grid.h"
#include <vector>
#include <future>
#include <thread>
#include <iostream>
#include "Timer.h"
#include <memory>
#include <iomanip>
#include <sstream>
#include "State.h"
#include "Utilities.h"

class SteadyState : public State
{
public:
	std::vector<std::vector<double>> Kspf;
	std::vector<double> P;

	SteadyState(const Grid& grid, const Data& data) : State(grid, data)
	{
		Kspf.resize(n, std::vector<double>(nspf));
		P.resize(n);
	}

	void calculate()
	{
		calculate_KspfP();
		boundaryConditions1(Kspf, P);
		calculate_T(Kspf, P);
	}

	void calculate_KspfP()
	{
		auto future_Kspf = std::async(std::launch::async, &SteadyState::calculate_Kspf, this);
		auto future_P = std::async(std::launch::async, &SteadyState::calculate_P, this);
		future_Kspf.wait();
		future_P.wait();
	}

	void calculate_P()
	{
		for (const auto& e : grid.elements)
		{
			for (const auto& ni : e.nodes)
			{
				P[ni->id] += e.a * e.b * e.c * e.qv / 8;
			}
		}
	}

	void calculate_Kspf()
	{
		size_t i = 0;
		size_t j = 0;
		for (const auto& e : grid.elements)
		{
			for (const auto& ni : e.nodes)
			{
				const auto& ei = e.xed(i);
				for (const auto& nj : e.nodes)
				{
					if (nj->id - ni->id < 0 || nj->id - ni->id > nspf - 1)
					{
						j++;
						continue;
					}
					const auto& ej = e.xed(j);
					Kspf[ni->id][nj->id - ni->id] += e.lambda / 144 * (e.a * e.b * ej.delta * ei.delta * (ej.eta * ei.eta + 3) * (ej.xi * ei.xi + 3) / e.c + e.a * e.c * ej.xi * ei.xi * (ej.delta * ei.delta + 3) * (ej.eta * ei.eta + 3) / e.b + e.b * e.c * ej.eta * ei.eta * (ej.xi * ei.xi + 3) * (ej.delta * ei.delta + 3) / e.a);
					j++;
				}
				i++;
				j = 0;
			}
			i = 0;
		}
	}
};
