#include"cirMgr.h"
#include<cstdlib>
#include<time.h>
#include<algorithm>
using namespace std;
extern int TIME_C;
extern int SLACK_C;
extern fstream fout;

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
		Gate* a=w->A;
		a->Y=w->Y;
		a->port=w->port;
		if(w->type=="out"){
			a->Y.push_back(w);
			a->port.push_back("A");
		}
		for(size_t i=0;i<w->Y.size();++i){
			if(w->port[i]=="A") w->Y[i]->A=a;
			else w->Y[i]->B=a;
		}
	}
	for(size_t i=0;i<_poList.size();++i){
		Gate* g=_poList[i];
		g->Y.clear();
	}
	return true;
}

void CirMgr::print(){
	cout<<"gateMap:======================================"<<endl;
	for(GateMap::iterator i=_gateMap.begin();i!=_gateMap.end();++i){
		Gate* g=(*i).second;
		cout<<(*i).first<<" "<<g->type<<endl;
		if(g->A) cout<<"A: "<<g->A->name<<endl;
		if(g->B) cout<<"B: "<<g->B->name<<endl;
		for(int i=0;i<g->Y.size();++i){
			cout<<"Y: "<<g->Y[i]->name<<endl;
		}
	} 
	cout<<"piList:======================================="<<endl;
	for(size_t i=0;i<_piList.size();++i){
		Gate* g=_piList[i];
		cout<<g->name<<" "<<g->type<<endl;
	}
	cout<<"poList:======================================="<<endl;
	for(size_t i=0;i<_poList.size();++i){
		Gate* g=_poList[i];
		cout<<g->name<<" "<<g->type<<endl;
	}
	cout<<"pathMap:======================================"<<endl;
	for(PathMap::iterator i=_pathMap.begin();i!=_pathMap.end();++i){
		cout<<(*i).first<<endl;
	}
	cout<<"Total: "<<_pathMap.size()<<" paths"<<endl;
}

void CirMgr::dfs(){
	for(size_t i=0;i<_poList.size();++i){
		Circuit tmpckt;
		tmpckt.PO = _poList[i];
		build_dfs(_poList[i]);
		build_dfs1(_poList[i],tmpckt);
		for(size_t i=0;i<tmpckt.DFS_list.size();i++)
			tmpckt.DFS_list[i]->flag1 = false;
		_circuits.push_back(tmpckt);
	}
	for(size_t i=0;i<_dfsList.size();++i){
		_dfsList[i]->flag=false;
	}
	
	cout<<"dfsList:======================================"<<endl;
	for(size_t i=0;i<_dfsList.size();++i){
		Gate* g=_dfsList[i];
		cout<<g->name<<" "<<g->type<<endl;
	}
}

void CirMgr::path(){
	
	for(size_t i=0;i<_poList.size();++i){
		//Circuit	tmpckt;
		GateList tmpPath;
		vector<string> tmpPort;
		find_path(_poList[i],tmpPath,tmpPort,TIME_C+1,_circuits[i]);
	}
	cout<<"pathMap:======================================"<<endl;
	for(PathMap::iterator i=_pathMap.begin();i!=_pathMap.end();++i){
		cout<<(*i).first<<endl;
	}
	cout<<"Total: "<<_pathMap.size()<<" paths"<<endl;
}

void CirMgr::falsepath(){
	set_time();
	for(size_t i=0;i<_piList.size();++i){
		Gate* g=_piList[i];
		cout<<g->name<<"(rise)"<<endl;
		g->inValue=0;//rise
		find_falsepath(g);
		cout<<g->name<<"(fall)"<<endl;
		g->inValue=1;//fall
		find_falsepath(g);
	}
	
	cout<<"truepathMap:==================================="<<endl;
	for(PathMap::iterator i=_pathMap.begin();i!=_pathMap.end();++i){
		cout<<(*i).first<<endl;
	}
	cout<<"Total: "<<_pathMap.size()<<" true paths"<<endl;
}

