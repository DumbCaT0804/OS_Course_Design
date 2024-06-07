objects = main.o interface.o prase.o execute.o

yzw_shell : $(objects)
	gcc -o yzw_shell $(objects) -lreadline -lncurses

main.o interface.o prase.c execute.c : shell.h

.PHONY : clean
clean :
	rm yzw_shell $(objects)
