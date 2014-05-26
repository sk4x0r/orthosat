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

void print2dVector(vector<vector<int> > v) {
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].size(); j++) {
			//cout<<v[i][j]<<" ";
		} //cout<<endl;
	}
}

void printVector(vector<int> v) {
	for (int i = 0; i < v.size(); i++) {
		//cout<<v[i];
	} //cout<<endl;
}

bool isPresentIn(vector<int> v, int n) {
	//cout<<"inside isPresentIn"<<endl;
	for (int i = 0; i < v.size(); i++) {
		if (v[i] == n) {
			return true;
		}
	}
	//cout<<"returning from isPresentIn"<<endl;
	return false;
}

vector<vector<int> > evaluateQuotient(vector<vector<int> > formula, vector<int> assignments) {
	//cout << "inside evaluateQuotient" << endl;
	vector < vector<int> > fIter(formula);
	int removedClauses = 0;
	for (int i = 0; i < assignments.size(); i++) {
		int assignment = assignments[i];
		for (int j = fIter.size() - 1; j >= 0; j--) {
			//cout<<"1"<<endl;
			for (int k = fIter[j].size() - 1; k >= 0; k--) {
				//cout<<"2"<<endl;
				if (assignment + fIter[j][k] == 0) {
					//cout<<"if"<<endl;
					//cout << "removing term" << endl;
					fIter.erase(fIter.begin() + j);
					//cout << "removed" << endl;
					break;
				} else if (assignment == fIter[j][k]) {
					//cout<<"else"<<endl;
					//cout << "removing literal at pos [" << j << "][" << k << "]"<< endl;
					//cout << "size=" << fIter[j].size() << endl;
					//cout << "fIter[j][k]=" << fIter[j][k] << endl;
					//cout << "fIter[j].begin()=" << *(fIter[j].begin()) << endl;
					//cout << "fIter[j].begin()+k=" << *(fIter[j].begin() + k)<< endl;
					//cout << "sz=" << fIter[j].size() << endl;
					if (fIter[j].size() <= 1) {
						fIter[j].clear();
					} else {
						fIter[j].erase(fIter[j].begin() + k);
					}
					//cout<<"removed"<<endl;
				}
			}
		}
	}
	//cout << "returning from evaluateQuotient" << endl;
	return fIter;
}

void applyUnitPropogation(vector<vector<int> > formula, vector<int> assignments, vector<int> unknowns) {
	//cout<<"inside applyUnitProp"<<endl;
	vector<int> alreadyRemoved;
	for (int i = 0; i < formula.size(); i++) {
		if (formula[i].size() == 1) {
			if (isPresentIn(alreadyRemoved, abs(formula[i][0]))) {
				continue;
			}
			alreadyRemoved.push_back(abs(formula[i][0]));
			assignments.push_back(formula[i][0]);
			//cout<<"erasing from unknowns"<<endl;
			unknowns.erase(remove(unknowns.begin(), unknowns.end(), abs(formula[i][0])), unknowns.end());
			//cout<<"erased"<<endl;
			vector<int> v{-1 * (formula[i][0])};
			//cout<<"evaluating Quitient"<<endl;
			formula = evaluateQuotient(formula, v);
			//cout<<"evaluated"<<endl;
		}
	}
	//cout<<"returning from applyUnitProp"<<endl;
}

