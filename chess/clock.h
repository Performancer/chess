#pragma once
#include <sys\timeb.h> 

struct timeb start, end;

void startClock()
{
	ftime(&start);
}

void stopClock(int* seconds, int* militm)
{
	ftime(&end);

	*seconds = end.time - start.time;
	*militm = end.millitm - start.millitm;

	if (*militm < 0)
	{
		*militm += 1000;
		*seconds--;
	}
}
