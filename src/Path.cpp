#include"Path.h"
using namespace std;


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
