#ifndef CIR_MGR_H
#define CIR_MGR_H

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
//#include<map>
#include"cirDef.h"
//#include"Gate.h"
#include"Path.h"
using namespace std;

class CirMgr
{
public:
	CirMgr();
	~CirMgr();
	bool read(string);
	void print();
	void dfs_path();
	GateMap _gateMap;
	PathMap _pathMap;
	GateList _piList;
	GateList _poList;
	GateList _dfsList;
	
	Gate* build_dfs_and_path(Gate*,GateList&,vector<string>&);
};

string wireName(string);

#endif
