#ifndef CIR_DEF_H
#define CIR_DEF_H
#include<iostream>
#include<map>
#include"Gate.h"
#include"Path.h"

using namespace std;
typedef vector<Gate*> GateList;
typedef map<string, Gate*> GateMap;
typedef pair<string, Gate*> GatePair;
typedef map<string, Path*> PathMap;
typedef pair<string, Path*> PathPair;

#endif
