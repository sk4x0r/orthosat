/*
 * solver.h
 *
 *  Created on: 14-May-2014
 *      Author: sp
 */
#include <deque>
#include<algorithm>
#include<math.h>
#ifndef SOLVER_H_
#define SOLVER_H_

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

//TODO: write similar function to find element in sorted list
inline bool isPresentIn(vector<int> &v, int n) {
	//cout<<"inside isPresentIn"<<endl;
	for (int i = 0; i < v.size(); i++) {
		if (v[i] == n) {
			return true;
		}
	}
	return false;
}

inline int findIndexOf(vector<int> &v, int n){
	for(int i=0;i<v.size();i++){
		if(v[i]==n){
			return i;
		}
	}
	cout<<"Fatal error in findIndexOf: n not found in v"<<endl;
	return -1;
}

inline int findMin(vector<int> &array) {
	int smallest = array[0];
	for (int i = 0; i < array.size(); i++) {
		if (array[i] < smallest) {
			smallest = array[i];
		}
	}
	return smallest;
}

inline void evaluateQuotient(vector<vector<int> > &formula, vector<int> &assignments, vector<vector<int> > &fIter) {
	fIter=vector<vector<int> >(formula);
	int removedClauses = 0;
	for (int i = 0; i < assignments.size(); i++) {
		int assignment = assignments[i];
		for (int j = fIter.size() - 1; j >= 0; j--) {
			for (int k = fIter[j].size() - 1; k >= 0; k--) {
				if (assignment + fIter[j][k] == 0) {
					fIter[j]=fIter.back();
					fIter.pop_back();
					break;
				} else if (assignment == fIter[j][k]) {
					if (fIter[j].size() <= 1) {
						fIter[j].clear();
					} else {
						if(k!=fIter[j].size()-1)
							fIter[j][k]=fIter[j].back();
						fIter[j].pop_back();
					}
				}
			}
		}
	}
}

inline void evaluateQuotientInPlace(vector<vector<int> > &formula, vector<int> &assignments) {
	int removedClauses = 0;
	for (int i = 0; i < assignments.size(); i++) {
		int assignment = assignments[i];
		for (int j = formula.size() - 1; j >= 0; j--) {
			for (int k = formula[j].size() - 1; k >= 0; k--) {
				if (assignment + formula[j][k] == 0) {
					formula[j]=formula.back();
					formula.pop_back();
					break;
				} else if (assignment == formula[j][k]) {
					if (formula[j].size() <= 1) {
						formula[j].clear();
					} else {
						if(k!=formula[j].size()-1)
							formula[j][k]=formula[j].back();
						formula[j].pop_back();
					}
				}
			}
		}
	}
}

inline void applyUnitPropogation(vector<vector<int> > &formula, vector<int> &assignments, vector<int> &unknowns) {
	vector<int> alreadyRemoved;
	vector<int> propogatedLiterals;
	for (int i = 0; i < formula.size(); i++) {
		if (formula[i].size() == 1) {
			if (isPresentIn(alreadyRemoved, abs(formula[i][0]))) {
				continue;
			}
			alreadyRemoved.push_back(abs(formula[i][0]));
			assignments.push_back(formula[i][0]);
			int idx=findIndexOf(unknowns, abs(formula[i][0]));
			unknowns[idx]=unknowns.back();
			unknowns.pop_back();
			propogatedLiterals.push_back(-1*formula[i][0]);
		}
	}
	evaluateQuotientInPlace(formula, propogatedLiterals);
}

inline void generateOnSet(vector<int> &unknowns, vector<vector<int> > &onSet) {
	onSet.clear();
	int minCount = findMin(unknowns);
	for (int i = 0; i < unknowns.size(); i++) {
		int count = unknowns[i];
		vector<int> term;
		for (int j = minCount; j < count; j++) {
			term.push_back((-1) * j);
		}
		term.push_back(count);
		onSet.push_back(term);
	}
	//create last term
	vector<int> lastTerm = vector<int>();
	for (int j = 0; j < unknowns.size(); j++) {
		lastTerm.push_back((-1) * unknowns[j]);
	}
	onSet.push_back(lastTerm);
}

inline void generateOnSet2(vector<int> &unknowns, vector<vector<int> > &onSet){
	onSet.clear();
	int size=3;
	int power=pow(2,size);
	if(unknowns.size()<size){
		size=unknowns.size();
		power=size*size;
	}
	for (int i = 0; i < power; i++) {
		int temp = i;
		vector<int> term;
		for (int j = 0; j < size; j++) {
			if (temp % 2 == 0) {
				term.push_back(unknowns[j]);
			} else {
				term.push_back(-1 * unknowns[j]);
			}
			temp = temp / 2;
		}
		onSet.push_back(term);
	}
}

