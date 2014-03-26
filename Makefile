all: tangle

tangle: webdown.o tangle.o
	g++ -o $@ $^

webdown.o: webdown.cpp webdown.h
	g++ -c $<

tangle.o: tangle.cpp webdown.h
	g++ -c $<

clean:
	rm -rf *.o tangle