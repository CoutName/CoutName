#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include"cirMgr.h"
#include"Gate.h"
using namespace std;
int main(){
	CirMgr* cirMgr=0;
	cout<<"Input file name: ";
	string file;
	cin>>file;
	if(!cirMgr->read(file)) return 1;
	/*fstream fin;
	fin.open(file.c_str(),ios::in);
	if(!fin){cout<<"Cannot open file!!"<<endl; return 1;}
	cirMgr->read(file);
	string status;
	string tok;
	while(fin>>tok){	cout<<tok<<endl;} */
	cout<<"Read successfully!"<<endl;	
	return 0;
}
