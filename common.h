/*
 * common.h
 *
 *  Created on: 11-Jun-2014
 *      Author: sp
 */

#ifndef COMMON_H_
#define COMMON_H_

using namespace std;

struct q_element {
	vector<vector<int> > formula;
	vector<int> assignments;
	vector<int> unknowns;
};

void printQElement(struct q_element q){
	cout<<"Formula="<<endl;
	for(int i=0;i<q.formula.size();i++){
		for(int j=0;j<q.formula[i].size();j++){
			cout<<q.formula[i][j]<<" ";
		}cout<<endl;
	}
	cout<<"Assignments"<<endl;
	for(int i=0;i<q.assignments.size();i++){
		cout<<q.assignments[i]<<" ";
	}cout<<endl;

	cout<<"Unknowns"<<endl;
	for(int i=0;i<q.unknowns.size();i++){
		cout<<q.unknowns[i]<<" ";
	}cout<<endl;
}


inline void print2dVector(vector<vector<int> > v) {
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].size(); j++) {
			cout<<v[i][j]<<" ";
		} cout<<endl;
	}
}

inline void printVector(vector<int> v) {
	for (int i = 0; i < v.size(); i++) {
		cout<<v[i]<<" ";
	} cout<<endl;
}

inline void printPairs(vector<pair<int, int> > v) {
	for (int i = 0; i < v.size(); i++) {
		cout << v[i].first << " " << v[i].second << endl;
	}
}


#endif /* COMMON_H_ */
