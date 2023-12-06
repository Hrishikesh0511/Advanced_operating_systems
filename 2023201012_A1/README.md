# 2023201012_A1_Q1.cpp
### Steps to Execute:
> **Compile:** g++ 2023201012_A1_Q1.cpp

> **run:** ./a.out <file_path>(The file you want to reverse)
 ### Approach Used:
* To reverse the file I have just read the given file from the last and placed it in the starting of the new file.
* To optimize it for larger files I divided the files into equal chunks and then did a reversal operation in each chunk and  placed it in the file.
### Assumptions:
* The input file will not be empty and a valid .txt file.
* The input file will be having read permissions.
 ### Output:
It will create a new working directory named `write_file_q1` and it will have a file named `b.txt`.(_which is our required file_). 

---

# 2023201012_A1_Q2.cpp
### Steps to Execute:
> **Compile:** g++ 2023201012_A1_Q2.cpp

> **run:** ./a.out <file_path>(The file you want to do operations on)
we need to give two space seperated integers as input.
 ### Approach Used:
* Read the file from `index1` to `start` and placed it in the write file.
* Read the file from `index1` to `index2` as it is and placed in the write file.
* Read the file from `last` to `index2 `placed it in the write file. 
* To optimize it for larger files I divided the files into equal chunks and performed necessary operations in each chunk and  placed it in the file.
### Assumptions:
* The input file will not be empty and a valid .txt file.
* The input file will be having read permissions.
* The index 1 is never greater than index 2.Index 2 is never greater than file size and both indices are never negative.
 ### Output:
 It will create a new working directory called `write_file_q2` and it will have a file named `b.txt.`(_which is our required file_).

---
# 2023201012_A1_Q3.cpp
### Steps to Execute:
> **Compile:** g++ 2023201012_A1_Q3.cpp

> **run:** ./a.out <new_file_path> <old_file_path> <dir_path>
 ### Approach Used:
* Read the file from last to first for new file and first to last for old file and checked if they are matching at each character. 
* To optimize it for larger files I divided the files into equal chunks and stored the chunks in buffers and checking each buffer.
* Got all the file permissions info value using stat() library.
 ### Output:
 For each given newfile,oldfile and directory:
 
 Each of the permissions were checked and given "yes",if it was there and "no" if it was not there.
 