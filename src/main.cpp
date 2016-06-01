#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include"cirMgr.h"
//#include"Gate.h"
using namespace std;
int TIME_C;
int SLACK_C;

int main(){
	CirMgr* cirMgr =new CirMgr;
	string file;
	cout<<"Input file name: ";
	cin>>file;
	if(!cirMgr->read(file)) return 1;
	cout<<"Read successfully!"<<endl;	
	
//	cout<<"Time constraint: ";
//	cin>>TIME_C;
//	cout<<"Slack constraint: ";
//	cin>>SLACK_C;
	
	string str;
	while(true){
		cout<<"p(print)/q(quit)/r(read)/d(dfs)... : ";
		cin>>str;
		if(str=="p") cirMgr->print();
		if(str=="q") break;
		if(str=="r") {
			delete cirMgr;
			cirMgr=new CirMgr;
			cout<<"Input file name: ";
			cin>>file;
			if(!cirMgr->read(file)) break;
			cout<<"Read successfully!"<<endl;
		}
		if(str=="d"){
			cirMgr->_dfsList.clear();
			cirMgr->_pathMap.clear();
			cirMgr->dfs_path();
		}
	}
	delete cirMgr;
	return 0;
}
