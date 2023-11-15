CXXFLAGS += -std=c++14

all: root_yaml.cxx root_yaml.so

ROOTLIBS = -lCore -lRIO -lCling -lHist

#root_yaml.so: root_yaml.o
#	g++ -shared $^ -o $@

%.o: %.cc @$(CXX) -I$(ROOTSYS)/include $(CXXFLAGS) -o"$@" "$<"

root_yaml.cxx: root_yaml.h LinkDef.h
	rootcling -f $@ -c $(CXXFLAGS) -p $^
 
root_yaml.so: root_yaml.cxx root_yaml.cc
	g++ -shared -o$@ `root-config --ldflags` $(CXXFLAGS) -I$(ROOTSYS)/include -I$(ROOTSYS)/include/root -L$(ROOTSYS)/lib/root $(ROOTLIBS) $^



 
