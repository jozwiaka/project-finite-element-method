#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "Metadata.h"

struct Data
{
	size_t id = 0;

	struct Area
	{
		std::vector<SubArea> subAreas;
	}area;

	struct Conditions
	{
		double T0 = 0;
		std::vector<BoundaryCondition1> boundaryConditions1;
		std::vector<BoundaryCondition2> boundaryConditions2;
		std::vector<BoundaryCondition3> boundaryConditions3;
	}conditions;

	struct Time
	{
		double dt = 0;
		double tc = 0;
	}time;

	struct Density
	{
		Level level;
	}density;

	struct Points
	{
		std::vector<Point> points;
	}points;

	struct Monitor
	{
		std::vector<Point> points;
	}monitor;

	struct AutomaticControlSystem
	{
		struct Identification
		{
			double Am = 0;
			double f0 = 0;
			double y0 = 0;
			double eps = 0;
		}identification;

		double yIn = 0;

		struct PID
		{
			double kp = 0;
			double Ti = 0;
			double Td = 0;
			double Smin = 0;
			double Smax = 0;
			double Tp = 0;
		}pid;
	}automaticControlSystem;

	struct PeltierModules
	{
		std::vector<PeltierModule> peltierModules;
	}peltierModules;

	void readFromFile(const std::string& path)
	{
		clear();
		enum class Mode
		{
			Area,
			Conditions,
			BoundaryCondition1,
			BoundaryCondition2,
			BoundaryCondition3,
			Time,
			Density,
			Points,
			Monitor,
			AutomaticControlSystem,
			Identification,
			PID,
			PeltierModule,
			NotSet
		};

		enum class Var
		{
			x1, x2, y1, y2, z1, z2,
			lambda, rho, cw, qv,
			T, T0, qs, h, control,
			x, y, z,
			dt, tc,
			lvl_x, lvl_y, lvl_z,
			yIn, Am, f0, y0, eps, kp, Ti, Td, Smin, Smax, Tp,
			Th, Tc, K, R, h_c, alpha,
			NotSet
		};
		std::unordered_map<std::string, Mode> modes =
		{
			{"Area",Mode::Area},
			{"Conditions",Mode::Conditions},
			{"BoundaryCondition1",Mode::BoundaryCondition1},
			{"BoundaryCondition2",Mode::BoundaryCondition2},
			{"BoundaryCondition3",Mode::BoundaryCondition3},
			{"Time",Mode::Time},
			{"Density",Mode::Density},
			{"Points",Mode::Points},
			{"Monitor",Mode::Monitor},
			{"AutomaticControlSystem", Mode::AutomaticControlSystem},
			{"Identification", Mode::Identification},
			{"PID",Mode::PID},
			{"PeltierModule",Mode::PeltierModule}
		};

		std::unordered_map<std::string, Var> vars =
		{
			{"x1",Var::x1},
			{"x2",Var::x2},
			{"y1",Var::y1},
			{"y2",Var::y2},
			{"z1",Var::z1},
			{"z2",Var::z2},
			{"lambda",Var::lambda},
			{"rho",Var::rho},
			{"cw",Var::cw},
			{"qv",Var::qv},
			{"T",Var::T},
			{"T0",Var::T0},
			{"qs",Var::qs},
			{"h",Var::h},
			{"x",Var::x},
			{"y",Var::y},
			{"z",Var::z},
			{"dt",Var::dt},
			{"tc",Var::tc},
			{"lvl_x",Var::lvl_x},
			{"lvl_y",Var::lvl_y},
			{"lvl_z",Var::lvl_z},
			{"yIn",Var::yIn},
			{"Am",Var::Am},
			{"f0",Var::f0},
			{"y0",Var::y0},
			{"eps",Var::eps},
			{"kp",Var::kp},
			{"Ti",Var::Ti},
			{"Td",Var::Td},
			{"Smin",Var::Smin},
			{"Smax",Var::Smax},
			{"Tp",Var::Tp},
			{"Th",Var::Th},
			{"Tc",Var::Tc},
			{"K",Var::K},
			{"R",Var::R},
			{"h_c",Var::h_c},
			{"alpha",Var::alpha},
			{"control",Var::control}
		};

		std::ifstream file(path);
		std::string line;
		std::string strVar;
		std::string strEq;
		double val = 0;
		Mode mode = Mode::NotSet;
		Mode modeBC = Mode::NotSet;
		Mode modeACS = Mode::NotSet;
		Var var = Var::NotSet;
		double lambda = 0, rho = 0, cw = 0, qv = 0, T = 0, T0 = 0, qs = 0, alpha = 0, dt = 0, tc = 0, yIn = 0, iIn = 0, kp = 0, Ti = 0, Td = 0, Smin = 0, Smax = 0, Tp = 0, Th = 0, Tc = 0, K = 0, R = 0, h = 0, Am = 0, f0 = 0, y0 = 0, eps = 0;
		bool control = 0;
		Heating_Cooling h_c = Heating_Cooling::notSet;
		Range range;
		Point point;
		Level level;

		if (file.is_open())
		{
			while (std::getline(file, line))
			{
				if (mode == Mode::NotSet)
				{
					if (modes.count(line))
					{
						mode = modes[line];
					}
					else
					{
						mode = Mode::NotSet;
						continue;
					}
				}

				std::istringstream iss(line);
				iss >> strVar >> strEq >> val;

				if (vars.count(strVar))
				{
					var = vars[strVar];
				}
				else
				{
					var = Var::NotSet;
				}

				switch (var)
				{
				case Var::x1:
					range.x1 = val;
					break;
				case Var::x2:
					range.x2 = val;
					break;
				case Var::y1:
					range.y1 = val;
					break;
				case Var::y2:
					range.y2 = val;
					break;
				case Var::z1:
					range.z1 = val;
					break;
				case Var::z2:
					range.z2 = val;
					break;
				case Var::lambda:
					lambda = val;
					break;
				case Var::rho:
					rho = val;
					break;
				case Var::cw:
					cw = val;
					break;
				case Var::qv:
					qv = val;
					break;
				case Var::T:
					T = val;
					break;
				case Var::T0:
					T0 = val;
					break;
				case Var::qs:
					qs = val;
					break;
				case Var::h:
					h = val;
					break;
				case Var::alpha:
					alpha = val;
					break;
				case Var::x:
					point.x = val;
					break;
				case Var::y:
					point.y = val;
					break;
				case Var::z:
					point.z = val;
					break;
				case Var::dt:
					dt = val;
					break;
				case Var::tc:
					tc = val;
					break;
				case Var::lvl_x:
					level.x = val;
					break;
				case Var::lvl_y:
					level.y = val;
					break;
				case Var::lvl_z:
					level.z = val;
					break;
				case Var::yIn:
					yIn = val;
					break;
				case Var::Am:
					Am = val;
					break;
				case Var::f0:
					f0 = val;
					break;
				case Var::y0:
					y0 = val;
					break;
				case Var::eps:
					eps = val;
					break;
				case Var::kp:
					kp = val;
					break;
				case Var::Ti:
					Ti = val;
					break;
				case Var::Td:
					Td = val;
					break;
				case Var::Smin:
					Smin = val;
					break;
				case Var::Smax:
					Smax = val;
					break;
				case Var::Tp:
					Tp = val;
					break;
				case Var::Th:
					Th = val;
					break;
				case Var::Tc:
					Tc = val;
					break;
				case Var::K:
					K = val;
					break;
				case Var::R:
					R = val;
					break;
				case Var::h_c:
					if (val == 0)
						h_c = Heating_Cooling::heating;
					if (val == 1)
						h_c = Heating_Cooling::cooling;
					break;
				case Var::control:
					control = val;
					break;
				default:
					break;
				}

				switch (mode)
				{
				case Mode::Area:
					if (line.find('}') != std::string::npos)
					{
						area.subAreas.emplace_back(
							range,
							lambda,
							rho,
							cw,
							qv);
					}
					break;
				case Mode::Conditions:
					conditions.T0 = T0;
					if (modes.count(strVar))
					{
						modeBC = modes[strVar];
					}
					switch (modeBC)
					{
					case Mode::BoundaryCondition1:
						if (line.find('}') != std::string::npos)
						{
							conditions.boundaryConditions1.emplace_back(
								range,
								T,
								control);
							control = 0;
						}
						break;
					case Mode::BoundaryCondition2:
						if (line.find('}') != std::string::npos)
						{
							conditions.boundaryConditions2.emplace_back(
								range,
								qs,
								control);
							control = 0;
						}
						break;
					case Mode::BoundaryCondition3:
						if (line.find('}') != std::string::npos)
						{
							conditions.boundaryConditions3.emplace_back(
								range,
								T,
								h,
								control);
							control = 0;
						}
						break;
					}
					break;

				case Mode::Time:
					time.dt = dt;
					time.tc = tc;
					break;

				case Mode::Density:
					density.level.x = level.x;
					density.level.y = level.y;
					density.level.z = level.z;
					break;

				case Mode::Points:
					if (line.find('}') != std::string::npos)
					{
						points.points.emplace_back(point);
					}
					break;

				case Mode::Monitor:
					if (line.find('}') != std::string::npos)
					{
						monitor.points.emplace_back(point);
					}
					break;

				case Mode::Identification:
					automaticControlSystem.identification.Am = Am;
					automaticControlSystem.identification.f0 = f0;
					automaticControlSystem.identification.y0 = y0;
					automaticControlSystem.identification.eps = eps;
					break;

				case Mode::AutomaticControlSystem:
					automaticControlSystem.yIn = yIn;

					if (modes.count(strVar))
					{
						modeACS = modes[strVar];
					}

					switch (modeACS)
					{

					case Mode::PID:
						automaticControlSystem.pid.kp = kp;
						automaticControlSystem.pid.Ti = Ti;
						automaticControlSystem.pid.Td = Td;
						automaticControlSystem.pid.Smin = Smin;
						automaticControlSystem.pid.Smax = Smax;
						automaticControlSystem.pid.Tp = Tp;
						break;
					}
					if (line.find(");") != std::string::npos)
					{
						modeACS = Mode::NotSet;
					}
					break;
				case Mode::PeltierModule:
					if (line.find('}') != std::string::npos)
					{
						peltierModules.peltierModules.emplace_back(
							range,
							h_c,
							0,
							0,
							alpha,
							K,
							R,
							0
						);
					}
					break;
				default:
					break;
				}
				if (line.find(");") != std::string::npos) mode = Mode::NotSet;
			}
			file.close();
		}
		else
		{
			std::cout << "Can't open file from path: " << path << "\n";
			exit(-1);
		}

	};

private:
	void clear()
	{
		area.subAreas.clear();
		conditions.boundaryConditions1.clear();
		conditions.boundaryConditions2.clear();
		conditions.boundaryConditions3.clear();
		points.points.clear();
	}
};