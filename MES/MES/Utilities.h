#pragma once
#include<iostream>
#include<vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <tuple>

inline void cout(const std::string& str)
{
	std::cout << str << std::endl;

}

inline void cout(double d)
{
	std::cout << std::to_string(d) << std::endl;

}

inline bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

inline std::tuple<bool, std::string, std::string> read_one_value(std::istream& in)
{
	std::string name;

	if (std::getline(in, name, '='))
	{
		std::string value;

		if (std::getline(in, value))
		{
			return std::make_tuple(true, name, value);
		}
	}
	return std::make_tuple(false, "", "");
}

inline void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr)
{
	size_t pos = data.find(toSearch);
	while (pos != std::string::npos)
	{
		data.replace(pos, toSearch.size(), replaceStr);
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}

inline size_t numberOfDigits(double precision)
{
	std::ostringstream strs;
	strs << precision;
	return strs.str().size();
}

inline double round(double d, int nAfterDot)
{
	int temp = std::pow(10, nAfterDot);
	d = static_cast<int>(d * temp);
	d = d / temp;
	return d;
}

inline void show(const std::vector<double>& B)
{
	for (const auto& b : B)
	{
		std::cout << b << std::endl;
	}
}

inline std::string rstr(double d)
{
	if (d == 0) return "-";
	std::ostringstream ss;
	std::string str = "";
	if (std::abs(d) >= 10000)
	{
		int n = 0;
		for (int i = 4; i++;)
		{
			n = i;
			if (std::abs(d) < std::pow(10, n))
			{
				d = d / std::pow(10, n - 1);
				break;
			}
		}
		ss << std::fixed << std::setprecision(2) << d;
		str = ss.str();
		for (int i = str.size(); i--;)
		{
			if (str[i] == '.')
			{
				return str.erase(i) + "\\mathrm{e+}" + std::to_string(n - 1);
			}
			if (str[i] != '0')
			{
				return str.erase(i+1) + "\\mathrm{e+}" + std::to_string(n - 1);
			}
		}
	}
	else if (std::abs(d) <= 0.01)
	{
		int n = 0;
		for (int i = -2; i--;)
		{
			n = -i;
			if (std::abs(d) >= std::pow(10, i))
			{
				d = d * std::pow(10, n);
				break;
			}
		}
		ss << std::fixed << std::setprecision(2) << d;
		str = ss.str();
		for (int i = str.length(); i--;)
		{
			if (str[i] == '.')
			{
				return str.erase(i) + "\\mathrm{e-}" + std::to_string(n);
			}
			if(str[i] != '0')
			{
				return str.erase(i+1) + "\\mathrm{e-}" + std::to_string(n);
			}
		}
	}
	else
	{
		ss << std::fixed << std::setprecision(2) << d;
		str = ss.str();
		for (int i = str.size(); i--;)
		{
			if (str[i] == '.')
			{
				return str.erase(i);
			}
			if(str[i] != '0')
			{
				return str.erase(i+1);
			}
		}
	}
	return str;
}

inline void show(const std::vector<std::vector<double>>& A)
{
	auto n = A.size();
	auto m = A[n - 1].size();
	constexpr size_t nmax{ 100 };
	std::vector<size_t> max_len_per_column;
	max_len_per_column.resize(m);
	size_t max_len = 0;
	int precision = 1;
	for (size_t j = 0; j < m; j++)
	{
		max_len = 0;
		for (size_t i = 0; i < n; i++)
		{
			size_t num_length = numberOfDigits(round(A[i][j], precision));
			if (num_length > max_len)
			{
				max_len = num_length;
			}
		}
		max_len_per_column[j] = max_len;
	}
	double max = *std::max_element(max_len_per_column.begin(), max_len_per_column.end());
	std::cout << std::fixed << std::setprecision(precision);

	for (size_t i = 0; i < n; i++)
	{
		std::cout << std::endl
			<< std::setw(2) << "|" << std::setw(1);
		for (size_t j = 0; j < m; j++)
		{
			std::cout << std::setw(max + 1); // max+1
			std::cout << A[i][j];
			if (j == m - 1)
				std::cout << std::setw(1) << " |";
		}
	}
	std::cout << std::endl;
}

