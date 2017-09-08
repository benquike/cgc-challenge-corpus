/*

Author: Joe Rogers <joe@cromulence.co>

Copyright (c) 2015 Cromulence LLC

Permission is hereby granted, __free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#include <libcgc.h>
#include "stdlib.h"
#include "stdint.h"

#define pM(matrix,x,y) (matrix + x + y*X)
#define MAX_X (20)
#define MAX_Y (20)
#define MAX_MATRIX_SIZE (255)
uint8_t X;
uint8_t Y;

void Add(int32_t *m1, int32_t *m2, int32_t *result) {
	uint8_t x, y;

	for (x = 0; x < X; x++) {
		for (y = 0; y < Y; y++) {
			*pM(result,x,y) = *pM(m1,x,y) + *pM(m2,x,y);
		}
	}
}

void Sub(int32_t *m1, int32_t *m2, int32_t *result) {
	uint8_t x, y;

	for (x = 0; x < X; x++) {
		for (y = 0; y < Y; y++) {
			*pM(result,x,y) = *pM(m1,x,y) - *pM(m2,x,y);
		}
	}
}

void ScalerMultiply(int32_t *m1, int32_t multiplier, int32_t *result) {
	uint8_t x, y;

	for (x = 0; x < X; x++) {
		for (y = 0; y < Y; y++) {
			*pM(result,x,y) = *pM(m1,x,y) * multiplier;
		}
	}
}

void PrintMenu(void) {
	__puts("Matrix Calculator");
	__puts("1. Add two matrices");
	__puts("2. Subtract two matrices");
	__puts("3. Scalar multiply a matrix");
	__puts("4. Return to main menu");
	__printf("Choice: ");

}

int32_t ReadMatrix(int32_t *m) {
	uint8_t x, y;
	char buf[8];

	if (X == 0) {
		// X
		__printf("X: ");
		if (__receive_until(buf, '\n', 7) == 0) {
			return(0);
		}
		x = __atoi(buf);	
		if (x < 1 || x > MAX_X) {
			__puts("invalid");
			return(0);
		}
	
		// Y
		__printf("Y: ");
		if (__receive_until(buf, '\n', 7) == 0) {
			return(0);
		}
		y = __atoi(buf);	
		if (y < 1 || y > MAX_Y) {
			__puts("invalid");
			return(0);
		}

		// set the globals for later use
		X = x;
		Y = y;
	}
	
#ifdef PATCHED
	uint16_t matrix_size;
#else
	// BUG: 8-bit integer overflow possible
	uint8_t matrix_size;
#endif
	matrix_size = x * y;
	if (matrix_size > MAX_MATRIX_SIZE) {
		__puts("matrix too large");
		return(0);
	}
		

	// Read in the matrix values
	__puts("Enter the matrix values");
	for (x = 0; x < X; x++) {
		for (y = 0; y < Y; y++) {
			__printf("@d,@d: ", x, y);
			if (__receive_until(buf, '\n', 7) == 0) {
				return(0);
			}
			*pM(m, x, y) = __atoi(buf);
		}
	}

	return(1);
}

void PrintMatrix(int32_t *m) {
	uint8_t x, y;

	for (x = 0; x < X; x++) {
		for (y = 0; y < Y; y++) {
			__printf("@d ", *pM(m, x, y));
		}
		__puts("");
	}

}

int32_t matrix_add(void) {
	int32_t m1[MAX_MATRIX_SIZE];
	int32_t m2[MAX_MATRIX_SIZE];
	int32_t result[MAX_MATRIX_SIZE];
	uint8_t x, y;

	X = 0;
	Y = 0;
	__puts("First matrix");
	__puts("Provide the matrix dimensions");
	if (!ReadMatrix(m1)) {
		return(0);
	}	
	__puts("Second matrix");
	if (!ReadMatrix(m2)) {
		return(0);
	}	
	Add(m1, m2, result);
	__puts("Result");
	PrintMatrix(result);

	return(1);
}
	
int32_t matrix_sub(void) {
	int32_t m1[MAX_MATRIX_SIZE];
	int32_t m2[MAX_MATRIX_SIZE];
	int32_t result[MAX_MATRIX_SIZE];
	uint8_t x, y;

	X = 0;
	Y = 0;
	__puts("First matrix");
	__puts("Provide the matrix dimensions");
	if (!ReadMatrix(m1)) {
		return(0);
	}	
	__puts("Second matrix");
	if (!ReadMatrix(m2)) {
		return(0);
	}	
	Sub(m1, m2, result);
	__puts("Result");
	PrintMatrix(result);

	return(1);
}
	
int32_t matrix_smul(void) {
	int32_t m1[MAX_MATRIX_SIZE];
	int32_t result[MAX_MATRIX_SIZE];
	uint8_t x, y;
	int32_t multiplier;
	char buf[8];

	X = 0;
	Y = 0;
	__puts("Provide the matrix dimensions");
	if (!ReadMatrix(m1)) {
		return(0);
	}	
	__printf("Multiplier: ");
	if (__receive_until(buf, '\n', 7) == 0) {
		return(0);
	}
	multiplier = __atoi(buf);	
	ScalerMultiply(m1, multiplier, result);
	__puts("Result");
	PrintMatrix(result);

	return(1);
}
	
int32_t matrix_command(char *t) {
	char buf[8];
	uint8_t choice;
	
	while (1) {
		PrintMenu();
		if (__receive_until(buf, '\n', 7) == 0) {
			return(0);
		}
		choice = __atoi(buf);
		if (choice < 1 || choice > 4) {
			continue;
		}

		switch (choice) {
			case 1:
				matrix_add();
				break;
			case 2:
				matrix_sub();
				break;
			case 3:
				matrix_smul();
				break;
			case 4:
				return(0);
		}
	}
}
