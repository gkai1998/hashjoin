hashjoin_r1r2: Hash_Join.o
	g++ -o HashJoin_r1r2 Hash_Join.o
hash_join.o:Hash_Join.cpp
	g++ -c  Hash_Join.cpp

benchmark: 
	g++ -Wall -std=c++14 Hash_Join.cpp -pthread -lbenchmark

clean:
	rm Hash_Join.o HashJoin_r1r2