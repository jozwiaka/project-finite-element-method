#pragma once
#include "Node.h"
#include <vector>
#include <memory>
#include <stdexcept>
#include <array>

class Element
{
public:
	size_t id;
	double a, b, c;
	double lambda;
	double rho;
	double cw;
	double qv;
	std::vector<std::shared_ptr<Node>> nodes;

	struct localCoordinate
	{
		double xi;
		double eta;
		double delta;
	};

	Element(size_t id, std::vector<std::shared_ptr<Node>> nodes, double lambda, double rho, double cw, double qv) : id(id), nodes(nodes), lambda(lambda), rho(rho), cw(cw), qv(qv)
	{
		a = nodes[1]->x - nodes[0]->x;
		b = nodes[2]->y - nodes[0]->y;
		c = nodes[4]->z - nodes[0]->z;
	}

	localCoordinate xed(size_t index) const
	{
		switch (index)
		{
		case 0:
			return localCoordinate{ -1, -1, -1 };
			break;
		case 1:
			return localCoordinate{ 1, -1, -1 };
			break;
		case 2:
			return localCoordinate{ -1, 1, -1 };
			break;
		case 3:
			return localCoordinate{ 1, 1, -1 };
			break;
		case 4:
			return localCoordinate{ -1, -1, 1 };
			break;
		case 5:
			return localCoordinate{ 1, -1, 1 };
			break;
		case 6:
			return localCoordinate{ -1, 1, 1 };
			break;
		case 7:
			return localCoordinate{ 1, 1, 1 };
			break;
		default:
			throw std::invalid_argument("Index has to be 0-7");
		}
	}
};

