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
	//cout<<"returning from isPresentIn"<<endl;
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
	//cout<<"inside findmin"<<endl;
	//cout<<"size of array="<<array.size()<<endl;
	int smallest = array[0];
	for (int i = 0; i < array.size(); i++) {
		if (array[i] < smallest) {
			smallest = array[i];
		}
	}
	return smallest;
}

inline void evaluateQuotient(vector<vector<int> > &formula, vector<int> &assignments, vector<vector<int> > &fIter) {
	//cout << "inside evaluateQuotient" << endl;
	fIter=vector<vector<int> >(formula);
	int removedClauses = 0;
	for (int i = 0; i < assignments.size(); i++) {
		int assignment = assignments[i];
		for (int j = fIter.size() - 1; j >= 0; j--) {
			for (int k = fIter[j].size() - 1; k >= 0; k--) {
				if (assignment + fIter[j][k] == 0) {
					//fIter.erase(fIter.begin() + j);
					fIter[j]=fIter.back();
					fIter.pop_back();
					break;
				} else if (assignment == fIter[j][k]) {
					if (fIter[j].size() <= 1) {
						fIter[j].clear();
					} else {
						//fIter[j].erase(fIter[j].begin() + k);
						if(k!=fIter[j].size()-1)
							fIter[j][k]=fIter[j].back();
						fIter[j].pop_back();
					}
				}
			}
		}
	}
	//cout << "returning from evaluateQuotient" << endl;
}

inline void evaluateQuotientInPlace(vector<vector<int> > &formula, vector<int> &assignments) {
	//cout << "inside evaluateQuotient" << endl;
	//cout<<"assignments"<<endl;
	//printVector(assignments);
	//cout<<"formula"<<endl;
	//print2dVector(formula);
	int removedClauses = 0;
	for (int i = 0; i < assignments.size(); i++) {
		int assignment = assignments[i];
		for (int j = formula.size() - 1; j >= 0; j--) {
			for (int k = formula[j].size() - 1; k >= 0; k--) {
				if (assignment + formula[j][k] == 0) {
					//fIter.erase(fIter.begin() + j);
					formula[j]=formula.back();
					formula.pop_back();
					break;
				} else if (assignment == formula[j][k]) {
					if (formula[j].size() <= 1) {
						formula[j].clear();
					} else {
						//fIter[j].erase(fIter[j].begin() + k);
						if(k!=formula[j].size()-1)
							formula[j][k]=formula[j].back();
						formula[j].pop_back();
					}
				}
			}
		}
	}

	//cout<<"after"<<endl<<"formula"<<endl;
	//print2dVector(formula);
}

inline void applyUnitPropogation(vector<vector<int> > &formula, vector<int> &assignments, vector<int> &unknowns) {
	//cout<<"inside applyUnitProp"<<endl;
	vector<int> alreadyRemoved;
	vector<int> propogatedLiterals;
	for (int i = 0; i < formula.size(); i++) {
		if (formula[i].size() == 1) {
			if (isPresentIn(alreadyRemoved, abs(formula[i][0]))) {
				continue;
			}
			alreadyRemoved.push_back(abs(formula[i][0]));
			assignments.push_back(formula[i][0]);
			//cout<<"erasing from unknowns"<<endl;
			//unknowns.erase(remove(unknowns.begin(), unknowns.end(), abs(formula[i][0])), unknowns.end());
			int idx=findIndexOf(unknowns, abs(formula[i][0]));
			unknowns[idx]=unknowns.back();
			unknowns.pop_back();
			//cout<<"erased"<<endl;
			//vector<int> v{-1 * (formula[i][0])};
			propogatedLiterals.push_back(-1*formula[i][0]);
			//cout<<"evaluating Quitient"<<endl;
			//formula = evaluateQuotient(formula, v);
			//cout<<"evaluated"<<endl;
		}
	}
	evaluateQuotientInPlace(formula, propogatedLiterals);
	//cout<<"returning from applyUnitProp"<<endl;
}

inline void generateOnSet(vector<int> &unknowns, vector<vector<int> > &onSet) {
	//cout<<"inside generateOnSet"<<endl;
	//cout<<"size of unknowns="<<unknowns.size()<<endl;
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
	//newUnknowns.clear();
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
	//cout<<"inside findUnassignedVars"<<endl;
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
	//cout<<"inside checkUnsatisfiable"<<endl;
	for (int i = 0; i < formula.size(); i++) {
		if (formula[i].size() == 0) {
			return true;
		}
	}
	return false;
}

