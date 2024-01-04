#pragma once
#include "Node.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Element.h"
#include <memory>
#include "Timer.h"
#include "Data.h"


class Grid
{
private:
public:
	size_t nx, ny, nz;
	std::vector<std::shared_ptr<Node>> nodes;
	std::vector<Element> elements;

	Grid(const Data& data)
	{
		setNodes(data);
		setElements(data);
	}

	void setNodes(const Data& data)
	{
		std::vector<double> xn0;
		std::vector<double> yn0;
		std::vector<double> zn0;

		std::vector<double> dx;
		std::vector<double> dy;
		std::vector<double> dz;

		for (const auto& sa : data.area.subAreas)
		{
			xn0.emplace_back(sa.range.x1);
			xn0.emplace_back(sa.range.x2);
			yn0.emplace_back(sa.range.y1);
			yn0.emplace_back(sa.range.y2);
			zn0.emplace_back(sa.range.z1);
			zn0.emplace_back(sa.range.z2);
		}
		std::sort(xn0.begin(), xn0.end());
		std::sort(yn0.begin(), yn0.end());
		std::sort(zn0.begin(), zn0.end());

		xn0.erase(std::unique(xn0.begin(), xn0.end()), xn0.end());
		yn0.erase(std::unique(yn0.begin(), yn0.end()), yn0.end());
		zn0.erase(std::unique(zn0.begin(), zn0.end()), zn0.end());

		nx = 1;
		ny = 1;
		nz = 1;
		for (size_t i = 0; i < xn0.size() - 1; i++)
		{
			nx += std::pow(2, data.density.level.x);
			dx.emplace_back((xn0[i + 1] - xn0[i]) / std::pow(2, data.density.level.x));
		}
		for (size_t i = 0; i < yn0.size() - 1; i++)
		{
			ny += std::pow(2, data.density.level.y);
			dy.emplace_back((yn0[i + 1] - yn0[i]) / std::pow(2, data.density.level.y));
		}
		for (size_t i = 0; i < zn0.size() - 1; i++)
		{
			nz += std::pow(2, data.density.level.z);
			dz.emplace_back((zn0[i + 1] - zn0[i]) / std::pow(2, data.density.level.z));
		}
		size_t id = 0;
		nodes.reserve(nx * ny * nz);
		for (size_t i = 0; i < (zn0.size() - 1); i++)
		{
			for (auto z = i == 0 ? zn0[i] : zn0[i] + dz[i]; z <= zn0[i + 1] + 0.5 * dz[i]; z += dz[i])
			{
				for (size_t j = 0; j < (yn0.size() - 1); j++)
				{
					for (auto y = j == 0 ? yn0[j] : yn0[j] + dy[j]; y <= yn0[j + 1] + +0.5 * dy[j]; y += dy[j])
					{
						for (size_t k = 0; k < (xn0.size() - 1); k++)
						{
							for (auto x = k == 0 ? xn0[k] : xn0[k] + dx[k]; x <= xn0[k + 1] + 0.5 * dx[k]; x += dx[k])
							{
								nodes.emplace_back(std::make_shared<Node>(id, x, y, z));
								id++;
							}
						}
					}
				}
			}
		}
	}

	void setElements(const Data& data)
	{
		size_t id = 0;
		double lambda = 0;
		double rho = 0;
		double cw = 0;
		double qv = 0;
		elements.reserve((nx - 1) * (ny - 1) * (nz - 1));
		for (size_t z = 0; z < nz - 1; z++)
		{
			for (size_t y = 0; y < ny - 1; y++)
			{
				for (size_t x = 0; x < nx - 1; x++)
				{
					std::vector<std::shared_ptr<Node>> temp{
					nodes[z * (nx * ny) + y * nx + x],
					nodes[z * (nx * ny) + y * nx + x + 1],
					nodes[z * (nx * ny) + (y + 1) * nx + x],
					nodes[z * (nx * ny) + (y + 1) * nx + x + 1],
					nodes[(z + 1) * (nx * ny) + y * nx + x],
					nodes[(z + 1) * (nx * ny) + y * nx + x + 1],
					nodes[(z + 1) * (nx * ny) + (y + 1) * nx + x],
					nodes[(z + 1) * (nx * ny) + (y + 1) * nx + x + 1]
					};

					int nodesWhichBelongToArea;
					for (const auto& sa : data.area.subAreas)
					{
						nodesWhichBelongToArea = 0;
						for (const auto& n : temp)
						{
							if ((n->x >= sa.range.x1 - DBL_EPSILON && n->x <= sa.range.x2 + DBL_EPSILON) &&
								(n->y >= sa.range.y1 - DBL_EPSILON && n->y <= sa.range.y2 + DBL_EPSILON) &&
								(n->z >= sa.range.z1 - DBL_EPSILON && n->z <= sa.range.z2 + DBL_EPSILON)
								)
							{
								nodesWhichBelongToArea++;
							}
						}
						if (nodesWhichBelongToArea == 8)
						{
							lambda = sa.lambda;
							rho = sa.rho;
							cw = sa.cw;
							qv = sa.qv;
						}
					}
					elements.emplace_back(id, std::move(temp), lambda, rho, cw, qv);
					id++;
				}
			}
		}
	}

	void showNumberOfNodes()
	{
		std::cout << "--------------------------------------------------" << std::endl;
		std::cout << "Nodes = " << nodes.size() << std::endl;
		std::cout << "--------------------------------------------------" << std::endl << std::endl;
	}

	void showNodes()
	{
		for (auto&& n : nodes)
		{
			std::cout << "(" << n->x << ", " << n->y << ", " << n->z << ")" << std::endl;
		}
	}
};

