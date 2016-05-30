#ifndef GATE_H
#define GATE_H
#include<iostream>
#include<string>
#include<vector>
using namespace std;
class Gate
{
public:
    Gate(string, string);
	~Gate();
    string name;
	string type;

	vector<Gate*> Y;
	Gate* A;
	Gate* B;
};

#endif
