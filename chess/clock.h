#pragma once
#include <sys\timeb.h> 

struct Clock
{
private:
	struct timeb _start, _end;
	int _seconds, _militm;

public:
	void start()
	{
		ftime(&_start);
	}

	void stop()
	{
		ftime(&_end);

		_seconds = _end.time - _start.time;
		_militm = _end.millitm - _start.millitm;

		if (_militm < 0)
		{
			_militm += 1000;
			_seconds--;
		}
	}

	int getSeconds() { return _seconds; }
	int getMilliseconds() { return _militm; }
};
