#include"Gate.h"
using namespace std;
Gate::Gate(string n,string t):name(n),type(t){ 
	A=0; B=0;  inGate=0;
	flag=false; flag1=false; searched=false;
	trueA=false; trueB=false;
	Out_test = 10;
}
Gate::~Gate(){}
