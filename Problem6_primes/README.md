# Problem 6 - Primes

There are 5 functions in problem 6:
+ primes - generate all primes numbers less than or equal to maxval
+ trialdiv - Test the primality of number using trial division by trying sequentially all small prime numbers from 2 to floor(sqrt(number)), inclusive
+ millerrabin - Test the primality of number using Miller-Rabin with security parameter maxitr
+ rndsearch - Generate a numbits-bit probable prime using the Random-Search(numbits,maxitr) algorithm with security parameter maxitr
+ maurer - Generate a numbits-bit provable prime using the Maurer(numbits) algorithm

Test:
./p6 primes -n=604235199 > f0.pms

./p6 trialdiv -n=604235199 -p=primes.n223 > f0.td

./p6 millerrabin -n=604235199 -t=12 -p=primes.n223 > f0.mr

./p6 rndsearch -k=15 -t=10 -p=primes.n223 -r=rnd.220 > f0.rs

./p6 maurer -k=15 -p=primes.n223 -r=rnd.220 > f0.ma