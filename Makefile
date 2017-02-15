all:
	g++ -c main.cpp -o main.out
clean:
	rm main
cleancompile: clean all
