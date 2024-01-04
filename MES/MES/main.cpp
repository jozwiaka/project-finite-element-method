#include "Run.h"
#include "Data.h"
#include <iostream>
#include "Grid.h"
#include "Timer.h"

int main()
{
	Timer::timeThread();
	Run run;
	run.run(Path::Data_EXE);
	//run.run(Path::Data_ACS);
	//run.run(Path::Data_MES);
}
