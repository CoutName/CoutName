#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include"cirMgr.h"
#include"Gate.h"
using namespace std;
int main(){
	CirMgr* cirMgr =new CirMgr;
	cout<<"Input file name: ";
	string file;
	cin>>file;
	if(!cirMgr->read(file)) return 1;
	cout<<"Read successfully!"<<endl;	
	string str;
	while(true){
		cout<<"p(print)/q(quit)/r(read)... : ";
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
	}
	delete cirMgr;
	return 0;
}
