# Problem 1 - Bit manipulation

There are 3 functions in problem 1:
+ Hexdump
+ enc-base64
+ dec-base64

hexdump	      :  	Produce a hexdump of file (or stdin).
enc-base64	  :  	Base64 encode file (or stdin).
dec-base64	  :  	Base64 decode file (or stdin).

Test:
./p1 hexdump f5 > f5.hex

./p1 enc-base64 f5 > f5.b64

./p1 dec-base64 f5.b64 > f5.dat

./p1 enc-base64 f5 | ./p1 dec-base64 > f5_combined.dat