void CirMgr::brute(){
	int truepathNum=0;
	vector<int> simValue(_piList.size()+1,0);
	while(simValue.back()!=1){
		for(size_t i=0;i<_piList.size();++i){ //assign input value
			_piList[i]->valueY=simValue[i];
			_piList[i]->timeY=0;
			//cout<<_piList[i]->name<<" "<<_piList[i]->valueY<<endl;
		}
		simulate();
		if(_pathMap.empty()) break;
		vector<string> tobedeleted;
		for(PathMap::iterator it=_pathMap.begin();it!=_pathMap.end();++it){//print and delete
			Path* path=(*it).second;
			GateList gs=path->gates;
			vector<string> ps=path->ports;
			if((gs.back()->valueY==1 && path->inType=="f") ||
				(gs.back()->valueY==0 && path->inType=="r")) continue;
			bool istruepath=true;
			for(size_t i=1;i<gs.size()-1;++i){
				if((ps[i]=="A" && !gs[i]->trueA) ||
					(ps[i]=="B" && !gs[i]->trueB)){
					istruepath=false;	
					break;
				}
			}
			if(istruepath){
				cout<<"truepath: "<<path->pathKey<<endl;
				tobedeleted.push_back(path->pathKey);
				
				truepathNum++;
				fout<<"Path  {  "<<truepathNum<<"  }"<<endl<<endl;
				path->print();
				fout<<"Input Vector"<<endl<<"{"<<endl;
				string pathInName=path->gates.back()->name;
				for(size_t i=0;i<_piList.size();++i){
					fout<<"  "<<_piList[i]->name<<"  =  ";
					if(pathInName==_piList[i]->name) fout<<path->inType<<endl;
					else fout<<_piList[i]->valueY<<endl;
				}
				fout<<"}"<<endl<<endl;
			}
		}
		for(size_t i=0;i<tobedeleted.size();++i){
			_pathMap.erase(tobedeleted[i]);
		}
		int count=0;
		while(true){
			if(simValue[count]==0){
				simValue[count]=1;
				break;
			}
			else{
				simValue[count]=0;
				++count;
			}
		}
	}
}
void CirMgr::output_test(){
	for(int  i=0; i<_circuits.size(); i++){
		for(int j=0; j<_circuits[i].path_list.size();j++){
			Set_Test(_circuits[i].path_list[j],0);
			Simulate(_circuits[i],_circuits[i].path_list[j]);
			Reset(_circuits[i]);
			Set_Test(_circuits[i].path_list[j],1);
			Simulate(_circuits[i],_circuits[i].path_list[j]);
			Reset(_circuits[i]);
		}		
	}	
	vector<string>	overlap;
	vector<string>::iterator ier;
	for(it = in_ans.begin(); it!=in_ans.end();it++){
		cout<<it->first<<":"<<endl;
		for(size_t i=0;i<it->second.size();i++){
			ier = find(overlap.begin(),overlap.end(),it->second[i]);
			if(ier == overlap.end()){
				cout<<it->second[i]<<endl;
				overlap.push_back(it->second[i]);
			}
		}
	}
	cout<<"Total true path:"<<overlap.size()<<endl;
}
void Reset(Circuit& ckt){
	for(size_t i=0; i<ckt.DFS_list.size();i++){
		ckt.DFS_list[i]->Out_test = 10;
	}
	for(size_t i=0; i<ckt.PI_list.size();i++){
		ckt.PI_list[i]->Out_test =10;
	}
}
void Set_Test(Path* t_path,int set_out){
	int num = 0;
	Gate*	now_gate = t_path -> gates[num];

	if(now_gate->type != "out")	cout<<"ERROR!";
	else{
		now_gate -> Out_test = set_out;
		Trace_Out_test(now_gate,t_path,num);
	}
}

