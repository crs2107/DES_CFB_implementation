The "DES.cpp" file contains the whole implementation of the DES encryption and decryption algorithm. "CFB.cpp" contains the implementation of CFB encryption and decryption. I have added 3 files to test where you can stress test using a very small file i.e "small_poem.txt" or with a comparatively larger file "t512505.soc". I have written "makefile" to handle multi file coding. To build all the cpp files you have to write

1."make clean"
2."make" 

All the files will be built and the combined object file will  be "CFB_application-linux" .You can check using the "ls" command. 
Now you need to run the code using 

3."./CFB_application-linux "filename"". 

The cipher text will be put in "cipher.txt" and the decipher text will be put in "decipher.txt" .
