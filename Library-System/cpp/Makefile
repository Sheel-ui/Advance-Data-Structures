CXX = g++
CXXFLAGS = -std=c++11 -Wall

gatorLibrary: main.o
	$(CXX) $(CXXFLAGS) main.o -o gatorLibrary

main.o: main.cpp redBlackTree.h redBlackTreeNode.h reservationHeap.h reservationHeapNode.h book.h
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	rm -f *.o gatorLibrary