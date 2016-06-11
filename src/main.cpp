#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include"cirMgr.h"
using namespace std;
int TIME_C;
int SLACK_C;
fstream fout;

int main(){
	CirMgr* cirMgr =new CirMgr;
	string file;
	cout<<"Input file name: ";
	cin>>file;
	if(!cirMgr->read(file)) return 1;
	cout<<"Read successfully!"<<endl;	
	string outputfile=file+"_true_path_set";
	fout.open(outputfile.c_str(),ios::out);
	fout<<"Header  {  A True Path Set  }"<<endl<<endl;
	fout<<"Benchmark  {  "<<file<<"  }"<<endl<<endl;
	

	cout<<"Time constraint: ";
	cin>>TIME_C;
	cout<<"Slack constraint: ";
	cin>>SLACK_C;
	
	string str;
	while(true){
		cout<<"p(print)/q(quit)/r(read)/d(dfs)/pa(path)/f(findfalse)/b(bruteforce)... : ";
		cin>>str;
		if(str=="p") cirMgr->print();
		if(str=="q") {fout.close();break;}
		if(str=="r") {
			fout.close();
			delete cirMgr;
			cirMgr=new CirMgr;
			cout<<"Input file name: ";
			cin>>file;
			if(!cirMgr->read(file)) break;
			cout<<"Read successfully!"<<endl;
			outputfile=file+"_true_path_set";
			
			cout<<"Time constraint: ";
			cin>>TIME_C;
			cout<<"Slack constraint: ";
			cin>>SLACK_C;
			fout.open(outputfile.c_str(),ios::out);
		}
		if(str=="d"){
			cirMgr->_dfsList.clear();
			cirMgr->dfs();
		}
		if(str=="pa"){
			cirMgr->path();
		}
		if(str=="f"){
			cirMgr->falsepath();
		}
		if(str=="b"){
			cirMgr->brute();
		}
	}
	delete cirMgr;
	return 0;
}
