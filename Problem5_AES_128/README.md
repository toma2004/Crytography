# Problem 5 - AES-128

There are 6 functions in problem 5:
+ tablecheck - check to see if an input table file is valid
+ modprod - perform modular product of two polynomials with coefficients in GF(2^8). This operation is also called CircleX
+ keyexpand - Perform the key expansion algorithm for AES-128 using key as the 16-byte cipher key in hexstring format (32 characters long). Please use the s-box in tablefile.
+ encrypt - encrypt a file using AES-128
+ decrypt - decrypt a file using AES-128
+ inverse - calculate multiplicative inverse of a polynomial with coefficient in GF(2^8) using the extended Euclidean Algorithm. Using Table Method to implment the Euclidean Algorithm

Test:
./p5 tablecheck -t=f0.tbl

./p5 modprod -p1=f183f2a4 -p2=0d5fe07d > f0.mp

./p5 keyexpand -k=4c1a7217a218cf6daa603c4a39736301 -t=f0.tbl > f0.ke

./p5 encrypt -k=4c1a7217a218cf6daa603c4a39736301 -t=f0.tbl f0 > f0.enc

./p5 decrypt -k=4c1a7217a218cf6daa603c4a39736301 -t=f0.tbl f0.enc > f0.dec

./p5 inverse -p=0d5fe07d > f0.inv