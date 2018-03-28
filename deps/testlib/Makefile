##
##
##
LIBS = liblut.a

SRC=$(wildcard ./*.cpp)

OBJ=$(patsubst %.cpp,%.o,$(SRC) )

RM :=rm -f 



CFLAGS= -g -Wall -fPIC -rdynamic -O2

CXXFLAGS = -g -Wall -fPIC -rdynamic -O2

CPPFLAGS =


all: $(OBJ) $(LIBS)


$(LIBS): $(OBJ)
	ar rcs $@ $?

clean:
	$(RM) $(LIBS) *.o


.c:
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $<

.cpp:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<
