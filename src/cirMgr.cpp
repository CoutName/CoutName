#include"cirMgr.h"
using namespace std;

CirMgr::CirMgr(){}
CirMgr::~CirMgr(){}

bool CirMgr::read(string file){
	fstream fin;
	fin.open(file.c_str(),ios::in);
	if(!fin){cout<<"Cannot open file!!"<<endl; return false;}
	
	GateMap wireMap;
	string status="";
	string tok;
	while(fin>>tok){
		//cout<<tok<<endl;
		if( tok=="input" || tok=="output" || tok=="wire" ||
			tok=="NOT1" || tok=="NOR2" || tok=="NAND2")
		{
			status=tok; 
			continue;
		}
				
		if(status=="") continue;
        if(status=="input"){
					
		}
		else if(status=="output"){

		}
		else if(status=="wire"){
	
		}

	}
	return true;
}
	  
