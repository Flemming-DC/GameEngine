#include "Profiler.h"
#include "logger.h"
#include "EngineLiterals.h"
#include <fstream>
#include <chrono>

Shorts;
using time_point = std::chrono::steady_clock::time_point;
using duration = std::chrono::duration<double>; // units of seconds

struct Record 
{
	int callCount;
	time_point start;
	time_point last; // last ProfileFunc or ProfileLine
	duration durationFunc;
	map<string, duration> durationByLine; // ordered map
};
static map_uo<string, Record> data;

inline time_point Now() { return std::chrono::high_resolution_clock::now(); }

Profiler::FuncTimer::FuncTimer(const string& pathToFunc_)
{
	pathToFunc = pathToFunc_;
	data[pathToFunc]; // this initializes data[pathToFunc] if not already initialized
	data.at(pathToFunc).callCount += 1;
	data.at(pathToFunc).start = Now();
	data.at(pathToFunc).last = data.at(pathToFunc).start;
}

Profiler::FuncTimer::~FuncTimer()
{
	//if (Tools::ContainsKey(data, pathToFunc))
	if (data.at(pathToFunc).callCount == 0)
		return; // this happens if Reset gets called between Timer construction and destruction.
	data.at(pathToFunc).durationFunc += Now() - data.at(pathToFunc).start;

}



void Profiler::_ProfileLine(const string& pathToFunc, int lineNumber, const char* lineText)
{
	if (data.at(pathToFunc).callCount == 0)
		return; // this happens if Reset gets called between Timer construction and destruction.
	auto now = Now();
	string line = std::to_string(lineNumber) + ": " + string(lineText);
	data.at(pathToFunc).durationByLine[line] += now - data.at(pathToFunc).last;
	data.at(pathToFunc).last = now;

	
}

void Profiler::LogAndPrint()
{
	string output = _CalculateOutput();
	logger::print(output);

	std::ofstream logFile(Literals::Log + "lastLog.txt");
	if (logFile)
		logFile << output << std::endl;
	else
		RaiseError("Failed to open file. Logging to console instead");
    // logFile is automatically closed, when leaving scope.
}



std::string Column(const std::string& str, uint width)
{
	if (str.length() >= width)
		return str.substr(0, width - 4) + "... | "; // cut off the remainder of the string (-4 makes room for "... ")
	else 
		return str + std::string(width - str.length(), ' ') + "| "; // Append spaces to the original string
}

string str(double value) // to rounded string
{
	int precision = 3;
	if (value < 0)
		precision++; // make room for the minus sign. Ohh, well... we dont need negative numbers.
	
	if (value > std::pow(10, precision))
		// "rounding" a big number like 12345.6 -> 1234 is an error. Instead, we return the number unchanged.
		return std::to_string(value); 
	else
		// rounding a small number like 0.123456 -> 0.12 is desirable.
		return std::to_string(value).substr(0, 2 + precision); // the ekstra 2 are for the 0. prefix
}

string Profiler::_CalculateOutput()
{
	string out = "";
	string next = "\n    ";
	uint colWidth1 = 35;
	uint colWidth2 = 10;
	for (const auto& [func, record] : data)
	{
		int callCount = record.callCount;
		double totalDurationPerCall = record.durationFunc.count() / callCount;

		out += "\n" + func + next;
		if (record.durationByLine.size() > 1)
		{
			out += Column("line", colWidth1) + Column("sec/call", colWidth2) + "percent" + next;
			out += "--------------------------------------------------------" + next;
		}
		for (const auto& [line, duration] : record.durationByLine)
		{
			double durationPerCall = record.durationByLine.at(line).count() / callCount;
			double percent = 100 * durationPerCall / totalDurationPerCall;
			out += Column(line, colWidth1) + Column(str(durationPerCall), colWidth2) + str(percent) + " %" + next;
		}

		out += Column("total", colWidth1) + Column(str(totalDurationPerCall), colWidth2) + "100.0 % (" + str(callCount) + " hits) \n";
	}
	return out;
}



void Profiler::Reset()
{
	//data.clear();
	for (const auto& [func, _] : data)
		data.at(func) = Record(); // clear the record
}