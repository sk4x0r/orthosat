/*
 * solver.h
 *
 *  Created on: 14-May-2014
 *      Author: sp
 */
#include <deque>
#include<algorithm>
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
		cout<<v[i];
	} cout<<endl;
}

inline bool isPresentIn(vector<int> v, int n) {
	//cout<<"inside isPresentIn"<<endl;
	for (int i = 0; i < v.size(); i++) {
		if (v[i] == n) {
			return true;
		}
	}
	//cout<<"returning from isPresentIn"<<endl;
	return false;
}

inline int findIndexOf(vector<int> v, int n){
	for(int i=0;i<v.size();i++){
		if(v[i]==n){
			return i;
		}
	}
	cout<<"Fatal error in findIndexOf: n not found in v"<<endl;
	return -1;
}

inline vector<vector<int> > evaluateQuotient(vector<vector<int> > formula, vector<int> assignments) {
	//cout << "inside evaluateQuotient" << endl;
	vector < vector<int> > fIter(formula);
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
	return fIter;
}

inline void evaluateQuotientInPlace(vector<vector<int> > formula, vector<int> assignments) {
	//cout << "inside evaluateQuotient" << endl;
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
}

inline void applyUnitPropogation(vector<vector<int> > formula, vector<int> assignments, vector<int> unknowns) {
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

inline int findMin(vector<int> array) {
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

inline vector<vector<int> > generateOnSet(vector<int> unknowns) {
	//cout<<"inside generateOnSet"<<endl;
	//cout<<"size of unknowns="<<unknowns.size()<<endl;
	vector < vector<int> > onSet = vector<vector<int> >();
	int minCount = findMin(unknowns);
	for (int i = 0; i < unknowns.size(); i++) {
		int count = unknowns[i];
		vector<int> term = vector<int>();
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
	return onSet;
}

inline vector<int> findUnassignedVars(vector<int> term, vector<int> varList) {
	//cout<<"inside findUnassignedVars"<<endl;
	vector<int> unassignedVars;
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
	return unassignedVars;
}

inline bool checkUnsatisfiable(vector<vector<int> > formula) {
	//cout<<"inside checkUnsatisfiable"<<endl;
	for (int i = 0; i < formula.size(); i++) {
		if (formula[i].size() == 0) {
			return true;
		}
	}
	return false;
}

//TODO: improve this
inline void trimFormula(vector<vector<int> > formula) {
	//cout<<"inside trimFormula"<<endl;
	for (int i = 0; i < formula.size(); i++) {
		if (formula[i].size() != 0) {
			formula[i]=formula.back();
			formula.pop_back();
		}
	}
	//cout<<"returning"<<endl;
}

inline bool checkSolution(vector<vector<int> > formula, vector<int> assignments) {
	//cout<<"inside checkSolution"<<endl;
	if (evaluateQuotient(formula, assignments).size() == 0) {
		return true;
	}
	return false;
}

inline void applyPureLiteralRule(vector<vector<int> > formula, vector<int> assignments, vector<int> unknowns) {
	//cout << "applyPureLiteral begins" << endl;
	vector<int> pureLiterals;
	for (int i = 0; i < unknowns.size(); i++) {
		//cout << "i=" << i << endl;
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
				complementExists = true;
				//cout << "complement exists" << endl;
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
	}
	evaluateQuotientInPlace(formula, pureLiterals);
	//cout << "applyPureLiteral ends" << endl;
}

struct q_element {
	vector<vector<int> > formula;
	vector<int> assignments;
	vector<int> unknowns;
};

bool solve(vector<vector<int> > formula, vector<int> unknowns) {
	//cout << "inside solve" << endl;
	bool satisfiable = false;
	vector<int> assignments;
	q_element t = {formula, assignments, unknowns, };
	deque<q_element> q;
	q.push_back(t);

	//cout << "going inside while" << endl;
	while (!q.empty()) {
		t = q.front();
		q.pop_front();
		formula = t.formula;
		assignments = t.assignments;
		unknowns = t.unknowns;

		applyUnitPropogation(formula, assignments, unknowns);
		if (checkUnsatisfiable(formula)) {
			continue;
		}
		applyPureLiteralRule(formula, assignments, unknowns);
		if (formula.size() == 0) {
			if (!checkSolution(formula, assignments)) {
				cout << "Fatal error!!!" << endl;
			}
			satisfiable = true;
			cout << "SATISFIABLE" << endl;
			//print solution
			for (int i = 0; i < assignments.size(); i++) {
				cout << assignments[i] << " ";
			}
			cout << endl;
			return satisfiable;

		}

		vector < vector<int> > onSet = generateOnSet(unknowns);
		for (int i = 0; i < onSet.size(); i++) {
			vector<int> onTerm = onSet[i];
			vector < vector<int> > f = evaluateQuotient(formula, onTerm);
			if (f.size() == 0) {
				vector<int> newAssignments(assignments);
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
				vector<int> newAssignments(assignments);
				newAssignments.insert(newAssignments.end(), onTerm.begin(), onTerm.end());
				unknowns = findUnassignedVars(onTerm, unknowns);
				trimFormula(f);
				q_element temp = { f, newAssignments, unknowns };
				q.push_back(temp);
			}
		}
	}
	return false;
}
#endif /* SOLVER_H_ */
