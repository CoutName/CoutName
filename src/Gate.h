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
	bool flag1;
	vector<Gate*> Y;
	vector<string> port;
	Gate* A;
	Gate* B;
	
	//for simulate
	int valueA;
	int valueB;
	int valueY;
	int timeA;
	int timeB;
	int timeY;
	bool trueA;
	bool trueB;

	//for bst
	int inValue;
	int inTime;
	Gate* inGate;
	string inPort;
	bool searched;

	//for new hope ->  die
	int Out_test;

};

#endif
