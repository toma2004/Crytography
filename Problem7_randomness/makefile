p7: main.o rc4.o x1.o x2.o x3.o x4.o x5.o
	gcc -g -o p7 main.o rc4.o x1.o x2.o x3.o x4.o x5.o -L/home/scf-22/csci551b/openssl/lib -lcrypto -lm

main.o: main.c
	gcc -c -g -Wall -Wextra -std=c99 main.c -I/home/scf-22/csci551b/openssl/include

rc4.o: rc4.c
	gcc -c -g -Wall -Wextra -std=c99 rc4.c -I/home/scf-22/csci551b/openssl/include
	
x1.o: x1.c
	gcc -c -g -Wall -Wextra -std=c99 x1.c	

x2.o: x2.c
	gcc -c -g -Wall -Wextra -std=c99 x2.c

x3.o: x3.c
	gcc -c -g -Wall -Wextra -std=c99 x3.c

x4.o: x4.c
	gcc -c -g -Wall -Wextra -std=c99 x4.c

x5.o: x5.c
	gcc -c -g -Wall -Wextra -std=c99 x5.c

clean:
	-$(RM) *.o p7
