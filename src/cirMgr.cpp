#include"cirMgr.h"
using namespace std;

CirMgr::CirMgr(){}
CirMgr::~CirMgr(){}

bool CirMgr::read(string file){
	fstream fin;
	fin.open(file.c_str(),ios::in);
	if(!fin){cout<<"Cannot open file!!"<<endl; return false;}
	
	GateMap wireMap;
	bool start=false;
	string status="";
	string tok;
	string name;
	int line;
	while(fin>>tok){
		if(tok=="module") start=true;
		if(!start) continue;
		if( tok=="input" || tok=="output" || tok=="wire" ||
			tok=="NOT1" || tok=="NOR2" || tok=="NAND2"){
			if(tok=="input") status="in";
			else if(tok=="output") status="out";
			else status=tok;
			line=1;
			continue;
		}
				
		if(status=="") continue;
        if(status=="in"){
			name=wireName(tok);
			Gate* g= new Gate(name,status);
			wireMap.insert(GatePair(name,g));
			_gateMap.insert(GatePair(name,g));
			_piList.push_back(g);
		}
		else if(status=="out"){
			name=wireName(tok);
			Gate* g= new Gate(name,status);
			wireMap.insert(GatePair(name,g));
			_gateMap.insert(GatePair(name,g));
			_poList.push_back(g);
		}
		else if(status=="wire"){
			name=wireName(tok);
			Gate* g= new Gate(name,status);
			wireMap.insert(GatePair(name,g));
		}
		else{ //NOT1. NOR2. NAND2 
			if(line==1){ 
				name=tok;
				Gate* g= new Gate(name,status);
				_gateMap.insert(GatePair(name,g));
				++line; continue; 
			}
			if(tok=="endmodule") break;
			if(tok=="(" || tok==");"){ ++line; continue; }
			string head=tok.substr(1,1);
			string content=tok.substr(3,tok.find(")")-3);
			
			Gate* n= (*wireMap.find(content)).second;
			Gate* gate= (*_gateMap.find(name)).second;
			if(head=="A"){
				n->Y.push_back(gate);
				n->port.push_back(head);
				if(n->type=="in") gate->A=n;
			}
			else if(head=="B"){
				n->Y.push_back(gate);
				n->port.push_back(head);
				if(n->type=="in") gate->B=n;
			}
			else{//head==Y
				n->A=gate;
				if(n->type=="out"){
					gate->Y.push_back(n);
					gate->port.push_back("A");
				}
			}
			++line;
		}
	}
	for(GateMap::iterator i=wireMap.begin();i!=wireMap.end();++i){
		Gate* w=(*i).second;
		if(w->type=="in") continue;
		if(w->type=="out") continue;
		Gate* a=w->A;
		a->Y=w->Y;
		a->port=w->port;
		for(size_t i=0;i<w->Y.size();++i){
			if(w->port[i]=="A") w->Y[i]->A=a;
			else w->Y[i]->B=a;
		}
	}
	return true;
}
void CirMgr::print(){
	/*cout<<"gateMap:"<<endl;
	for(GateMap::iterator i=_gateMap.begin();i!=_gateMap.end();++i){
		Gate* g=(*i).second;
		cout<<(*i).first<<" "<<g->type<<endl;
		if(g->A) cout<<"A: "<<g->A->name<<endl;
		if(g->B) cout<<"B: "<<g->B->name<<endl;
		for(int i=0;i<g->Y.size();++i){
			cout<<"Y: "<<g->Y[i]->name<<endl;
		}
	}*/
	cout<<"piList:"<<endl;
	for(size_t i=0;i<_piList.size();++i){
		Gate* g=_piList[i];
		cout<<g->name<<" "<<g->type<<endl;
	}
	cout<<"poList:"<<endl;
	for(size_t i=0;i<_poList.size();++i){
		Gate* g=_poList[i];
		cout<<g->name<<" "<<g->type<<endl;
	}
	cout<<"dfsList:"<<endl;
	for(size_t i=0;i<_dfsList.size();++i){
		Gate* g=_dfsList[i];
		cout<<g->name<<" "<<g->type<<endl;
	}
	cout<<"pathMap:"<<endl;
	for(PathMap::iterator i=_pathMap.begin();i!=_pathMap.end();++i){
		cout<<(*i).first<<endl;
	}
	
}

void CirMgr::dfs_path(){
	for(size_t i=0;i<_poList.size();++i){
		GateList tmpPath;
		vector<string> tmpPort;
		build_dfs_and_path(_poList[i],tmpPath,tmpPort);
	}
}
Gate* CirMgr::build_dfs_and_path(Gate* g, GateList& tmppath, vector<string>& port){
	tmppath.insert(tmppath.begin(),g);
	if(g->A){
		port.insert(port.begin(),"A");
		build_dfs_and_path(g->A, tmppath, port);
	}
	if(g->B){
		port.insert(port.begin(),"B");
		build_dfs_and_path(g->B, tmppath, port);
	}
	if(g->type=="in") {
		port.insert(port.begin(),"in");
		Path* rpath=new Path(tmppath,port,"r");
		_pathMap.insert(PathPair(rpath->pathKey, rpath));
		Path* fpath=new Path(tmppath,port,"f");
		_pathMap.insert(PathPair(fpath->pathKey, fpath));
	}
	if(g->flag==false){
		g->flag=true;
		_dfsList.push_back(g);
	}
	tmppath.erase(tmppath.begin());
	port.erase(port.begin());
	return g;

return 0;
}
	 
string wireName(string str){
	string sstr;
	if(str.find(",")!=string::npos){
		sstr=str.substr(0,str.find(","));
	}
	else{
		sstr=str.substr(0,str.find(";"));
	}
	return sstr;
}


