p6: main.o primes.o trialdiv.o millerrabin.o rndsearch.o maurer.o
	gcc -g -o p6 main.o primes.o trialdiv.o millerrabin.o rndsearch.o maurer.o -lcrypto -lm

main.o: main.c
	gcc -c -g -Wall -Wextra -std=c99 main.c

primes.o: primes.c
	gcc -c -g -Wall -Wextra -std=c99 primes.c

trialdiv.o: trialdiv.c
	gcc -c -g -Wall -Wextra -std=c99 trialdiv.c

millerrabin.o: millerrabin.c
	gcc -c -g -Wall -Wextra -std=c99 millerrabin.c

rndsearch.o: rndsearch.c
	gcc -c -g -Wall -Wextra -std=c99 rndsearch.c

maurer.o: maurer.c
	gcc -c -g -Wall -Wextra -std=c99 maurer.c

clean:
	-$(RM) *.o p6
