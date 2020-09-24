# Gaussian-Elimination-with-Partial-Pivoting-Public
This program is designed for solving linear system of equations in the format of Ax = b 
where A is nxn square matrix and b is nx1 vector which consist of real valued elements. 

Machine Precision(eMACH) for this algorithm is determined to be as of the IEEE Single Precision
Floating Point Number Sytem(1 x 10^-7).  

In order to run the program:

1)You need to compile the source code file (UtkuTurkbey.cpp) with a compiler, and create 
"UtkuTurkbey.exe" executable file.
2)You need two text files of data named "A.txt" and "b.txt" to be stored in the same 
folder with the "UtkuTurkbey.exe" file. "A.txt" must include nxn matrix elements written in
the order of "a11 a12 ... a1n a21 .... a2n .... an1 .... ann"(order is strict, 
giving only one row in one line is preferred). "b.txt" must include nx1 vector elements 
written in the same order as in "A.txt". 
3)You need to run a command line, direct the locaiton to the folder mentioned in the first steps,
and type: <UtkuTurkbey.exe A.txt b.txt> without <> part and press the enter.
4)File names are arbitrary and you are free to name input files(".txt" part must remain unchaged) 
as long as all "A.txt"s and "b.txt" in part 3 updated accordingly.

Output:

1)If matrix A is singular, singularity warning will be prompt in the console and program will terminate. 
2)If A is not singular, solution vector will be written to the "soluion.txt" file inside the 
same folder with "UtkuTurkbey.exe" with the same format with "b.txt file". Also results will be prompted in the console
with the same format in "solution.txt".  
3)If A is not singular and 2x2, then condition number of A for 1 and infinity will be prompted in the console.    
