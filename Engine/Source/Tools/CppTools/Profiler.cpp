#include "Profiler.h"
#include "logger.h"
#include "EngineLiterals.h"
#include <fstream>
#include <chrono>

Shorts;
using time_point = std::chrono::steady_clock::time_point;
using duration = std::chrono::duration<double>; // units of seconds
using DurationByLine = map_uo<int, duration>;

const int funcEnd = -1;
static map_uo<string, DurationByLine> performanceData;
static map_uo<string, int> callCountByFunc;
static map_uo<string, time_point> startByFunc;

inline time_point Now() { return std::chrono::high_resolution_clock::now(); }
//double to_double(duration dur) { return std::chrono::duration<double>(dur).count(); } // also casts ns to s

Profiler::FuncTimer::FuncTimer(const string& pathToFunc_)
{

	pathToFunc = pathToFunc_;
	startByFunc[pathToFunc] = Now();
	callCountByFunc[pathToFunc] += 1;
	performanceData[pathToFunc]; // initializes performanceData for pathToFunc, if not already initialized
}

Profiler::FuncTimer::~FuncTimer()
{
	if (Tools::ContainsKey(startByFunc, pathToFunc))
		performanceData[pathToFunc][funcEnd] += Now() - startByFunc.at(pathToFunc);
}



void Profiler::_ProfileLine(const string& pathToFunc, int codeLine)
{
	performanceData.at(pathToFunc)[codeLine] += Now() - startByFunc.at(pathToFunc);
}

void Profiler::Print()
{
	logger::print(_CalculateOutput());
}

void Profiler::LogToFile()
{
	std::ofstream logFile(Literals::Log + "lastLog.txt");

	if (!logFile) // Check if the file is open
	{
		Profiler::Print();
		RaiseError("Failed to open file. Logging to console instead");
	}

    logFile << _CalculateOutput() << std::endl;
    // logFile is automatically closed, when leaving scope.
}

std::string Column(const std::string& str)
{
	uint columnWidth = 6;
	if (str.length() >= columnWidth)
		return str; // No padding needed if string is already equal to or longer than the specified length
	
	return str + std::string(columnWidth - str.length(), ' ') + "| "; // Append spaces to the original string
}

string Profiler::_CalculateOutput()
{
	string out = "";
	string next = "\n    ";
	for (const auto& [func, durationByLine] : performanceData)
	{
		int callCount = callCountByFunc.at(func);

		out += "\n" + func + next;
		if (durationByLine.size() > 1)
		{
			out += Column("line") + "durationPerFrame" + next;
			out += "-----------------------" + next;
		}
		for (const auto& [line, duration] : durationByLine)
		{
			if (line == funcEnd)
				continue;
			double durationPerFrame = durationByLine.at(funcEnd).count() / callCount;
			out += Column(std::to_string(line)) + std::to_string(durationPerFrame) + next;
		}

		double durationPerFrame = durationByLine.at(funcEnd).count() / callCount;
		out += Column("total") + std::to_string(durationPerFrame) + " (" + std::to_string(callCount) + " hits) \n";
	}
	return out;
}



void Profiler::Reset()
{
	performanceData.clear();
	callCountByFunc.clear();
	startByFunc.clear();
}