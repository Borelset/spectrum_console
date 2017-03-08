CXX		= g++
CFLAGS		= -Wall 
LDFLAGS		= `pkg-config --cflags --libs opencv`
SRCS		= $(wildcard *.cpp)
all:$(SRCS)
	$(CXX) $(SRCS) -o spectrum $(LDFLAGS) $(CFLAGS)
clean:
	-rm -rf $(TARGETS) *~ .*swp
.PHONY: clean all
