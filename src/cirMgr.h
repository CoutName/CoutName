#ifndef CIR_MGR_H
#define CIR_MGR_H

#include<sstream>
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
	void falsepath();
	void brute();
	void simulate();
	void output_test();
	
	GateMap _gateMap;
	PathMap _pathMap;
	GateList _piList;
	GateList _poList;
	GateList _dfsList;
	

	////////////////////
	vector<Circuit>	_circuits;
	map< string,vector<string> >	in_ans;
	map< string,vector<string> >::iterator	it;
	void Simulate(Circuit& , Path*);

	////////////////////
	Gate* build_dfs(Gate*);
	void  build_dfs_1(Gate*, Circuit&);
	Gate* find_path(Gate*, GateList&, vector<string>&, int, Circuit&);
	void set_time();
	void find_falsepath(Gate*);
	void delete_falsepath(Gate*);
	string pathToInput(Gate*);
	Gate* pathToOutput(Gate*, string);
};

string wireName(string);
bool isControl(Gate*, int);
void changeInPort(Gate*);
void simNOT1(Gate*);
void simNAND2(Gate*);
void simNOR2(Gate*);
void simPO(Gate*);

void Set_Test(Path* ,int );
void Reset(Circuit& );
bool delay_conf(Path* );
void Trace_Out_test(Gate* , Path* ,int );
void maybe_in(Gate*);
#endif
