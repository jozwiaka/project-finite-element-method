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

class TransientState : public State
{
public:
	std::vector<std::vector<double>> Aspf;
	std::vector<double> B;

	TransientState(const Grid &grid, const Data &data) : State(grid, data)
	{
		Aspf.resize(n, std::vector<double>(nspf));
		B.resize(n);
		setT0();
	}

	void setT0()
	{
		for (int i = 0; i < n; i++)
		{
			T[i] = data.conditions.T0;
		}
	}

	void calculate()
	{
		calculate_AB();
		boundaryConditions1(Aspf, B);
		calculate_T(Aspf, B);
	}

	void calculate(const Data &data_new)
	{
		updateData(data_new);
		calculate_AB();
		boundaryConditions1(Aspf, B);
		calculate_T(Aspf, B);
	}

	void updateData(const Data &data_new)
	{
		data = data_new;
		// auto& c = data.conditions;
		// const auto& c_new = data_new.conditions;
		// if (!c.boundaryConditions1.empty() && !c_new.boundaryConditions1.empty())
		// {
		// 	for (int i = 0; i<c.boundaryConditions1.size();i++)
		// 	{

		// 	data.conditions.boundaryConditions1[0].T = data_new.conditions.boundaryConditions1[0].T;
		// 	}
		// }
		// if (!c.boundaryConditions2.empty() && !c_new.boundaryConditions2.empty())
		// {
		// 	data.conditions.boundaryConditions2[0].qs = data_new.conditions.boundaryConditions2[0].qs;
		// }
		// if (!c.boundaryConditions3.empty() && !c_new.boundaryConditions3.empty())
		// {
		// 	data.conditions.boundaryConditions3[0].T = data_new.conditions.boundaryConditions3[0].T;
		// }
		// auto& pm = data.peltierModules;
		// const auto& pm_new = data_new.peltierModules;
		// for (int i = 0; i < pm.peltierModules.size(); i++)
		// {
		// 	pm.peltierModules[i].Q = pm_new.peltierModules[i].Q;
		// }
	}

