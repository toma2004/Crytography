p2: main.o stream.o encrypt.o pbm.o merge.o decrypt.o
	gcc -g -o p2 main.o stream.o encrypt.o pbm.o merge.o decrypt.o -lcrypto -lm
		
main.o: main.c
	gcc -c -g -Wall -Wextra -std=c99 main.c

stream.o: stream.c
	gcc -c -g -Wall -Wextra -std=c99 stream.c
	
encrypt.o: encrypt.c
	gcc -c -g -Wall -Wextra -std=c99 encrypt.c

pbm.o: pbm.c
	gcc -c -g -Wall -Wextra -std=c99 pbm.c

merge.o: merge.c
	gcc -c -g -Wall -Wextra -std=c99 merge.c

decrypt.o: decrypt.c
	gcc -c -g -Wall -Wextra -std=c99 decrypt.c
		
clean:
	-$(RM) *.o p2