int findMin(vector<int> array) {
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

vector<vector<int> > generateOnSet(vector<int> unknowns) {
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

vector<int> findUnassignedVars(vector<int> term, vector<int> varList) {
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

bool checkUnsatisfiable(vector<vector<int> > formula) {
	//cout<<"inside checkUnsatisfiable"<<endl;
	for (int i = 0; i < formula.size(); i++) {
		if (formula[i].size() == 0) {
			return true;
		}
	}
	return false;
}

vector<vector<int> > trimFormula(vector<vector<int> > formula) {
	//cout<<"inside trimFormula"<<endl;
	vector < vector<int> > trimmedFormula;
	for (int i = 0; i < formula.size(); i++) {
		if (formula[i].size() != 0) {
			trimmedFormula.push_back(formula[i]);
		}
	}
	//cout<<"returning"<<endl;
	return trimmedFormula;
}

bool checkSolution(vector<vector<int> > formula, vector<int> assignments) {
	//cout<<"inside checkSolution"<<endl;
	if (evaluateQuotient(formula, assignments).size() == 0) {
		return true;
	}
	return false;
}

struct q_element {
	vector<vector<int> > formula;
	vector<int> assignments;
	vector<int> unknowns;
};

void applyPureLiteralRule(vector<vector<int> > formula, vector<int> assignments, vector<int> unknowns) {
	//cout << "applyPureLiteral begins" << endl;
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
				//cout<<"literal exists and complement doesn't"<<endl;
				vector<int> v{-1 * unknowns[i]};
				formula = evaluateQuotient(formula, v);
				assignments.push_back(-1 * unknowns[i]);
				unknowns.erase(remove(unknowns.begin(), unknowns.end(), unknowns[i]), unknowns.end());
			}
			if (!literalExists && complementExists) {
				//cout<<"complement exists and literal doesn't"<<endl;
				vector<int> v{unknowns[i]};
				formula = evaluateQuotient(formula, v);
				assignments.push_back(unknowns[i]);
				unknowns.erase(remove(unknowns.begin(), unknowns.end(), unknowns[i]), unknowns.end());
			}
		}
	//cout << "applyPureLiteral ends" << endl;
}

bool solve(vector<vector<int> > formula, vector<int> unknowns) {
	//cout << "inside solve" << endl;
	bool satisfiable = false;
	vector<int> assignments;
	q_element t = { formula, assignments, unknowns, };
	deque<q_element> q;
	q.push_back(t);

	//cout << "going inside while" << endl;
	while (!q.empty()) {
		t = q.front();
		q.pop_front();
		formula = t.formula;
		assignments = t.assignments;
		unknowns = t.unknowns;

		//cout<<"formula:"<<endl;
		print2dVector(formula);
		//cout<<"assignments:"<<endl;
		printVector(assignments);
		//cout<<"unknowns:"<<endl;
		printVector(unknowns);

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

		//cout << "generating ON sets from " << unknowns.size() << " unknowns"<< endl;
		vector < vector<int> > onSet = generateOnSet(unknowns);
		//cout << "generated " << onSet.size() << " on sets" << endl;
		//cout << "onsets are" << endl;
		for (int i = 0; i < onSet.size(); i++) {
			for (int j = 0; j < onSet[i].size(); j++) {
				//cout << onSet[i][j] << " ";
			}
			//cout << endl;
		}
		//cout << "Evaluating quotient for each ON term" << endl;
		for (int i = 0; i < onSet.size(); i++) {
			vector<int> onTerm = onSet[i];
			//cout << "onTerm=";
			for (int i = 0; i < onTerm.size(); i++) {
				//cout << onTerm[i] << " ";
			}
			//cout << endl;
			vector < vector<int> > f = evaluateQuotient(formula, onTerm);
			/*//cout<<"Quotient=";
			 for(int i=0;i<f.size();i++){
			 for(int j=0;j<f[i].size();j++){
			 //cout<<f[i][j];
			 }//cout<<" + ";
			 }//cout<<endl;*/
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
				//cout<<"prev assignments"<<endl;
				printVector(assignments);
				//cout<<"on Terms"<<endl;
				printVector(onTerm);
				newAssignments.insert(newAssignments.end(), onTerm.begin(), onTerm.end());
				//cout<<"new assignments"<<endl;
				printVector(newAssignments);
				unknowns = findUnassignedVars(onTerm, unknowns);
				f = trimFormula(f);
				q_element temp = { f, newAssignments, unknowns };
				//cout << "pushing to queue" << endl;
				q.push_back(temp);
			}
		}
	}
	return false;
}
#endif /* SOLVER_H_ */
