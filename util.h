/*
 * util.h
 *
 *  Created on: 13-May-2014
 *      Author: sp
 */

#ifndef UTIL_H_
#define UTIL_H_
#include<stdlib.h>
#include<string.h>
#include<cstdio>
#include<vector>
#include<iostream>
#include <fstream>
#include<time.h>
#include <sys/time.h>
#include <utility>
#include<algorithm>
using namespace std;


typedef unsigned long long timestamp_t;

    static timestamp_t
    get_timestamp ()
    {
      struct timeval now;
      gettimeofday (&now, NULL);
      return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
    }



template<class B>
static void skipWhitespace(B& in) {
	while ((*in >= 9 && *in <= 13) || *in == 32)
		++in;
}

template<class B>
static void skipLine(B& in) {
	//cout<<"inside skipline"<<endl;
	for (;;) {
		if (*in == -1) //TODO: check *in for EOF
			return;
		if (*in == '\n') {
			++in;
			//cout<<"leaving skipline"<<endl;
			return;
		}
		++in;
	}
	//cout<<"leaving skipline"<<endl;
}

template<class B>
static int parseInt(B& in) {
	//cout << "inside parseInt" << endl;
	int val = 0;
	bool neg = false;
	skipWhitespace(in);
	if (*in == '-')
		neg = true, ++in;
	else if (*in == '+')
		++in;
	if (*in < '0' || *in > '9')
		fprintf(stderr, "PARSE ERROR in parseInt! Unexpected char: %c %d\n",
				*in, *in), exit(3);
	while (*in >= '0' && *in <= '9')
		val = val * 10 + (*in - '0'), ++in;
	return neg ? -val : val;
}

template<class B>
static bool eagerMatch(B& in, const char* str) {
	for (; *str != '\0'; ++str, ++in)
		if (*str != *in)
			return false;
	return true;
}

template<class B>
static vector<int> readClause(B& in) {
	vector<int> lits;
	//cout << "inside readClause" << endl;
	int parsed_lit;
	lits.clear();
	for (;;) {
		parsed_lit = parseInt(in);
		//cout << "parsed_lit=" << parsed_lit << endl;
		if (parsed_lit == 0)
			break;
		//cout << "pushing " << parsed_lit << endl;
		//cout << "lits.size()=" << lits.size() << endl;
		lits.push_back(parsed_lit*-1);
		//cout << "pushed" << endl;
	}
	//cout << "returning from readClause" << endl;
	return lits;
}

template<class B>
pair<int, int> parseDimacs(B& in, vector<vector<int> > &formula, int* vars,
		int* clauses) {
	vector<int> lits; //TODO: initialize lits to size 3
	int cnt = 0;

	for (;;) {
		//cout << *in << endl;
		if (*in == -1) {
			//cout << "EOF found" << endl;
			break;
		}
		skipWhitespace(in);
		if (*in == 'p') {
			//cout << "found p" << endl;
			if (eagerMatch(in, "p cnf")) {
				*vars = parseInt(in);
				*clauses = parseInt(in);
			} else {
				printf(
						"PARSE ERROR in dimacsToHypergraph! Unexpected char: %c\n",
						*in), exit(3);
			}
		} else if (*in == 'c') {
			skipLine(in);
		} else {
			cnt++;
			//cout << "reading clauses" << endl;
			lits = readClause(in);
			formula.push_back(lits);
		}
	}
}

char* readFile(char* fileName) {
	char* data;
	//cout<<"Filename is"<<fileName<<endl;
	FILE * f = fopen(fileName, "rb");
	if (f) {
		fseek(f, 0, SEEK_END);
		int length = ftell(f);
		fseek(f, 0, SEEK_SET);
		data = (char*) malloc(length);
		if (data) {
			fread(data, 1, length - 1, f);
			data[length - 1] = '\0';
		} else {
			cout << "Fatal Error: Could not allocate memory buffer of length "
					<< length << endl;
		}
		fclose(f);
	} else {
		cout << "Fatal Error: Could not read from file " << fileName << endl;
	}
	//cout<<"contents of file="<<data<<endl;
	return data;
}

struct sort_pred {
    bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) {
        return left.second < right.second;
    }
};


inline void printVec(vector<int> v) {
	for (int i = 0; i < v.size(); i++) {
		cout<<v[i]<<" ";
	} cout<<endl;
}

inline void printPairs(vector<pair<int, int> > v) {
	for (int i = 0; i < v.size(); i++) {
		cout<<v[i].first<<" "<<v[i].second<<endl;
	}
}

void orderUnknowns(vector<vector<int> > &formula, int unknownCount, vector<int> &unknowns){
	vector<int> count(unknownCount,0);
	for (int i = 0; i < formula.size(); i++) {
		for (int j = 0; j < formula[i].size(); j++) {
			++count[abs(formula[i][j])-1];
		}
	}
	vector<pair<int, int> > pairs;
	for(int i=0;i<unknownCount;i++){
		pairs.push_back(pair<int, int>(i+1, count[i]));
	}
	sort(pairs.begin(), pairs.end(), sort_pred());
	for(int i=pairs.size()-1;i>=0;i--){
		unknowns.push_back(pairs[i].first);
	}
}

#endif /* UTIL_H_ */
