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
#include "AutomaticControlSystem.h"
#include "Utilities.h"
#include <filesystem>
#include <unordered_map>

static std::vector<std::future<void>> futures;
static std::string bcDefault;

class Run
{
public:
	void run(const std::string_view &directory)
	{
		std::string path = directory.data();
		Data data;
		size_t id_ss = 1;
		size_t id_ts = 1;
		size_t id_acs = 1;
		for (auto &entry : std::filesystem::directory_iterator(path))
		{
			const wchar_t *wchar = entry.path().c_str();
			std::wstring ws(wchar);
			std::string p(ws.begin(), ws.end());
			findAndReplaceAll(p, "\\", "/");
			data.readFromFile(p);
			if (p.find("ts") != std::string::npos)
			{
				data.id = id_ts;
				runTransientState(data);
				id_ts++;
			}
			else if (p.find("acs") != std::string::npos)
			{
				data.id = id_acs;
				runAutomaticControlSystem(data);
				id_acs++;
			}
			else if (p.find("ss") != std::string::npos)
			{
				data.id = id_ss;
				runSteadyState(data);
				id_ss++;
			}
		}
	}

	void runTransientState(const Data &data)
	{
		futures.emplace_back(std::async(std::launch::async, &Run::transientState, this, data));
		futures.emplace_back(std::async(std::launch::async, &Run::transientState_TN, this, data));
		futures.emplace_back(std::async(std::launch::async, &Run::transientState_N, this, data));
		futures.emplace_back(std::async(std::launch::async, &Run::latex, this, data, Path::ts_data.data(), "ts"));
	}

	void runSteadyState(const Data &data)
	{
		futures.emplace_back(std::async(std::launch::async, &Run::steadyState, this, data));
		futures.emplace_back(std::async(std::launch::async, &Run::latex, this, data, Path::ss_data.data(), "ss"));
	}

	void runAutomaticControlSystem(const Data &data)
	{
		futures.emplace_back(std::async(std::launch::async, &Run::identification, this, data));
		futures.emplace_back(std::async(std::launch::async, &Run::params, this, data));
		futures.emplace_back(std::async(std::launch::async, &Run::u_matlab, this, data));
		//futures.emplace_back(std::async(std::launch::async, &Run::u_matlab_TN, this, data));
		//futures.emplace_back(std::async(std::launch::async, &Run::u_matlab_N, this, data));
		futures.emplace_back(std::async(std::launch::async, &Run::latex, this, data, Path::acs_data.data(), "acs"));
		futures.emplace_back(std::async(std::launch::async, &Run::u_matlab_monitor, this, data));
		// futures.emplace_back(std::async(std::launch::async, &Run::automaticControlSystem, this, data));
		// futures.emplace_back(std::async(std::launch::async, &Run::automaticControlSystem_TN, this, data));
		// futures.emplace_back(std::async(std::launch::async, &Run::automaticControlSystem_N, this, data));
	}

private:
	void u_matlab_TN(const Data &data)
	{
		std::ifstream file_r(Path::acs.data() + std::string("acs") + std::to_string(data.id) + "_u_matlab.txt");
		if (!file_r.is_open())
		{
			return;
		}
		std::ofstream file_w(Path::acs.data() + std::string("acs") + std::to_string(data.id) + "_mes_TN.txt");
		if (!file_w.is_open())
			exit(-1);
		auto grid = std::make_unique<Grid>(data);
		auto acs = std::make_unique<AutomaticControlSystem>(*grid, data);
		double t = 0, t_prev = 0, yIn = 0;
		double T;
		std::string line = "";
		std::string comma = "";
		auto &dt = acs->ts.data.time.dt;
		while (std::getline(file_r, line))
		{
			std::istringstream iss(line);
			iss >> t >> comma >> yIn;
			if (t == 0)
			{
				file_w << t << ", ";
				for (const auto& Ti : acs->ts.T)
				{
					file_w << Ti << ", ";
				}
				file_w << "\n";
				continue;
			}
			dt = t - t_prev;
			if (dt == 0)
				continue;
			acs->calculate_control(yIn);
			file_w << t << ", ";
			for (const auto &Ti : acs->ts.T)
			{
				file_w << Ti << ", ";
			}
			file_w << "\n";
			t_prev = t;
		}
		file_r.close();
		file_w.close();
	}

