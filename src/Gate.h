#ifndef GATE_H
#define GATE_H
#include<iostream>
#include<string>
#include<vector>
#include<map>
using namespace std;
class Gate
{
public:
    Gate(string n, string t);
	~Gate();
    string name;
	string type;
	bool flag;

	vector<Gate*> Y;
	vector<string> port;
	Gate* A;
	Gate* B;
};

#endif