void Trace_Out_test(Gate* g, Path* p,int num){
  	if(g->type == "in")
    	return;
    else if(g->type == "NAND2"){
      	if(g->Out_test == 1)
          	p->gates[num+1] ->Out_test = 0;
   		else if (g -> Out_test == 0){
       		g->A->Out_test = 1;
       		g->B->Out_test = 1;		
       		if( p->gates[num+1] == g->A)
				maybe_in(g -> B);
			else	maybe_in(g -> A);
   		}
	   	else cout<<"ERROR!!"<<endl;
		num++;
       	Trace_Out_test(p -> gates[num],p,num);
    }
    else if (g->type == "NOR2"){
   		if(g->Out_test == 1){
       		g->A->Out_test = 0;
       		g->B->Out_test = 0;
       		if( p->gates[num+1] == g->A)
				maybe_in(g -> B);
			else	maybe_in(g -> A);
   		}
   		else if(g->Out_test==0)
       		p->gates[num+1]->Out_test = 1;
	   	else cout<<"ERROR!!"<<endl;
        num++;
        Trace_Out_test(p -> gates[num],p,num);
   	}
    else if (g->type == "NOT1"){
   		if(g->Out_test == 1)
			p->gates[num+1]->Out_test = 0;
    	else if(g->Out_test == 0)
			p->gates[num+1]->Out_test = 1;
	   	else cout<<"ERROR!!"<<endl;
		num++;
        Trace_Out_test(p -> gates[num],p,num);
	}
	else if(g->type == "out"){ 													//if (type == "OUT")
    	p->gates[num+1]->Out_test = g->Out_test;
		num++;
        Trace_Out_test(p -> gates[num],p,num);
    }
}

void maybe_in(Gate* g){
	if(g->type == "in") return;
	else if(g->type == "NAND2"){
		if(g->Out_test == 0){
			g->A->Out_test = 1;
			maybe_in(g->A);
			g->B->Out_test = 1;
			maybe_in(g->B);
		}
		else return;
	}
	else if(g->type == "NOR2"){
		if(g->Out_test == 1){
			g->A->Out_test = 0;
			maybe_in(g->A);
			g->B->Out_test = 0;
			maybe_in(g->B);
		}
		else return;
	}
	else if(g->type == "NOT1"){
		g->A->Out_test == ~g->Out_test;
		maybe_in(g->A);
	}
}

