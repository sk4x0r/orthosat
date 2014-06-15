all:
	mpic++ main.cpp -o orthosat -std=c++0x -O3

clean:
	rm -rf *o orthosat *~