	void calculate_AB()
	{
		size_t i = 0;
		size_t j = 0;
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < nspf; j++)
			{
				Aspf[i][j] = 0;
			}
			B[i] = 0;
		}
		i = 0;
		j = 0;
		double t = 0;
		double k = 0;
		double cdt = 0;
		double p = 0;

		for (const auto &e : grid.elements)
		{
			for (const auto &ni : e.nodes)
			{
				const auto &ei = e.xed(i);
				for (const auto &nj : e.nodes)
				{
					const auto &ej = e.xed(j);
					k = e.lambda / 144.0 *
						(e.a * e.b * ej.delta * ei.delta * (ej.eta * ei.eta + 3) * (ej.xi * ei.xi + 3) / e.c + e.a * e.c * ej.xi * ei.xi * (ej.delta * ei.delta + 3) * (ej.eta * ei.eta + 3) / e.b + e.b * e.c * ej.eta * ei.eta * (ej.xi * ei.xi + 3) * (ej.delta * ei.delta + 3) / e.a);

					for (auto const &bc : data.conditions.boundaryConditions3)
					{
						if ((ni->x >= bc.range.x1 - DBL_EPSILON && ni->x <= bc.range.x2 + DBL_EPSILON) &&
							(ni->y >= bc.range.y1 - DBL_EPSILON && ni->y <= bc.range.y2 + DBL_EPSILON) &&
							(ni->z >= bc.range.z1 - DBL_EPSILON && ni->z <= bc.range.z2 + DBL_EPSILON))
						{
							const int constant = 36;
							if (fabs(bc.range.x2 - bc.range.x1) < DBL_EPSILON)
							{
								if (ni->id == nj->id)
								{
									k += bc.h * e.b * e.c / constant * 4;
								}
								else if (
									(i == 1 && j == 7) || (i == 7 && j == 1) ||
									(i == 0 && j == 6) || (i == 6 && j == 0) ||
									(i == 3 && j == 5) || (i == 5 && j == 3) ||
									(i == 2 && j == 4) || (i == 4 && j == 2))
								{
									k += bc.h * e.b * e.c / constant;
								}
								else
								{
									k += bc.h * e.b * e.c / constant * 2;
								}
							}
							else if (fabs(bc.range.y2 - bc.range.y1) < DBL_EPSILON)
							{
								if (ni->id == nj->id)
								{
									k += bc.h * e.a * e.c / constant * 4;
								}
								else if (
									(i == 0 && j == 5) || (i == 5 && j == 0) ||
									(i == 1 && j == 4) || (i == 4 && j == 1) ||
									(i == 3 && j == 6) || (i == 6 && j == 3) ||
									(i == 2 && j == 7) || (i == 7 && j == 2))
								{
									k += bc.h * e.a * e.c / constant;
								}
								else
								{
									k += bc.h * e.a * e.c / constant * 2;
								}
							}
							else if (fabs(bc.range.z2 - bc.range.z1) < DBL_EPSILON)
							{
								if (ni->id == nj->id)
								{
									k += bc.h * e.a * e.b / constant * 4;
								}
								else if (
									(i == 0 && j == 3) || (i == 3 && j == 0) ||
									(i == 1 && j == 2) || (i == 2 && j == 1) ||
									(i == 5 && j == 6) || (i == 6 && j == 5) ||
									(i == 4 && j == 7) || (i == 7 && j == 4))
								{
									k += bc.h * e.a * e.b / constant;
								}
								else
								{
									k += bc.h * e.a * e.b / constant * 2;
								}
							}
						}
					}

					cdt = 3.0 / data.time.dt * e.a * e.b * e.c * e.cw * e.rho / 1728.0 * (ej.delta * ei.delta + 3) * (ej.eta * ei.eta + 3) * (ej.xi * ei.xi + 3);

					if (nj->id - ni->id >= 0 && nj->id - ni->id <= nspf - 1)
					{
						Aspf[ni->id][nj->id - ni->id] += 2 * k + cdt;
					}
					t += (cdt - k) * T[nj->id];
					j++;
				}
				p = e.a * e.b * e.c * e.qv / 8;

				for (auto &pm : data.peltierModules.peltierModules)
				{
					if ((ni->x >= pm.range.x1 - DBL_EPSILON && ni->x <= pm.range.x2 + DBL_EPSILON) &&
						(ni->y >= pm.range.y1 - DBL_EPSILON && ni->y <= pm.range.y2 + DBL_EPSILON) &&
						(ni->z >= pm.range.z1 - DBL_EPSILON && ni->z <= pm.range.z2 + DBL_EPSILON))
					{
						p += pm.Q / 4;
					}
				}

				for (auto const &bc : data.conditions.boundaryConditions2)
				{
					if ((ni->x >= bc.range.x1 - DBL_EPSILON && ni->x <= bc.range.x2 + DBL_EPSILON) &&
						(ni->y >= bc.range.y1 - DBL_EPSILON && ni->y <= bc.range.y2 + DBL_EPSILON) &&
						(ni->z >= bc.range.z1 - DBL_EPSILON && ni->z <= bc.range.z2 + DBL_EPSILON))
					{
						if (fabs(bc.range.x2 - bc.range.x1) < DBL_EPSILON)
						{
							p += bc.qs * e.b * e.c / 4;
						}
						else if (fabs(bc.range.y2 - bc.range.y1) < DBL_EPSILON)
						{
							p += bc.qs * e.a * e.c / 4;
						}
						else if (fabs(bc.range.z2 - bc.range.z1) < DBL_EPSILON)
						{
							p += bc.qs * e.a * e.b / 4;
						}
					}
				}

				for (auto const &bc : data.conditions.boundaryConditions3)
				{
					if ((ni->x >= bc.range.x1 - DBL_EPSILON && ni->x <= bc.range.x2 + DBL_EPSILON) &&
						(ni->y >= bc.range.y1 - DBL_EPSILON && ni->y <= bc.range.y2 + DBL_EPSILON) &&
						(ni->z >= bc.range.z1 - DBL_EPSILON && ni->z <= bc.range.z2 + DBL_EPSILON))
					{
						if (fabs(bc.range.x2 - bc.range.x1) < DBL_EPSILON)
						{
							p += bc.h * bc.T * e.b * e.c / 4;
						}
						else if (fabs(bc.range.y2 - bc.range.y1) < DBL_EPSILON)
						{
							p += bc.h * bc.T * e.a * e.c / 4;
						}
						else if (fabs(bc.range.z2 - bc.range.z1) < DBL_EPSILON)
						{
							p += bc.h * bc.T * e.a * e.b / 4;
						}
					}
				}

				B[ni->id] += 3 * p + t;
				i++;
				t = 0;
				j = 0;
			}
			i = 0;
		}
	}
};