void CirMgr::Simulate(Circuit& ckt, Path* t_path){
	int count=0;
	vector<Gate*> unsurePI;
	for(size_t i=0; i<ckt.PI_list.size();i++){
		ckt.PI_list[i]->valueY =  ckt.PI_list[i]->Out_test;
		if(ckt.PI_list[i]->valueY == 10){
			count++;
			unsurePI.push_back(ckt.PI_list[i]);
		}
	}
	vector<int> cal_in(count+1,0);
	while(cal_in.back()!=1){
		for(size_t i=0; i<unsurePI.size();i++){
			unsurePI[i]->valueY = cal_in[i];
		}
		
		for(size_t i=0;i<ckt.PI_list.size();++i){ //assign input value
			ckt.PI_list[i]->timeY=0;
		}
		for(size_t i=0;i<ckt.DFS_list.size();++i){ //simulate
			Gate* g = ckt.DFS_list[i];
			g->trueA=g->trueB=false;
			if(g->type=="NOT1") simNOT1(g);
			else if(g->type=="NAND2") simNAND2(g);
			else if(g->type=="NOR2") simNOR2(g);
			else simPO(g);
		}
		
	/*	if(t_path->pathKey == "fA0U15BU13AU1AU12BM1"){
			cout<<t_path->pathKey<<endl;
			for(size_t i=0;i<ckt.PI_list.size();++i){ //assign input value
				cout<<ckt.PI_list[i]->name<<":"<<ckt.PI_list[i]->valueY<<" ";
			}	
			if(delay_conf(t_path)) cout<<"delay wrong"<<endl;
		 	if (ckt.PO->valueY != ckt.PO->Out_test) {		
				cout<<"output wrong!"<<endl;
				cout<<"Y:"<<ckt.PO->valueY<<" O:"<<ckt.PO->Out_test<<endl;
			}
		}
	*/	if(!(delay_conf(t_path) || (ckt.PO->valueY != ckt.PO->Out_test))){
			cout<<"true!!"<<endl;
			string in_pattern = "";
			for(size_t i=0; i<_piList.size();i++){
				if(_piList[i]->valueY)
					in_pattern.append("X");
				else{
					stringstream	ss;
					ss << 	_piList[i]->valueY;
					in_pattern.append(ss.str());
				}
			}
			bool goon = true;
			if(t_path -> inType == "r" && t_path->gates.back()->valueY == 0) goon = false;
			if(t_path -> inType == "f" && t_path->gates.back()->valueY == 1) goon = false;
			if(goon){
				it = in_ans.find(in_pattern);
				if(it != in_ans.end()){
					in_ans[in_pattern].push_back(t_path->pathKey);
				}
				else{
					vector<string> tmp;
					tmp.push_back(t_path->pathKey);
					in_ans.insert(pair<string, vector<string> >(in_pattern, tmp));
				}
		//		cout<<"We success!!!"<<endl;
			}
		}

		int digit=0;
		while(true){
			if(cal_in[digit]==0){
				cal_in[digit]=1;
				break;
			}
			else{
				cal_in[digit]=0;
				++digit;
			}
		}
	}

}
bool delay_conf(Path* p){
	for(size_t i=0;i<p->gates.size()-1;i++){
		bool done=false,dzero=false;
		if(p->gates[i]->valueA == 1 && p->gates[i]->valueB ==1) done = true;
		if(p->gates[i]->valueA == 0 && p->gates[i]->valueB ==0) dzero = true;
		if(p->gates[i]->type == "NOR2"){
			if(p->gates[i]->A->name == p->gates[i+1]->name) {
				if(done) {
					if(p->gates[i]->timeA > p->gates[i]->timeB) return true;
				}
				else if(dzero) {
					if(p->gates[i]->timeA < p->gates[i]->timeB) return true;
				}
			}
			else if(p->gates[i]->B->name == p->gates[i+1]->name) {
				if(done) {
					if(p->gates[i]->timeB > p->gates[i]->timeA) return true;
				}
				else if(dzero) {
					if(p->gates[i]->timeB < p->gates[i]->timeA) return true;
				}

			}
		}
		else if(p->gates[i]->type == "NAND2"){	
			if(p->gates[i]->A->name == p->gates[i+1]->name) {
				if(done) {
					if(p->gates[i]->timeA < p->gates[i]->timeB) return true;
				}
				else if(dzero) {
					if(p->gates[i]->timeA > p->gates[i]->timeB) return true;
				}
			}
			else if(p->gates[i]->B->name == p->gates[i+1]->name) {
				if(done) {
					if(p->gates[i]->timeB < p->gates[i]->timeA) return true;
				}
				else if(dzero) {
					if(p->gates[i]->timeB > p->gates[i]->timeA) return true;
				}
			}
			else continue;
		}
	}
	return false;
}
void CirMgr::simulate(){
	for(size_t i=0;i<_dfsList.size();++i){ 
		Gate* g=_dfsList[i];
		g->trueA=false;
		g->trueB=false;
		if(g->type=="NOT1") simNOT1(g);
		else if(g->type=="NAND2") simNAND2(g);
		else if(g->type=="NOR2") simNOR2(g);
		else simPO(g);
		//cout<<g->name<<"; simValueY: "<<g->valueY<<endl;
	}
}

///////////////////////////////////////////////////////////////////////////
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

void CirMgr::build_dfs1(Gate* g, Circuit& tmpckt){
	if(g->flag1) return ;
	if(g->A) build_dfs1(g->A, tmpckt);
	if(g->B) build_dfs1(g->B, tmpckt);
	if(g->flag1==false){
		g->flag1=true;
		if(g->type!="in"){
			tmpckt.DFS_list.push_back(g);
			cout<<"push_back DFS"<<endl;
		}
	}
	return ;
}
Gate* CirMgr::build_dfs(Gate* g){
	if(g->flag) return g;
	if(g->A) build_dfs(g->A);
	if(g->B) build_dfs(g->B);
	if(g->flag==false){
		g->flag=true;
		if(g->type!="in"){
			_dfsList.push_back(g);
		}
	}
	return g;
}

