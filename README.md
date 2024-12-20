# m4ri_tmp
Benchmarking matrix multiplication with M4RI

**Goal**: Have a fixed matrix of size 1024x1024 bits. Generate many random vectors v of size 1024 bit. Compute A x v. Measure average cycles required using rdtscp.

Downloaded the m4ri library from https://bitbucket.org/malb/m4ri and followed the installation steps of autoreconf and make, etc. (On a Windows machine in WSL)
The files are in m4ri directory. See my.c for my implementation to test the matrix multiplication goal described above. Below I have explained my reasoning for implementating this way and the structure of the file.

Following the example file at https://malb.bitbucket.io/m4ri/testsuite_2test_multiplication_8c-example.html.
In function main:
1. Create the matrix A using mzd_init and mzd_randomize. This will be fixed
2. Generate random bits (as many as required, to be used later for random sample vectors) and store them
3. In for loop, load the random bits into a vector v  of size 1024 using mzd_write_bit
4. Multiply vector v with matrix A using _mzd_mul_va
5. Perform this for NUMBER_OF_SAMPLE many vectors and also performing warmup computation (same computation for 25% of NUMBER_OF_SAMPLES.
6. Start __rdtsc() after the warmup and print avverage cpu cycles.


Compiled it using following compiler flags (found in m4ri_config.h and manually selecting some required C file by looking at the gcc errors for references.

```bash
gcc m4ri/my.c -Im4ri m4ri/m4ri/mzd.c m4ri/m4ri/misc.c m4ri/m4ri/mmc.c m4ri/m4ri/graycode.c -march=native -mmmx -msse -msse2 -msse3 -mssse3 -msse4.1 -msse4.2 -msha -maes -mavx -mfma -mavx2  -g -O2 -o m4ri/my.exe
```


**Question** 
1. Is this correct usage of the library? For instance the functions used mzd_write_bit for loading data into mzd_t type vectors and _mzd_mul_va for multiplying vector with matrix.

**Results**
For 10,000 samples, the averege CPU cycles for one vector-matrix computation is about 40,000. (On a Windows machinew with i7-1265U 1.80 GHz processor, running WSL

**PS** The result will need to be transposed since the function is for v times A but this is not included in the code as the benchmark is already bit slower than expected, and want to verify that first.
