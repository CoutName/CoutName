#ifndef CIR_MGR_H
#define CIR_MGR_H

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include"cirDef.h"
#include"Path.h"
using namespace std;

class CirMgr
{
public:
	CirMgr();
	~CirMgr();
	
	bool read(string);
	void print();
	void dfs();
	void path();
	void brute();
	void simulate();
	
	GateMap _gateMap;
	PathMap _pathMap;
	GateList _piList;
	GateList _poList;
	GateList _dfsList;

	Gate* build_dfs(Gate*);
	Gate* find_path(Gate*, GateList&, vector<string>&, int);
};

string wireName(string);
void simNOT1(Gate*);
void simNAND2(Gate*);
void simNOR2(Gate*);
void simPO(Gate*);

#endif
