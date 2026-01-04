CXX = g++
CXXFLAGS = -std=c++11 -Wall

OBJS = main.o Person.o ReadPost.o User.o Manager.o

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o main $(OBJS)

main.o: main.cpp Person.h User.h Manager.h ReadPost.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Person.o: Person.cpp Person.h
	$(CXX) $(CXXFLAGS) -c Person.cpp

ReadPost.o: ReadPost.cpp ReadPost.h
	$(CXX) $(CXXFLAGS) -c ReadPost.cpp

User.o: User.cpp User.h Person.h
	$(CXX) $(CXXFLAGS) -c User.cpp

Manager.o: Manager.cpp Manager.h Person.h
	$(CXX) $(CXXFLAGS) -c Manager.cpp

clean:
	rm -f *.o main
