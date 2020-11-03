

SRCS = col_compression.cc
LIBNAME = libtestzzy.so

all:$(OBJS)
	g++ ${SRCS} -fopenmp -shared -fPIC -o ${LIBNAME}

clean:
	rm -f *.so *.o

