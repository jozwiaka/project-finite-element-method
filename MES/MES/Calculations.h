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
#include "Utilities.h"

class Calculations
{
public:
	std::vector<double> T;
	std::vector<double> Y;
	size_t n;
	size_t nspf;

	Calculations() = default;

	void calculate_T(std::vector<std::vector<double>>& Aspf, std::vector<double>& B)
	{
		n = Aspf.size();
		nspf = Aspf[0].size();

		T.resize(n);
		Y.resize(n);
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

	void show_T()
	{
		std::cout << "X:" << std::endl;
		show(T);
	}
};