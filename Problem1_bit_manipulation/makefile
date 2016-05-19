p1: func.o test.o encBase64.o decBase64.o main.o
	gcc -g -o p1 func.o test.o  encBase64.o decBase64.o main.o

decBase64.o: decBase64.c
	gcc -c -g -Wall -Wextra -std=c99 decBase64.c

encBase64.o: encBase64.c
	gcc -c -g -Wall -Wextra -std=c99 encBase64.c

func.o: func.c
	gcc -c -g -Wall -Wextra -std=c99 func.c

main.o: main.c
	gcc -c -g -Wall -Wextra -std=c99 main.c

test.o: test.c
	gcc -c -g -Wall -Wextra -std=c99 test.c

clean:
	-$(RM) *.o p1