inline void findUnassignedVars(vector<int> &term, vector<int> &varList,vector<int> &unassignedVars) {
	unassignedVars.clear();
	for (int i = 0; i < varList.size(); i++) {
		bool found = false;
		for (int j = 0; j < term.size(); j++) {
			if (varList[i] == term[j] || varList[i] + term[j] == 0) {
				found = true;
				break;
			}
		}
		if (!found) {
			unassignedVars.push_back(varList[i]);
		}
	}
}

inline bool checkUnsatisfiable(vector<vector<int> > &formula) {
	for (int i = 0; i < formula.size(); i++) {
		if (formula[i].size() == 0) {
			return true;
		}
	}
	return false;
}

inline bool checkSolution(vector<vector<int> > &formula, vector<int> &assignments) {
	vector<vector<int> >fIter;
	evaluateQuotientInPlace(formula, assignments);
	if (formula.size()==0){
		return true;
	}
	return false;
}

inline void applyPureLiteralRule(vector<vector<int> > &formula, vector<int> &assignments, vector<int> &unknowns) {
	vector<int> pureLiterals;
	for (int i = 0; i < unknowns.size(); i++) {
		bool literalExists = false;
		bool complementExists = false;
		for (int j = 0; j < formula.size(); j++) {
			if (isPresentIn(formula[j], unknowns[i])) {
				literalExists = true;
				if (complementExists) {
					break;
				}
			}
			if (isPresentIn(formula[j], -1 * (unknowns[i]))) {
				complementExists = true;
				if (literalExists) {
					break;
				}
			}
		}
			if (literalExists && !complementExists) {
				pureLiterals.push_back(unknowns[i]);
			}else if (!literalExists && complementExists) {
				pureLiterals.push_back(-1*unknowns[i]);
			}
		}
	for(int i=0;i<pureLiterals.size();i++){
		assignments.push_back(-1*pureLiterals[i]);
		unknowns.erase(remove(unknowns.begin(), unknowns.end(), abs(pureLiterals[i])), unknowns.end());
		pureLiterals[i]=-1*pureLiterals[i];
	}
	evaluateQuotientInPlace(formula, pureLiterals);
}

struct q_element {
	vector<vector<int> > formula;
	vector<int> assignments;
	vector<int> unknowns;
};

bool solve(vector<vector<int> > formula, vector<int> unknowns) {
	bool satisfiable = false;
	vector<int> assignments;
	q_element t = {formula, assignments, unknowns, };
	deque<q_element> q;
	q.push_back(t);
	while (!q.empty()) {
		applyUnitPropogation(q.front().formula, q.front().assignments, q.front().unknowns);
		if (checkUnsatisfiable(q.front().formula)){
			q.pop_front();
			continue;
		}
		if (q.front().formula.size() == 0) {
			if (!checkSolution(q.front().formula, q.front().assignments)) {
				cout << "Fatal error!!!" << endl;
			}
			satisfiable = true;
			cout << "SATISFIABLE" << endl;
			//print solution
			for (int i = 0; i < q.front().assignments.size(); i++) {
				cout << q.front().assignments[i] << " ";
			}
			cout << endl;
			return satisfiable;
		}
		applyPureLiteralRule(q.front().formula, q.front().assignments, q.front().unknowns);
		if (q.front().formula.size() == 0) {
			if (!checkSolution(q.front().formula, q.front().assignments)) {
				cout << "Fatal error!!!" << endl;
			}
			satisfiable = true;
			cout << "SATISFIABLE" << endl;
			//print solution
			for (int i = 0; i < q.front().assignments.size(); i++) {
				cout << q.front().assignments[i] << " ";
			}
			cout << endl;
			return satisfiable;

		}
		vector < vector<int> > onSet;
		generateOnSet2(q.front().unknowns, onSet);
		for (int i = 0; i < onSet.size(); i++) {
			vector<int> onTerm = onSet[i];
			vector < vector<int> > f;
			evaluateQuotient(q.front().formula, onTerm,f);
			if (f.size() == 0) {
				vector<int> newAssignments(q.front().assignments);
				newAssignments.insert(newAssignments.end(), onTerm.begin(), onTerm.end());
				if (!checkSolution(f, newAssignments)) {
					cout << "Fatal error!!!" << endl;
				}
				satisfiable = true;
				cout << "SATISFIABLE" << endl;
				//print solution
				for (int i = 0; i < newAssignments.size(); i++) {
					cout << newAssignments[i] << " ";
				}
				cout << endl;
				return satisfiable;
			} else if (checkUnsatisfiable (f)) {
				continue;
			} else {
				vector<int> newAssignments(q.front().assignments);
				newAssignments.insert(newAssignments.end(), onTerm.begin(), onTerm.end());
				vector<int> newUnknowns;
				findUnassignedVars(onTerm, q.front().unknowns, newUnknowns);
				q.push_back({ f, newAssignments, newUnknowns });
			}
		}
	q.pop_front();
	}
	return false;
}
#endif /* SOLVER_H_ */
