findcomp.exe: ConnectedComponent.o PGMimageProcessor.o findcomp.o
	g++ ConnectedComponent.o PGMimageProcessor.o findcomp.o -o findcomp.exe -std=c++2a
	chmod 700 findcomp.exe
findcomp.o: findcomp.cpp
	g++ -c findcomp.cpp -o findcomp.o -std=c++2a
PGMimageProcessor.o: PGMimageProcessor.cpp
	g++ -c PGMimageProcessor.cpp -o PGMimageProcessor.o -std=c++2a
ConnectedComponent.o: ConnectedComponent.cpp
	g++ -c ConnectedComponent.cpp -o ConnectedComponent.o -std=c++2a
clean:
	rm *.o findcomp.exe

