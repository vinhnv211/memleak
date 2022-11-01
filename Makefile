du: du_malloc_mem.o test_library_du.o
	gcc du_malloc_mem.c test_library_du.c -o du -rdynamic -g 
clean:
	rm -rf du *.o
#-fsanitize=address
