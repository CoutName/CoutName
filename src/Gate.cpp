#include"Gate.h"
using namespace std;
Gate::Gate(string n,string t):name(n),type(t){ 
	A=0; B=0;  inGate=0;
	flag=false; searched=false;
	trueA=false; trueB=false;
}
Gate::~Gate(){}
