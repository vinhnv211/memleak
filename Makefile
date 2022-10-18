
test:test.o 
	gcc test.c linklist.c -o test

clean:
	rm -rf test *.o