	void u_matlab_N(const Data &data)
	{
		std::ifstream file_r(Path::acs.data() + std::string("acs") + std::to_string(data.id) + "_u_matlab.txt");
		if (!file_r.is_open())
		{
			return;
		}
		std::ofstream file_w(Path::acs.data() + std::string("acs") + std::to_string(data.id) + "_mes_N.txt");
		if (!file_w.is_open())
			exit(-1);
		auto grid = std::make_unique<Grid>(data);
		for (const auto &n : grid->nodes)
		{
			file_w << n->x << ", " << n->y << ", " << n->z << "\n";
		}
		file_w.close();
	}

	void u_matlab(const Data &data)

	{
		std::ifstream file_r(Path::acs.data() + std::string("acs") + std::to_string(data.id) + "_u_matlab.txt");
		if (!file_r.is_open())
		{
			return;
		}
		std::ofstream file_w(Path::acs.data() + std::string("acs") + std::to_string(data.id) + "_mes.txt");
		if (!file_w.is_open())
			exit(-1);
		auto grid = std::make_unique<Grid>(data);
		auto acs = std::make_unique<AutomaticControlSystem>(*grid, data);
		double t = 0, t_prev = 0, yIn = 0;
		double T;
		std::string line = "";
		std::string comma = "";
		auto &dt = acs->ts.data.time.dt;
		while (std::getline(file_r, line))
		{
			std::istringstream iss(line);
			iss >> t >> comma >> yIn;
			if (t == 0)
			{
				file_w << t << ", " << data.conditions.T0 << "\n";
				continue;
			}
			dt = t - t_prev;
			if (dt == 0)
				continue;
			acs->calculate_control(yIn);
			T = 0;
			for (const auto &p : data.points.points)
			{
				T += acs->ts.getT(p.x, p.y, p.z);
			}
			T /= data.points.points.size();
			file_w << t << ", " << T << "\n";
			t_prev = t;
		}
		file_r.close();
		file_w.close();
	}

	void u_matlab_monitor(const Data& data)

	{
		std::ifstream file_r(Path::acs.data() + std::string("acs") + std::to_string(data.id) + "_u_matlab.txt");
		if (!file_r.is_open())
		{
			return;
		}
		std::ofstream file_w(Path::acs.data() + std::string("acs") + std::to_string(data.id) + "_monitor.txt");
		if (!file_w.is_open())
			exit(-1);
		auto grid = std::make_unique<Grid>(data);
		auto acs = std::make_unique<AutomaticControlSystem>(*grid, data);
		double t = 0, t_prev = 0, yIn = 0;
		double T;
		std::string line = "";
		std::string comma = "";
		auto& dt = acs->ts.data.time.dt;
		for (const auto& p : data.monitor.points)
		{
			file_w << p.z << ", ";
		}
		file_w << "\n";
		while (std::getline(file_r, line))
		{
			std::istringstream iss(line);
			iss >> t >> comma >> yIn;
			if (t == 0)
			{
				file_w << t << ", ";
				for (const auto& p : data.monitor.points)
				{
					file_w << acs->ts.getT(p.x, p.y, p.z) << ", ";
				}
				file_w << "\n";
				continue;
			}
			dt = t - t_prev;
			if (dt == 0)
				continue;
			acs->calculate_control(yIn);
			T = 0;

			file_w << t << ", ";
			for (const auto& p : data.monitor.points)
			{
				file_w<<acs->ts.getT(p.x, p.y, p.z)<<", ";
			}
			file_w << "\n";
			t_prev = t;
		}
		file_r.close();
		file_w.close();
	}

