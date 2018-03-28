##
##
##
BINDIR=bin
EXES = $(BINDIR)/serialize_test
SRCS=$(wildcard src/*.cpp )
OBJS=$(patsubst %.cpp,%.o,$(SRCS) )
RM :=rm -f 

CFLAGS= -g -Wall  -rdynamic -O2
CXXFLAGS = -g -Wall -rdynamic -O2
CPPFLAGS = -I./deps -I./deps/testlib
LIBS = -L./lib -L./deps/lib -llut

all: dir $(OBJ) $(EXES)

show:
	@echo "EXES=$(EXES)"
	@echo "SRCS=$(SRCS)"
	@echo "OBJS=$(OBJS)"

dir:
	if [ ! -d $(BINDIR) ]; then mkdir $(BINDIR) ; fi;

$(EXES): $(OBJS)
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)

clean:
	$(RM) $(EXES) $(OBJS)
	
.c:
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $<

.cpp:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

#
#

