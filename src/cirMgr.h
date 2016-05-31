#ifndef CIR_MGR_H
#define CIR_MGR_H

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include"Gate.h"
using namespace std;

class CirMgr
{
typedef map<string, Gate*> GateMap;
typedef pair<string, Gate*> GatePair;
typedef vector<Gate*> GateList;

public:
	CirMgr();
	~CirMgr();
	bool read(string);
	void print();

	GateMap _gateMap;
	GateList _piList;
	GateList _poList;
};

string wireName(string);

#endif
