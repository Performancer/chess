#pragma once
#include <sys\timeb.h> 

struct Clock
{
private:
	struct timeb _start;
public:
	void start()
	{
		ftime(&_start);
	}

	int getSeconds() 
	{ 
		struct timeb end;
		ftime(&end);

		int seconds = end.time - _start.time;
		int militm = end.millitm - _start.millitm;

		if (militm < 0)
		{
			militm += 1000;
			seconds--;
		}

		return seconds;
	}

	int getMilliseconds()
	{
		struct timeb end;
		ftime(&end);

		int millitm = end.millitm - _start.millitm;

		if (millitm < 0)
			millitm += 1000;

		return millitm;
	}
};