	void latex(const Data &data, std::string path, std::string name)
	{
		std::ofstream file(path + name + std::to_string(data.id) + "_data" + ".txt");
		if (!file.is_open())
			exit(-1);
		auto grid = std::make_unique<Grid>(data);
		const auto &r = data.area.subAreas[0].range;
		const auto &ar = data.area.subAreas[0];
		const auto &t = data.time;
		const auto &c = data.conditions;
		const auto &acs = data.automaticControlSystem;
		const auto &pid = acs.pid;
		const auto &pm = data.peltierModules;
		file << rstr(r.x2 - r.x1) << "\n";
		file << rstr(ar.lambda) << "\n";
		file << rstr(t.tc) << "\n";
		file << rstr(r.y2 - r.y1) << "\n";
		file << rstr(ar.rho) << "\n";
		file << rstr(t.dt) << "\n";
		file << rstr(r.z2 - r.z1) << "\n";
		file << rstr(ar.cw) << "\n";
		if (rstr(ar.qv) == "-")
		{
			file << 0 << "\n";
		}
		else
		{
			file << rstr(ar.qv) << "\n";
		}
		file << rstr(c.T0) << "\n";
		file << std::to_string(grid->nodes.size()) << "\n";
		if (!c.boundaryConditions1.empty())
		{
			file << rstr(c.boundaryConditions1[0].T) << "\n";
		}
		else
		{
			file << "-\n";
		}
		if (!c.boundaryConditions2.empty())
		{
			file << rstr(c.boundaryConditions2[0].qs) << "\n";
		}
		else
		{
			file << "-\n";
		}
		if (!c.boundaryConditions3.empty())
		{
			file << rstr(c.boundaryConditions3[0].T) << "\n";
			file << rstr(c.boundaryConditions3[0].h) << "\n";
		}
		else
		{
			file << "-\n";
			file << "-\n";
		}
		file << rstr(acs.yIn) << "\n";
		file << rstr(pid.kp) << "\n";
		if (!pm.peltierModules.empty())
		{
			file << rstr(pm.peltierModules[0].alpha) << "\n";
		}
		else
		{
			file << "-\n";
		}
		file << rstr(acs.identification.Am) << "\n";
		file << rstr(pid.Ti) << "\n";
		if (!pm.peltierModules.empty())
		{
			file << rstr(pm.peltierModules[0].K) << "\n";
		}
		else
		{
			file << "-\n";
		}
		file << rstr(acs.identification.f0) << "\n";
		file << rstr(pid.Td) << "\n";
		if (!pm.peltierModules.empty())
		{
			file << rstr(pm.peltierModules[0].R);
		}
		else
		{
			file << "-";
		}
	}

	void params(const Data &data)
	{
		std::string path = Path::acs.data() + std::string("acs") + std::to_string(data.id) + "_params";
		std::ofstream file(path + ".txt");
		if (!file.is_open())
			exit(-1);
		const auto &pid = data.automaticControlSystem.pid;
		file << data.automaticControlSystem.yIn << "\n";
		file << pid.kp << "\n";
		file << pid.Ti << "\n";
		file << pid.Td << "\n";
		file << pid.Smin << "\n";
		file << pid.Smax << "\n";
		file << data.automaticControlSystem.identification.eps;
	}

	void identification(const Data &data)
	{
		auto grid = std::make_unique<Grid>(data);
		auto acs = std::make_unique<AutomaticControlSystem>(*grid, data);
		double T = 0;
		size_t iterations = data.time.tc / data.time.dt;
		const auto &c = data.conditions;
		const auto &a = data.automaticControlSystem;

		std::string path = Path::acs.data() + std::string("acs") + std::to_string(data.id) + "_obj";
		std::ofstream file(path + ".txt");
		if (!file.is_open())
			exit(-1);
		file << data.time.dt << "\n";

		for (const auto &p : data.points.points)
		{
			T += acs->ts.getT(p.x, p.y, p.z);
		}
		T /= data.points.points.size();
		file << acs->yIn_sin << ", " << T << "\n";

		for (size_t i = 1; i <= iterations; i++)
		{
			acs->calculate_sin();
			T = 0;
			for (const auto &p : data.points.points)
			{
				T += acs->ts.getT(p.x, p.y, p.z);
			}
			T /= data.points.points.size();
			file << acs->yIn_sin << ", " << T << "\n";
		}
		file.close();
	}

	void automaticControlSystem(const Data &data)
	{
		std::string path;
		std::string params;
		acs_setPath(data, path, params);
		std::ofstream file(path + ".txt");
		if (!file.is_open())
			exit(-1);
		auto grid = std::make_unique<Grid>(data);
		auto acs = std::make_unique<AutomaticControlSystem>(*grid, data);
		file << data.time.dt << "\n";
		double T = 0;
		double yIn = data.automaticControlSystem.yIn;
		size_t iterations = data.time.tc / data.time.dt;
		for (const auto &p : data.points.points)
		{
			T += acs->ts.getT(p.x, p.y, p.z);
		}
		T /= data.points.points.size();
		file << T << "\n";
		for (size_t i = 1; i <= iterations; i++)
		{
			acs->calculate_acs_discrete(yIn - T);
			T = 0;
			for (const auto &p : data.points.points)
			{
				T += acs->ts.getT(p.x, p.y, p.z);
			}
			T /= data.points.points.size();
			file << T << "\n";
		}
		file.close();
	}

