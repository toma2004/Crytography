# Problem 4 - DES-64

There are 5 functions in problem 4:
+ tablecheck - check to see if an input table file is valid
+ encrypt - encrypt a binary file using DES-64
+ decrypt - decrypt a binary file using DES-64
+ encrypt3 - use 3DES
+ decrypt3 - use 3DES

Test:
./p4 tablecheck -t=f0

./p4 encrypt -k=0123456789abcdef -t=f0 f22 > fout.enc

./p4 decrypt -k=0123456789abcdef -t=f0 fout.enc > fout.dec

./p4 encrypt3 -k=1596369d5aa562355453ae44101c2614c729a00607319591 -t=f0 f22 > fout-3DES.enc

./p4 decrypt3 -k=1596369d5aa562355453ae44101c2614c729a00607319591 -t=f0 fout-3DES.enc > fout-3DES.dec