all:
	mpic++ main.cpp -o orthosat -std=c++11 -O3

clean:
	rm -rf *o orthosat *~