	void automaticControlSystem_TN(const Data &data)
	{
		std::string path;
		std::string params;
		acs_setPath(data, path, params);
		std::ofstream file(path + "_TN.txt");
		if (!file.is_open())
			exit(-1);
		auto grid = std::make_unique<Grid>(data);
		auto acs = std::make_unique<AutomaticControlSystem>(*grid, data);
		int stepTime = 200;
		file << data.time.dt * stepTime << "\n";
		size_t iterations = static_cast<size_t>(data.time.tc / data.time.dt);
		double T = 0;
		double yIn = data.automaticControlSystem.yIn;
		for (size_t i = 0; i <= iterations; i++)
		{
			if (i)
			{
				acs->calculate_acs_discrete(yIn - T);
			}
			T = 0;
			for (const auto &p : data.points.points)
			{
				T += acs->ts.getT(p.x, p.y, p.z);
			}
			T /= data.points.points.size();
			if (!(i % stepTime))
			{
				for (const auto &Ti : acs->ts.T)
				{
					file << Ti << ", ";
				}
				file << "\n";
			}
		}
		file.close();
	}

	void automaticControlSystem_N(const Data &data)
	{
		std::string path;
		std::string params;
		acs_setPath(data, path, params);
		std::ofstream file(path + "_N.txt");
		if (!file.is_open())
			exit(-1);
		auto grid = std::make_unique<Grid>(data);
		for (const auto &n : grid->nodes)
		{
			file << n->x << ", " << n->y << ", " << n->z << "\n";
		}
		file.close();
	}

	void transientState(const Data &data)
	{
		std::string path;
		std::string bc;
		ts_setPathBc(data, path, bc);
		std::ofstream file(path + ".txt");
		if (!file.is_open())
			exit(-1);
		auto grid = std::make_unique<Grid>(data);
		auto ts = std::make_unique<TransientState>(*grid, data);
		const auto &point = data.points.points[0];
		const auto &area = data.area.subAreas[0];
		file << data.time.dt << ", " << point.z << ", " << data.conditions.T0 << ", " << area.lambda << ", " << area.rho << ", " << area.cw << ", " << bc << "\n";

		double T = 0;
		size_t iterations = data.time.tc / data.time.dt;
		T = ts->getT(point.x, point.y, point.z);
		file << T << "\n";
		for (size_t i = 1; i <= iterations; i++)
		{
			ts->calculate();
			T = ts->getT(point.x, point.y, point.z);
			file << T << "\n";
		}
		file.close();
	}

	void transientState_TN(const Data &data)
	{
		std::string path;
		ts_setPathBc(data, path);
		std::ofstream file(path + "_TN.txt");
		if (!file.is_open())
			exit(-1);
		auto grid = std::make_unique<Grid>(data);
		auto ts = std::make_unique<TransientState>(*grid, data);
		// int stepTime = 100;
		// file << data.time.dt * stepTime << "\n";
		//  file << data.time.dt << "\n"; //added
		size_t iterations = static_cast<size_t>(data.time.tc / data.time.dt);
		const double &dt = data.time.dt;
		for (size_t i = 0; i <= iterations; i++)
		{
			if (i)
			{
				ts->calculate();
			}
			// if (!(i % stepTime))
			{
				file << i * dt << ", ";
				for (const auto &Ti : ts->T)
				{
					file << Ti << ", ";
				}
				file << "\n";
			}
		}
		file.close();
	}

	void transientState_N(const Data &data)
	{
		std::string path;
		ts_setPathBc(data, path);
		std::ofstream file(path + "_N.txt");
		if (!file.is_open())
			exit(-1);
		auto grid = std::make_unique<Grid>(data);
		for (const auto &n : grid->nodes)
		{
			file << n->x << ", " << n->y << ", " << n->z << "\n";
		}
		file.close();
	}

	void steadyState(const Data &data)
	{
		std::string path;
		std::string bc;
		ss_setPathBc(data, path, bc);
		std::ofstream file(path + ".txt");
		if (!file.is_open())
			exit(-1);
		auto grid = std::make_unique<Grid>(data);
		auto ss = std::make_unique<SteadyState>(*grid, data);
		ss->calculate();
		file << data.area.subAreas[0].lambda << ", " << data.area.subAreas[0].qv << ", " << bc << "\n";
		size_t iz = 0;
		for (size_t i = 0; i < grid->nz; i++)
		{
			iz = (i * grid->nx * grid->ny + 1) - 1;
			file << grid->nodes[iz]->z << ", " << ss->T[iz] << "\n";
		}
		file.close();
	}

