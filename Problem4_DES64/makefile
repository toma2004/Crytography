p4: main.o tablecheck.o encrypt.o decrypt.o encrypt3.o decrypt3.o
	gcc -g -o p4 main.o tablecheck.o encrypt.o decrypt.o encrypt3.o decrypt3.o
		
main.o: main.c
	gcc -c -g -Wall -Wextra -std=c99 main.c
	
tablecheck.o: tablecheck.c
	gcc -c -g -Wall -Wextra -std=c99 tablecheck.c
	
encrypt.o: encrypt.c
	gcc -c -g -Wall -Wextra -std=c99 encrypt.c

decrypt.o: decrypt.c
	gcc -c -g -Wall -Wextra -std=c99 decrypt.c

encrypt3.o: encrypt3.c
	gcc -c -g -Wall -Wextra -std=c99 encrypt3.c

decrypt3.o: decrypt3.c
	gcc -c -g -Wall -Wextra -std=c99 decrypt3.c
		
clean:
	-$(RM) *.o p4
