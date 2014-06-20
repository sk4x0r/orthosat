#OrthoSat

OrthoSat is a parallel SAT solver implemented using approach of Orthonormal Expansion for scalably solving boolean satisfiability problems. The details of implemented approach can be found in paper [An algorithm for Boolean satisfiability based on generalized orthonormal expansion](http://arxiv.org/abs/1406.4712).

#Installation and Usage

```
// get the sourcecode
git pull https://github.com/sk4x0r/orthosat.git
//build the executable, this will create an executable named 'orthosat' in source folder
make
//run the executable using mpirun, passing the .cnf file as parameter to executable
mpirun <parameters_for_mpirun> ./orthosat <.cnf_file>
```

#Dependency
Implementation depends upon OpenMPI for executing the program on multiple cores which can be downloaded from [OpenMPI website](http://www.open-mpi.org/)

# License

The package is available under GNU General Public License. See the _LICENSE_ file.
