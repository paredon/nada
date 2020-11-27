all : encoder decoder

encoder : encoder.cpp
	g++ -std=c++11 -o encoder encoder.cpp

decoder : decoder.cpp
	g++ -std=c++11 -o decoder decoder.cpp

clean :
	rm *.out encoder *.out decoder