#pragma once
#include <vector>
#include "Grid.h"
#include <vector>
#include <future>
#include <thread>
#include <iostream>
#include "Timer.h"
#include <memory>
#include <iomanip>
#include <sstream>
#include "Utilities.h"
#include "Data.h"

class State
{
public:
	size_t n;
	size_t nspf;
	std::vector<double> Y;
	Grid grid;
	Data data;
	std::vector<double> T;
	State(const Grid& grid, const Data& data) : grid(grid), data(data)
	{
		n = grid.nodes.size();
		nspf = grid.nx * grid.ny + grid.nx + 2;

		T.resize(n);
		Y.resize(n);
	}


	void calculate_T(std::vector<std::vector<double>>& Aspf, std::vector<double>& B)
	{
		double yi;
		double xi;
		double sum;
		int temp1 = 0;
		int temp2;

		for (int i = 0; i < n; i++)
		{
			sum = 0;
			temp2 = 0;
			for (int k = 0; k <= i - 1; k++)
			{
				if (i < nspf + temp2 && i - temp2 >= 0)
				{
					sum += pow(Aspf[k][i - temp2], 2);
				}
				temp2++;
			}
			Aspf[i][0] = sqrt(Aspf[i][0] - sum);

			for (int j = i + 1; j < n; j++)
			{
				sum = 0;
				temp2 = 0;
				for (int k = 0; k <= i - 1; k++)
				{
					if (i < nspf + temp2 && j < nspf + temp2)
					{
						sum += Aspf[k][i - temp2] * Aspf[k][j - temp2];
					}
					temp2++;
				}
				if (j < nspf + temp1)
				{
					Aspf[i][j - temp1] = (Aspf[i][j - temp1] - sum) / Aspf[i][0];
				}
			}
			temp1++;
		}

		for (int i = 0; i < n; i++)
		{
			yi = 0;
			temp2 = 0;
			for (int j = 0; j < i; j++)
			{
				if (i < nspf + temp2)
				{
					yi -= Aspf[j][i - temp2] * Y[j];
				}
				temp2++;
			}
			yi += B[i];
			yi = yi / Aspf[i][0];
			Y[i] = yi;
		}

		for (int i = n - 1; i >= 0; i--)
		{
			xi = 0;
			for (int j = i + 1; j < n; j++)
			{
				if (j < nspf + i)
				{
					xi -= Aspf[i][j - i] * T[j];
				}
			}
			xi += Y[i];
			xi = xi / Aspf[i][0];
			T[i] = xi;
		}
	}


	void boundaryConditions1(std::vector<std::vector<double>>& Aspf, std::vector<double>& B)
	{
		for (auto const& ni : grid.nodes)
		{
			for (auto const& bc : data.conditions.boundaryConditions1)
			{
				if ((ni->x >= bc.range.x1 - DBL_EPSILON && ni->x <= bc.range.x2 + DBL_EPSILON) &&
					(ni->y >= bc.range.y1 - DBL_EPSILON && ni->y <= bc.range.y2 + DBL_EPSILON) &&
					(ni->z >= bc.range.z1 - DBL_EPSILON && ni->z <= bc.range.z2 + DBL_EPSILON)
					)
				{
					B[ni->id] = Aspf[ni->id][0] * 100000000 * bc.T;
					Aspf[ni->id][0] *= 100000000;
				}
			}
		}
	}

	void show_T()
	{
		std::cout << "T:" << std::endl;
		show(T);
	}

	double getT(double x, double y, double z)
	{
		double Txyz = 0;
		size_t i = 0;
		double xi = 0;
		double eta = 0;
		double delta = 0;
		for (const auto& e : grid.elements)
		{
			if (
				(x >= e.nodes[0]->x && x <= e.nodes[1]->x) &&
				(y >= e.nodes[0]->y && y <= e.nodes[2]->y) &&
				(z >= e.nodes[0]->z && z <= e.nodes[4]->z)
				)
			{
				xi = -1 + 2 * (x - e.nodes[0]->x) / e.a;
				eta = -1 + 2 * (y - e.nodes[0]->y) / e.b;
				delta = -1 + 2 * (z - e.nodes[0]->z) / e.c;

				for (const auto& n : e.nodes)
				{
					const auto& ei = e.xed(i);
					Txyz += T[n->id] / 8 * (1 + xi * ei.xi) * (1 + eta * ei.eta) * (1 + delta * ei.delta);
					i++;
				}
				return Txyz;
			}
		}
	}
};


//void calculate_T(std::vector<std::vector<double>>& Aspf, std::vector<double>& B) // Cholesky method
//{
//	double sum1 = 0;
//	double sum2 = 0;
//	for (size_t i = 0; i < n; i++)
//	{
//		for (size_t j = 0; j < i + 1; j++)
//		{
//			if (i == j)
//			{
//				for (size_t k = 0; k < j; k++)
//				{
//					sum1 += L[j][k] * L[j][k];
//				}
//				L[j][j] = std::sqrt(Aspf[j][0] - sum1);
//				sum1 = 0;
//			}
//			else
//			{
//				for (size_t k = 0; k < j; k++)
//				{
//					sum1 += L[j][k] * L[i][k];
//				}
//				if (j > i && (j - i) < nspf)
//				{
//					L[i][j] = (Aspf[i][j - i] - sum1) / L[j][j];
//				}
//				if (j < i && (i - j) < nspf)
//				{
//					L[i][j] = (Aspf[j][i - j] - sum1) / L[j][j];
//				}
//				sum1 = 0;
//			}
//			if (j < i)
//			{
//				sum2 += L[i][j] * Y[j];
//			}
//		}
//		Y[i] = (B[i] - sum2) / L[i][i];
//		sum2 = 0;
//	}

//	for (int i = n - 1; i >= 0; i--)
//	{
//		for (size_t j = n - 1; j > i; j--)
//		{
//			sum1 += L[j][i] * T[j];
//		}
//		T[i] = (Y[i] - sum1) / L[i][i];
//		sum1 = 0;
//	}
//}
