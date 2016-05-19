# Problem 2 - Visual Cryptography (please see here for more info: http://cacr.uwaterloo.ca/~dstinson/visual.html)

There are 4 functions in problem 2:
+ stream - generating a stream cipher using MD5
+ encrypt - image encrypting method
+ merge - merging two sub-images into 1 image
+ decrypt - image decrypting method

Test:
./p2 encrypt -p="yesnomaybe" -out=test test.pbm

./p2 merge test.1.pbm test.2.pbm > test-merged.pbm

./p2 decrypt test-merged.pbm > test-dec.pbm

(test-dec.pbm should be the same as the original input test.pbm)