	void acs_setPath(const Data &data, std::string &path, std::string &params)
	{
		const auto &pid = data.automaticControlSystem.pid;
		path = Path::acs.data() +
			   std::string("acs" + std::to_string(data.id) + "_mes_yIn") + std::to_string((int)data.automaticControlSystem.yIn) +
			   std::string("_kp") + std::to_string((int)pid.kp) +
			   std::string("_Ti") + std::to_string((int)pid.Ti) +
			   std::string("_Td") + std::to_string((int)pid.Td);
	}

	void ts_setPathBc(const Data &data, std::string &path, std::string &bc = bcDefault)
	{
		if (data.conditions.boundaryConditions1.size() == 1 && data.conditions.boundaryConditions2.empty() && data.conditions.boundaryConditions3.empty())
		{
			double T = data.conditions.boundaryConditions1[0].T;
			bc = std::to_string(T);
			path = Path::ts_bc1.data() + std::string("ts" + std::to_string(data.id) + "_bc1");
		}
		else if (data.conditions.boundaryConditions1.empty() && data.conditions.boundaryConditions2.size() == 1 && data.conditions.boundaryConditions3.empty())
		{
			double qs = data.conditions.boundaryConditions2[0].qs;
			bc = std::to_string(qs);
			path = Path::ts_bc2.data() + std::string("ts" + std::to_string(data.id) + "_bc2");
		}
		else if (data.conditions.boundaryConditions1.empty() && data.conditions.boundaryConditions2.empty() && data.conditions.boundaryConditions3.size() == 1)
		{
			double T = data.conditions.boundaryConditions3[0].T;
			double alpha = data.conditions.boundaryConditions3[0].h;
			bc = std::to_string(T) + ", " + std::to_string(alpha);
			path = Path::ts_bc3.data() + std::string("ts" + std::to_string(data.id) + "_bc3");
		}
	}

	void ss_setPathBc(const Data &data, std::string &path, std::string &bc = bcDefault)
	{
		if (data.conditions.boundaryConditions1.size() == 2 && data.conditions.boundaryConditions2.empty() && data.conditions.boundaryConditions3.empty())
		{
			double T1 = data.conditions.boundaryConditions1[0].T;
			double T2 = data.conditions.boundaryConditions1[1].T;
			bc = std::to_string(T1) + ", " + std::to_string(T2);
			path = Path::ss_bc1.data() + std::string("ss" + std::to_string(data.id) + "_bc1");
		}
		/*else if (data.conditions.boundaryConditions1.empty() && data.conditions.boundaryConditions2.size() == 2 && data.conditions.boundaryConditions3.empty())
		{
			double qs1 = data.conditions.boundaryConditions2[0].qs;
			double qs2 = data.conditions.boundaryConditions2[1].qs;
			bc = std::to_string(qs1) + ", " + std::to_string(qs2);
			path = Path::ss_bc1.data() + std::string("ss_bc2_qs") + std::to_string((int)qs1) + std::string("_qs") + std::to_string((int)qs2);
		}
		else if (data.conditions.boundaryConditions1.empty() && data.conditions.boundaryConditions2.empty() && data.conditions.boundaryConditions3.size() == 2)
		{
			double T1 = data.conditions.boundaryConditions3[0].T;
			double T2 = data.conditions.boundaryConditions3[1].T;
			double alpha1 = data.conditions.boundaryConditions3[0].h;
			double alpha2 = data.conditions.boundaryConditions3[1].h;
			bc = std::to_string(T1) + ", " + std::to_string(alpha1) + ", " + std::to_string(T2) + ", " + std::to_string(alpha2);
			path = Path::ss_bc3.data() + std::string("ss_bc3_T") + std::to_string((int)T1) + std::string("_h") + std::to_string((int)alpha1) + std::string("_T") + std::to_string((int)T2) + std::string("_h") + std::to_string((int)alpha2);
		}*/
	}

public:
	void testCholesky()
	{
		std::vector<std::vector<double>> Aspf{{1, 2}, {30, -5}, {1, 0}};
		std::vector<double> B = {11, 121, -19};
		auto c = std::make_unique<Calculations>();
		c->calculate_T(Aspf, B);
		c->show_T();
		printMemoryUsage();
	}
};