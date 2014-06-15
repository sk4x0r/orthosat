/*
 * main.cpp
 *
 *  Created on: 13-May-2014
 *      Author: sp
 */

#include<iostream>
#include<cstdio>
#include<stdlib.h>
#include<mpi.h>
#include "util.h"
#include "solver.h"
using namespace std;
int main(int argc, char** argv) {
	int ierr;
	ierr = MPI_Init(&argc, &argv);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	if(world_size<2){
		cout<<"World size must be greater than or equal to 2. Exiting";
		//MPI_Abort(MPI_COMM_WORLD, 0);
		ierr = MPI_Finalize();
		return -1;
	}
	if (world_rank == 0) {

		if (argc != 2) {
			cout << "Usage: solver <path_to_dimacs_file>\n";
			return -1;
		}

		char * in;
		long length;
		FILE * f = fopen(argv[1], "rb");

		if (f) {
			fseek(f, 0, SEEK_END);
			length = ftell(f);
			fseek(f, 0, SEEK_SET);
			in = (char*) malloc(length);
			if (in) {
				fread(in, 1, length - 1, f);
				in[length - 1] = -1;
			} else {
				cout << "Fatal Error: Could not allocate memory buffer of length " << length << endl;
				fclose(f);
				return -1;
			}
			fclose(f);
		} else {
			cout << "Fatal Error: Could not read from file.";
			return -1;
		}
		vector<vector<int> > formula;
		int vars, clauses;
		parseDimacs(in, formula, &vars, &clauses);
		cout << "vars=" << vars << endl << "clauses=" << clauses << endl;
		vector<int> unknowns;
		orderUnknowns(formula, vars, unknowns);
		timestamp_t t0 = get_timestamp(), t1;
		vector<int> solution;
		if (solve(formula, unknowns, world_size, solution)) {
			t1 = get_timestamp();
			cout << "SATISFIABLE" << endl;
			cout<<"Size="<<solution.size()<<endl;
			printVector(solution);
		} else {
			t1 = get_timestamp();
			cout << "UNSATISFIABLE" << endl;
		}
		double secs = (t1 - t0) / 1000000.0L;
		cout << "Time:" << secs << " secs" << endl;
	}else{
		worker(world_rank);
	}
	MPI_Abort(MPI_COMM_WORLD, 0);
	ierr = MPI_Finalize();
	return 0;
}
