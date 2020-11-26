#include "coins64f.h"

void printh(uint64_t* a, uint64_t len, bool rmv)
{
	cout << "0x";
	if (rmv)
	{
		cout << hex << setw(8) << setfill('0') << *(a + len - 1);
	}
	else
	{
		cout << hex << setw(16) << setfill('0') << *(a + len - 1);
	}
	for (int64_t i = len - 2; i >= 0; i--)
	{
		cout << hex << setw(16) << setfill('0') << *(a + i);
	}

	return;
}

void printkp(uint64_t* k, uint64_t* a)
{
	printh(k, 4, false);
	cout << ":";
	printh(a, 3, true);
	cout << endl;
	return;
}