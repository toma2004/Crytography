# Problem 3 - Cryptanalysis

There are 5 functions in problem 3:
+ keygen - generating key stream using MD5
+ crypt - encrypt an input file with generated key file
+ invkey - invert a key file for decyption
+ histo - analyze using histogram methood. Produce a histogram of ciphertext character frequencies assuming the ciphertext is produced by a polyalphabetic substitution cipher with period equals period. which specifies a one-based index for the alphabets. The output must be sorted according to the frequencies of the ciphertext characters in descending order. Please do not consider any characters that is not a lowercase English character (i.e., between 'a' and 'z', inclusive).
+ solve - Apply the Method of Kasiski and use Index of Coincidence in order to determine the period of a ciphertext encrypted by a full Vigenère cipher (with polyalphabetic substitution).

Test:
./p3 keygen -p="apple" -t=11 > f1.keys

./p3 invkey f1.keys > f1.invkeys

./p3 crypt -k=f1.keys f1.pt > f1.ct

./p3 crypt -k=f1.invkeys f1.ct > f1-dec.pt

(f1-dec.pt should be the same as the original plaintext f1.pt)

./p3 histo -t=5 -i=1 f1.ct > f1.histo.1

./p3 solve -l=17 f1.ct > f1.solve