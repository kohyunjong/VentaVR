#pragma once
#include <vector>

class QLicense
{
public:
	/*QLicense();
	~QLicense();*/
	
	int serverTime(int &year, int &month, int &day);
	int checkKeyScope(int &id, int &year, int &month, int &day);
	int checkMacAddress(int type);
};

