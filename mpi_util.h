/*
 * mpi_util.h
 *
 *  Created on: 11-Jun-2014
 *      Author: sp
 */

#include <mpi.h>
#include<vector>
#include "common.h"
#ifndef MPI_UTIL_H_
#define MPI_UTIL_H_
using namespace std;

void structToMsg(struct q_element &q, vector<int> &v){
	for(int i=0;i<q.unknowns.size();i++){
		v.push_back(q.unknowns[i]);
	}
	v.push_back(0);
	for(int i=0;i<q.assignments.size();i++){
		v.push_back(q.assignments[i]);
	}
	v.push_back(0);
	for(int i=0;i<q.formula.size();i++){
		for(int j=0;j<q.formula[i].size();j++){
			v.push_back(q.formula[i][j]);
		}
		v.push_back(0);
	}
}

void sendStruct(struct q_element &q, int receiver){
	vector<int> v;
	structToMsg(q, v);
	MPI_Send(&v[0], v.size(), MPI_INT, receiver, 0, MPI_COMM_WORLD);
}

void msgToStruct(vector<int> &v, struct q_element &t){
	int idx=0;
	vector<int> unknowns;
	for(;;idx++){
		if(v[idx]==0){
			idx++;
			break;
		}
		unknowns.push_back(v[idx]);
	}
	vector<int> assignments;
	for(;;idx++){
		if(v[idx]==0){
			idx++;
			break;
		}
		assignments.push_back(v[idx]);
	}
	vector<vector<int> > formula;
	vector<int> term;
	for(;idx<v.size();idx++){
		if(v[idx]==0){
			formula.push_back(term);
			term.clear();
		}else{
			term.push_back(v[idx]);
		}
	}
	t={formula, assignments, unknowns};
}

void receiveStruct(int sender, struct q_element &t){
	MPI_Status status;
	int msg_size;
	// Probe for an incoming message from process zero
	MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
	// When probe returns, the status object has the size and other
	// attributes of the incoming message. Get the size of the message
	MPI_Get_count(&status, MPI_INT, &msg_size);

	// Allocate a buffer just big enough to hold the incoming numbers
	vector<int> v(msg_size);
	// Now receive the message with the allocated buffer
	MPI_Recv(&v[0], msg_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	msgToStruct(v, t);
}
#endif /* MPI_UTIL_H_ */
