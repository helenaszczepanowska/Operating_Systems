CXX = gcc
CFLAGS = -c -g -Wall -std=c17

.PHONY: all clean

all: static shared dynamic

collatz_static: collatz.c
	$(CXX) $(CFLAGS) -o collatz_static.o collatz.c
	ar rcs libcollatz_static.a collatz_static.o

collatz_shared: collatz.c
	$(CXX) -fPIC $(CFLAGS) -o collatz_shared.o collatz.c
	$(CXX) -shared -o libcollatz_shared.so collatz_shared.o -lc

main_static: main.c
	$(CXX) main.c -lcollatz_static -L./ -o main_static.o 

main_shared: main.c
	$(CXX) main.c -lcollatz_shared -L./ -Wl,-rpath,. -o main_shared.o

main_dynamic: main.c
	$(CXX) -o main_dynamic.o main.c -D USE_DYNAMIC -ldl

objdump_static:
	objdump -d main_static.o > static_view.txt
objdump_shared:
	objdump -d main_shared.o > shared_view.txt
objdump_dynamic:
	objdump -d main_dynamic.o > dynamic_view.txt

static: collatz_static main_static
shared:	collatz_shared main_shared
dynamic: collatz_shared main_dynamic

test_all:
	./main_static.o
	./main_shared.o
	./main_dynamic.o

clean:
	rm -f *.o *.a *.so