Gate* CirMgr::find_path(Gate* g, GateList& tmppath, vector<string>& port, int slack, Circuit& tmpckt){
	if(slack<0) return g;
	
	tmppath.push_back(g);
	if(g->A){
		port.push_back("A");
		find_path(g->A, tmppath, port, slack-1, tmpckt);
	}
	if(g->B){
		port.push_back("B");
		find_path(g->B, tmppath, port, slack-1, tmpckt);
	}
	if(g->type=="in") {
		port.push_back("in");
		if(slack<SLACK_C){
			Path* rpath=new Path(tmppath,port,"r");
			_pathMap.insert(PathPair(rpath->pathKey, rpath));
			tmpckt.path_list.push_back(rpath);
			Path* fpath=new Path(tmppath,port,"f");
			_pathMap.insert(PathPair(fpath->pathKey, fpath));
			tmpckt.path_list.push_back(fpath);
			//Path* tpath=new Path(tmppath,port,"");
			//tmpckt.path_list.push_back(tpath);
		}
		bool alreadyin = false;
		for(int i=0;i<tmpckt.PI_list.size();i++){
			if(g->name == tmpckt.PI_list[i]->name){
				alreadyin = true;
				break;
			}
		}
		if(!alreadyin) tmpckt.PI_list.push_back(g);
		port.pop_back();
	}
	tmppath.pop_back();
	if(!port.empty()) port.pop_back();
	return g;

}
void CirMgr::set_time(){
	for(size_t i=0;i<_piList.size();++i){
		_piList[i]->timeY=0;		
	}
	for(size_t i=0;i<_dfsList.size();++i){
		Gate* g=_dfsList[i];
		g->timeA=g->A->timeY;
		if(g->type=="out") continue;
		if(g->type=="NOT1") {g->timeY=g->timeA+1; continue;}
		g->timeB=g->B->timeY;
		g->timeY=(g->timeA>=g->timeB?g->timeB:g->timeA)+1;
	}
}

//bfs find falsepath 
void CirMgr::find_falsepath(Gate* s){
	s->inTime=-1;
	s->searched=true;
	GateList allsearched;
	GateList Q;
	GateList tmpfalse;
	GateList falsegate;
	allsearched.push_back(s);
	Q.push_back(s);

	while(!Q.empty()){
		Gate* u=Q.front();
		if(u->inTime>TIME_C){
			Q.erase(Q.begin());
			continue;
		}
		if((u->inValue==0 && u->type=="NOR2") || 
			(u->inValue==1 &&u->type=="NAND2")){
			if(u->inPort=="A"){
				if(u->timeA<u->timeB) falsegate.push_back(u);
			}
			else{
				if(u->timeB<u->timeA) falsegate.push_back(u);
			}
		}
		else if(u->inValue!=0 && u->inValue!=1){
			u->inValue=5;
		}
		else u->flag=true;
		
		for(size_t i=0;i<u->Y.size();++i){
			Gate* v=u->Y[i];
			v->inGate=u;
			v->inPort=u->port[i];
			if(v->searched){
				tmpfalse.push_back(v);
				continue;
			}
			v->inValue=abs(u->inValue-1);
			v->inGate=u;
			v->inPort=u->port[i];
			v->searched=true;
			v->inTime=u->inTime+1;
			Q.push_back(v);
			allsearched.push_back(v);
		}
		Q.erase(Q.begin());
	}
	for(size_t i=0;i<tmpfalse.size();++i){
		Gate* g=tmpfalse[i];
		int value=abs(g->inGate->inValue-1);
		if(!g->flag){
			if(isControl(g,value)){
				changeInPort(g);
				falsegate.push_back(g);					
			}
		}
		else{
			if(isControl(g,value) && (g->inGate->inTime+1 == g->inTime))
				continue;
			falsegate.push_back(g);
		}
	}
	
	cout<<"false gate: "<<endl;
	for(size_t i=0;i<falsegate.size();++i){
		cout<< falsegate[i]->name<<falsegate[i]->inPort<<endl;
		delete_falsepath(falsegate[i]);
	}
	for(size_t i=0;i<allsearched.size();++i){
		allsearched[i]->flag=false;	
		allsearched[i]->searched=false;
	}
}

