

SRCS = calc.cc
LIBNAME = libcompetition.so

all:$(OBJS)
	g++ ${SRCS} -fopenmp -shared -fPIC -o ${LIBNAME}

clean:
	rm -f *.so *.o

