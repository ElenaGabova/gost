#include <stdio.h>
#include <stdint.h>

uint8_t Sbox[8][16] = 
    {
        {4,10,9,2,13,8,0,14,6,11,1,12,7,15,5,3},
        {14,11,4,12,6,13,15,10,2,3,8,1,0,7,5,9},
        {5,8,1,13,10,3,4,2,14,15,12,7,6,0,9,11},
        {7,13,10,1,0,8,9,15,14,4,6,12,11,2,5,3},
        {6,12,7,1,5,15,13,8,4,10,9,14,0,3,11,2},
        {4,11,10,0,7,2,1,13,3,6,8,5,9,12,15,14},
        {13,11,4,1,3,15,5,9,0,10,14,7,6,8,2,12},
        {1,15,13,0,5,7,10,4,9,2,3,14,6,11,8,12}
    };

int f(int block, int key) {
	int L, R;
	uint8_t	S, index;

	block = (block + key) % (4294967296);

	for (int i = 0; i < 8; i++) {
		index = (uint8_t)(block >> (4 * i) & 0x0f);
		S = Sbox[i][index];
		R |= (int) S << (4 * i);
	}

  	R = (R << 11) >> 21;

   	return R;
}



void Encrypt(int * left, int * right, int rounds, int * key) {
    int i, temp;
	
    for ( i = 0; i < rounds; i++ ) {
        temp = *right ^ f( *left, key[i] );
        *right = *left;
        *left = temp;
    }
}

void Decrypt (int * left, int * right, int rounds, int * key) {
    int i, temp;

    for ( i = rounds - 1; i >= 0; i-- ){
        temp = *left ^ f( *right, key[i] );
        *left = *right;
        *right = temp;
    }
}

int main() {
	int key[] = {1, 2, 3, 4, 5, 6, 7, 8};
	int a = 1;
	int b = 2;

	Encrypt(&a, &b, 8, key);
	Decrypt(&a, &b, 8, key);

	printf("%d %d\n", a,  b);

	return 0;
}
