CXX          = g++
CFLAGS    = -Wall 
LDFLAGS  = `pkg-config --cflags --libs opencv`
SRCS = $(wildcard *.cpp)
TARGETS = $(ptsubst %.cpp, %, $(SRCS))
all:$(TARGETS)
	$(CXX) $(TARGETS) -o spectrum $(LDFLAGS) $(CFLAGS)
clean:
	-rm -rf $(TARGETS) *~ .*swp
.PHONY: clean all
