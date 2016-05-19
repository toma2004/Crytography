p5: main.o tablecheck_AES.o modprod.o keyexpand.o encrypt.o decrypt.o inverse.o
	gcc -g -o p5 main.o tablecheck_AES.o modprod.o keyexpand.o encrypt.o decrypt.o inverse.o
		
main.o: main.c
	gcc -c -g -Wall -Wextra -std=c99 main.c
	
tablecheck_AES.o: tablecheck_AES.c
	gcc -c -g -Wall -Wextra -std=c99 tablecheck_AES.c

modprod.o: modprod.c
	gcc -c -g -Wall -Wextra -std=c99 modprod.c

keyexpand.o: keyexpand.c
	gcc -c -g -Wall -Wextra -std=c99 keyexpand.c

encrypt.o: encrypt.c
	gcc -c -g -Wall -Wextra -std=c99 encrypt.c

decrypt.o: decrypt.c
	gcc -c -g -Wall -Wextra -std=c99 decrypt.c

inverse.o: inverse.c
	gcc -c -g -Wall -Wextra -std=c99 inverse.c

clean:
	-$(RM) *.o p5
