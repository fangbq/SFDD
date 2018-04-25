CXX = g++


CXXFLAGS +=  -O3 -Wall -Wextra -std=c++11 -pg


APPS =  sfdd


SRC=$(shell ls *.cc)

all :  $(APPS)



clean :
	rm -f *.o $(APPS)



dep:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MG $(SRC) >makefile.depend


.cc.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<


sfdd : main.o manager.o sfdd_vtree.o sfdd_node.o readVerilog.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS)  $^ -o $@

-include makefile.depend
