# Generic target
all: studiomgmt

studiomgmt: bin/main.o bin/Studio.o bin/Trainer.o bin/Workout.o bin/Action.o bin/Customer.o
	g++ -o bin/studiomgmt bin/main.o bin/Studio.o bin/Trainer.o bin/Workout.o bin/Action.o bin/Customer.o

bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -I include -o bin/main.o src/main.cpp

bin/Studio.o: src/Studio.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -I include -o bin/Studio.o src/Studio.cpp

bin/Trainer.o: src/Trainer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -I include -o bin/Trainer.o src/Trainer.cpp

bin/Workout.o: src/Workout.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -I include -o bin/Workout.o src/Workout.cpp

bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -I include -o bin/Action.o src/Action.cpp

bin/Customer.o: src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -I include -o bin/Customer.o src/Customer.cpp

clean:
	rm -f bin/*

