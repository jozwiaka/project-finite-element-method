#pragma once
struct Range
{
	double x1 = 0;
	double x2 = 0;
	double y1 = 0;
	double y2 = 0;
	double z1 = 0;
	double z2 = 0;
};

struct SubArea
{
	Range range;
	double lambda = 0;
	double rho = 0;
	double cw = 0;
	double qv = 0;
};

struct Point
{
	double x = 0;
	double y = 0;
	double z = 0;
};

struct Level
{
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;
};

struct BoundaryCondition1
{
	Range range;
	double T = 0;
	bool control = 0;
};

struct BoundaryCondition2
{
	Range range;
	double qs = 0;
	bool control = 0;
};

struct BoundaryCondition3
{
	Range range;
	double T = 0;
	double h = 0;
	bool control = 0;
};

enum class Heating_Cooling
{
	heating,
	cooling,
	notSet
};

struct PeltierModule
{
	Range range;
	Heating_Cooling h_c = Heating_Cooling::notSet;
	double T1 = 0;
	double T2 = 0;
	double alpha = 0;
	double K = 0;
	double R = 0;
	double Q = 0;
};