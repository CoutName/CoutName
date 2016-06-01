#include"Path.h"
using namespace std;

Path::Path(vector<Gate*> g, vector<string> p, string t):gates(g), ports(p), inType(t){
	pathKey=inType;
	for(size_t i=0;i<gates.size();++i){
		pathKey+=gates[i]->name;
		if(gates[i]->type!="in" && gates[i]->type!="out")
			pathKey+=ports[i];
	}
}
Path::~Path(){}