void CirMgr::delete_falsepath(Gate* g){
	string pathName=pathToInput(g);
	pathToOutput(g,pathName);
}

string CirMgr::pathToInput(Gate* g){
	string str="";
	while(g->inGate){
		str=g->name+g->inPort+str;
		g=g->inGate;
	}
	str=g->name+str;
	if(g->inValue==0) str="r"+str;
	else str="f"+str;
	return str;
}

Gate* CirMgr::pathToOutput(Gate* g, string str){
	for(size_t i=0;i<g->Y.size();++i){
		Gate* y=g->Y[i];
		str=str+y->name;
		if(y->type=="out"){
		//	cout<<"delete: "<<str<<endl;
			_pathMap.erase(str);
			return y;
		}
		str=str+g->port[i];
		pathToOutput(y, str);
	}
}

bool isControl(Gate* g, int v){
	return ((v==0 && g->type=="NAND2") || (v==1 && g->type=="NOR2"));
}

void changeInPort(Gate* g){
	if(g->inPort=="A"){
		g->inPort="B";
		g->inGate=g->B;
	}
	else{
		g->inPort="A";
		g->inGate=g->A;
	}
}

void simNOT1(Gate* g){
	g->valueA=g->A->valueY;
	g->timeA=g->A->timeY;
	g->trueA=true;
	g->valueY=abs(g->valueA-1);
	g->timeY=g->timeA+1;
}

void simNAND2(Gate* g){
	g->valueA=g->A->valueY;
	g->valueB=g->B->valueY;
	g->timeA=g->A->timeY;
	g->timeB=g->B->timeY;
	if(g->timeA==g->timeB){
		g->timeY=g->timeA+1;
		if(g->valueA==g->valueB){
			g->trueA=true; g->trueB=true;
			g->valueY=abs(g->valueA-1);
		}
		else{
			if(g->valueA==0) g->trueA=true;
			else g->trueB=true;
			g->valueY=1;
		}
	}
	else if(g->timeA<g->timeB){
		if(g->valueA==1){
			g->trueB=true;
			g->valueY=abs(g->valueB-1);
			g->timeY=g->timeB+1;
		}
		else{
			g->trueA=true;
			g->valueY=1;
			g->timeY=g->timeA+1;
		}
	}
	else{
		if(g->valueB==1){
			g->trueA=true;
			g->valueY=abs(g->valueA-1);
			g->timeY=g->timeA+1;
		}
		else{
			g->trueB=true;
			g->valueY=1;
			g->timeY=g->timeB+1;
		}
	}
}

void simNOR2(Gate* g){
	g->valueA=g->A->valueY;
	g->valueB=g->B->valueY;
	g->timeA=g->A->timeY;
	g->timeB=g->B->timeY;
	if(g->timeA==g->timeB){
		g->timeY=g->timeA+1;
		if(g->valueA==g->valueB){
			g->trueA=true; g->trueB=true;
			g->valueY=abs(g->valueA-1);
		}
		else{
			if(g->valueA==1) g->trueA=true;
			else g->trueB=true;
			g->valueY=0;
		}
	}
	else if(g->timeA<g->timeB){
		if(g->valueA==0){
			g->trueB=true;
			g->valueY=abs(g->valueB-1);
			g->timeY=g->timeB+1;
		}
		else{
			g->trueA=true;
			g->valueY=0;
			g->timeY=g->timeA+1;
		}
	}
	else{
		if(g->valueB==0){
			g->trueA=true;
			g->valueY=abs(g->valueA-1);
			g->timeY=g->timeA+1;
		}
		else{
			g->trueB=true;
			g->valueY=0;
			g->timeY=g->timeB+1;
		}
	}
}

void simPO(Gate* g){
	g->timeA=g->A->timeY;
	g->valueY=g->valueA=g->A->valueY;
}
