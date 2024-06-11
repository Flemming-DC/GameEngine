#pragma once
#include "Time_.h"
#include "ShortHands.h"

#define ProfileFunc Profiler::FuncTimer funcTimer(std::string(__FILE__) + "." + __func__);
#define ProfileLine Profiler::_ProfileLine(std::string(__FILE__) + "." + __func__, __LINE__);

namespace Profiler
{
	Shorts;

	void Reset(); // called on gamestart/end
	void Print();
	void LogToFile();

	void _ProfileLine(const string& pathToFunc, int codeLine);
	string _CalculateOutput();

	struct FuncTimer
	{
		string pathToFunc;
		FuncTimer(const string& pathToFunc_);
		~FuncTimer();
	};
};

/*

#define ProfileFunc;
#define Profile(codeLine);
#define TimeStamp;
#define ProfileLine; // accumulate time for line and add line to LinesByFunc.
// enable / disable Profiler e.g based of based of InEditor
// send performance-data to console, file or gui. evt. make record of the last N runs or just the last run. 
//		Make window with current data

int foo()
{
	ProfileFunc; // create object that gets dropped at end of func. This way the func-end is detected.
	ProfileLine; int i = 0;
	ProfileLine; int i = 10;
	ProfileLine; int i = 0;
	ProfileLine; int i = 10;
	
}



*/

