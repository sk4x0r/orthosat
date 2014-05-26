/*
 * main.cpp
 *
 *  Created on: 13-May-2014
 *      Author: sp
 */

#include<iostream>
#include<cstdio>
#include<stdlib.h>
#include "util.h"
#include "solver.h"
using namespace std;
int main(int argc, char** argv){
	if (argc != 2){
		cout<<"Usage: solver <path_to_dimacs_file>\n";
		return -1;
	}

	char * in;
	long length;
	FILE * f = fopen (argv[1], "rb");

	if (f){
	  fseek (f, 0, SEEK_END);
	  length = ftell (f);
	  fseek (f, 0, SEEK_SET);
	  in = (char*)malloc(length);
	  if (in){
		fread (in, 1, length-1, f);
		in[length-1]=-1;
	  }else{
		  cout<<"Fatal Error: Could not allocate memory buffer of length "<<length<<endl;
		  fclose(f);
		  return -1;
	  }
	  fclose (f);
	}else{
		cout<<"Fatal Error: Could not read from file.";
		return -1;
	}
	vector<vector<int> > formula;
	int vars,clauses;
	parseDimacs(in,formula,&vars, &clauses);
	cout<<"vars="<<vars<<endl<<"clauses="<<clauses<<endl;
	vector<int> unknowns;
	for(int i=1;i<=vars;i++){
		unknowns.push_back(i);
	}
	cout<<"unknowns="<<endl;
	for(int i=0;i<unknowns.size();i++){
		cout<<unknowns[i]<<" ";
	}cout<<endl;
	timestamp_t t0 = get_timestamp();
	if(solve(formula,unknowns)){
		cout<<"SATISFIABLE"<<endl;
	}else{

		cout<<"UNSATISFIABLE"<<endl;
	}
	timestamp_t t1 = get_timestamp();
	double secs = (t1 - t0) / 1000000.0L;
	cout<<"Time:"<<secs<<" secs"<<endl;
}