inline void trimFormula(vector<vector<int> > &formula) {
	//cout<<"inside trimFormula"<<endl;
	for (int i = 0; i < formula.size(); i++) {
		if (formula[i].size() != 0) {
			formula[i]=formula.back();
			formula.pop_back();
		}
	}
	//cout<<"returning"<<endl;
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
	//cout << "applyPureLiteral begins" << endl;
	//cout<<"formula:"<<endl;
	//print2dVector(formula);
	//cout<<"unknowns"<<endl;
	//printVector(unknowns);
	vector<int> pureLiterals;
	for (int i = 0; i < unknowns.size(); i++) {
		//cout << "unknown="<<unknowns[i]<<endl;
		bool literalExists = false;
		bool complementExists = false;
		for (int j = 0; j < formula.size(); j++) {
			//cout << "j=" << j << endl;
			if (isPresentIn(formula[j], unknowns[i])) {
				literalExists = true;
				//cout << "literal exists" << endl;
				if (complementExists) {
					//cout << "and complement also exists" << endl;
					break;
				}
			}
			if (isPresentIn(formula[j], -1 * (unknowns[i]))) {
				//cout<<"complement exists"<<endl;
				complementExists = true;
				//cout << "complement exists" << endl;
				if (literalExists) {
					//cout<<"and literal also exists"<<endl;
					break;
				}
			}
		}
			if (literalExists && !complementExists) {
				//cout<<"only literal exists, pushing"<<endl;
				pureLiterals.push_back(unknowns[i]);
			}else if (!literalExists && complementExists) {
				//cout<<"only complement exists, pushing"<<endl;
				pureLiterals.push_back(-1*unknowns[i]);
			}
		}
	//cout<<"number of pure literals found="<<pureLiterals.size()<<endl;
	//cout<<"previous assignments="<<endl;
	//printVector(assignments);
	for(int i=0;i<pureLiterals.size();i++){
		assignments.push_back(-1*pureLiterals[i]);
		unknowns.erase(remove(unknowns.begin(), unknowns.end(), abs(pureLiterals[i])), unknowns.end());
		pureLiterals[i]=-1*pureLiterals[i];
	}
	//cout<<"pure lits"<<endl;
	//printVector(pureLiterals);
	//cout<<"new assignments="<<endl;
	//printVector(assignments);
	//cout<<"formula before"<<endl;
	//print2dVector(formula);
	evaluateQuotientInPlace(formula, pureLiterals);
	//cout<<"after"<<endl;
	//print2dVector(formula);
	//cout << "applyPureLiteral ends" << endl;
}

struct q_element {
	vector<vector<int> > formula;
	vector<int> assignments;
	vector<int> unknowns;
};

bool solve(vector<vector<int> > formulaa, vector<int> unknownsa) {
	//cout << "inside solve" << endl;
	bool satisfiable = false;
	vector<int> assignmentsa;
	q_element t = {formulaa, assignmentsa, unknownsa, };
	deque<q_element> q;
	q.push_back(t);

	//cout << "going inside while" << endl;
	while (!q.empty()) {
		t = q.front();

		//TODO: unit propogation and pure literal elimination
		applyUnitPropogation(t.formula, t.assignments, t.unknowns);
		if (checkUnsatisfiable(t.formula)){
			q.pop_front();
			continue;
		}
		if (t.formula.size() == 0) {
			if (!checkSolution(t.formula, t.assignments)) {
				cout << "Fatal error!!!" << endl;
			}
			satisfiable = true;
			cout << "SATISFIABLE" << endl;
			//print solution
			for (int i = 0; i < t.assignments.size(); i++) {
				cout << t.assignments[i] << " ";
			}
			cout << endl;
			return satisfiable;

		}
		applyPureLiteralRule(t.formula, t.assignments, t.unknowns);
		if (t.formula.size() == 0) {
			if (!checkSolution(t.formula, t.assignments)) {
				cout << "Fatal error!!!" << endl;
			}
			satisfiable = true;
			cout << "SATISFIABLE" << endl;
			//print solution
			for (int i = 0; i < t.assignments.size(); i++) {
				cout << t.assignments[i] << " ";
			}
			cout << endl;
			return satisfiable;

		}
		vector < vector<int> > onSet;
		generateOnSet2(t.unknowns, onSet);
		for (int i = 0; i < onSet.size(); i++) {
			vector<int> onTerm = onSet[i];
			vector < vector<int> > f;
			evaluateQuotient(t.formula, onTerm,f);
			if (f.size() == 0) {
				vector<int> newAssignments(t.assignments);
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
				q.pop_front();
				continue;
			} else {
				vector<int> newAssignments(t.assignments);
				newAssignments.insert(newAssignments.end(), onTerm.begin(), onTerm.end());
				vector<int> newUnknowns;
				findUnassignedVars(onTerm, t.unknowns, newUnknowns);
				//trimFormula(f);
				q.push_back({ f, newAssignments, newUnknowns });
			}
		}
	q.pop_front();
	}
	return false;
}
#endif /* SOLVER_H_ */
