#include<iomanip>
#include<fstream>
#include<cstdlib>
#include"Path.h"
using namespace std;
extern fstream fout;
extern int TIME_C;

Path::Path(vector<Gate*> g, vector<string> p, string t):gates(g), ports(p), inType(t){
	pathKey=gates.front()->name;
	for(size_t i=1;i<gates.size()-1;++i){
		pathKey=ports[i]+pathKey;
		pathKey=gates[i]->name+pathKey;
	}
	pathKey=gates.back()->name+pathKey;
	pathKey=inType+pathKey;
}
Path::~Path(){}

void Path::print(){
	int delay=0;
	fout<<"A True Path List"<<endl<<"{"<<endl;
	fout<<"---------------------------------------------------------------------------"<<endl;
	fout<<"Pin    type                                Incr        Path delay"<<endl;
	fout<<"---------------------------------------------------------------------------"<<endl;
	
	size_t length=gates.back()->name.length()+5;
	fout<<gates.back()->name<<" (in)";
	fout<<right<<setw(45-length)<<0<<right<<setw(11)<<delay<<" "<<(gates.back()->valueY==1?"r":"f")<<endl;
	
	for(size_t i=gates.size()-2;i>0;--i){
		Gate* g=gates[i];
		if(g->type=="NAND2") length=g->name.length()+10;
		else length=g->name.length()+9;
		fout<<g->name<<"/"<<ports[i]<<" ("<<g->type<<")";
		fout<<right<<setw(45-length)<<0<<right<<setw(11)<<delay<<" "<<(abs(g->valueY-1)==1?"r":"f")<<endl;
		fout<<g->name<<"/"<<"Y"<<" ("<<g->type<<")";
		fout<<right<<setw(45-length)<<1<<right<<setw(11)<<++delay<<" "<<(g->valueY==1?"r":"f")<<endl;
	}
	
	length=gates.front()->name.length()+6;
	fout<<gates.front()->name<<" (out)";
	fout<<right<<setw(45-length)<<0<<right<<setw(11)<<delay<<" "<<(gates.front()->valueA==1?"r":"f")<<endl;
	
	fout<<"---------------------------------------------------------------------------"<<endl;
	fout<<"Data Required Time"<<right<<setw(14)<<TIME_C<<endl;
	fout<<"Data Arrival Time"<<right<<setw(15)<<delay<<endl;
	fout<<"---------------------------------------------------------------------------"<<endl;
	fout<<"Slack"<<right<<setw(27)<<TIME_C-delay<<endl<<"}"<<endl<<endl;
}
