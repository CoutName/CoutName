#ifndef PATH_H
#define PATH_H
#include<iostream>
#include<vector>
#include<string>
#include"Gate.h"
using namespace std;

class Path{
public:
	Path(vector<Gate*> g, vector<string> p, string t);
	~Path();
	
	vector<Gate*> gates;
	vector<string> ports;
	string inType;
	string pathKey;
	int slack;
};











#endif
