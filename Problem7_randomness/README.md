# Problem 7 - Randomness

There are 6 functions in problem 7:
+ rc4 - simulate of RC4 stream cipher
+ x1 - Frequency test
+ x2 - Serial test
+ x3 - Poker test
+ x4 - Runs test
+ x5 - Autocorrelation test

Test:
./p7 rc4 -p="yesnomaybe" > f0.states

./p7 x1 f2 > f2.x1

./p7 x2 f2 > f2.x2

./p7 x3 f2 > f2.x3

./p7 x4 f2 > f2.x4

./p7 x5 -d=8 f2 > f2.x5