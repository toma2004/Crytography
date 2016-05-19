p3: main.o keygen.o invkey.o crypt.o histo.o solve.o
	gcc -g -o p3 main.o keygen.o invkey.o crypt.o histo.o solve.o -lcrypto -lm
		
main.o: main.c
	gcc -c -g -Wall -Wextra -std=c99 main.c

keygen.o: keygen.c
	gcc -c -g -Wall -Wextra -std=c99 keygen.c

crypt.o: crypt.c
	gcc -c -g -Wall -Wextra -std=c99 crypt.c
	
invkey.o: invkey.c
	gcc -c -g -Wall -Wextra -std=c99 invkey.c

histo.o: histo.c
	gcc -c -g -Wall -Wextra -std=c99 histo.c
	
solve.o: solve.c
	gcc -c -g -Wall -Wextra -std=c99 solve.c
		
clean:
	-$(RM) *.o p3

