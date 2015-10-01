#include <stdio.h>
#include <stdint.h>

int const MAX_SYMBOLS=1000; 

uint8_t const sbox[8][16] = {
	{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 },
	{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10 },
	{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8 },
	{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15 },
	{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9 },
	{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11 },
	{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1 },
	{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7 }
};
/*
uint32_t f(uint32_t block) {
	uint32_t L, R = 0;
	uint8_t	S, index;

	for (int i = 7; i >= 0; i--) {
		S = sbox[i][block >> (4 * i) & 0x0f] << (4 * i);
		R |= S ;
	}

  	R = (R << 11) || 21;

   	return R;
}*/

uint32_t CreateWord (uint8_t b[4]){
	uint32_t u;

	u = b[0];
	u = (u  << 8) + b[1];
	u = (u  << 8) + b[2];
	u = (u  << 8) + b[3];

	return u;
}


uint32_t f(uint32_t block) {

	block = sbox[8][block >>28 & 0x0f] << 28 | sbox[7][block >>24 & 0x0f] << 24 |
	sbox[6][block >>20 & 0x0f] << 20 | sbox[5][block >>16 & 0x0f] << 16 |
	sbox[4][block >>12 & 0x0f] << 12 | sbox[3][block >> 8 & 0x0f] << 8 |
	sbox[2][block >> 4 & 0x0f] << 4 | sbox[1][block & 0x0f];
	
	return block<<11 | block>>(21);
}

/*
void Encrypt(unsigned char *L, unsigned char *H, unsigned char *K)
{   int i;

    for ( i = 0; i < 32; i++)
    {
        H[i+1]=L[i+1];
        L[i+1]=L[i]^F(H[i],K[i]);
    }
}
*/

void Encrypt(uint32_t in[2], uint32_t out[2], uint32_t key[8]) {
	uint32_t n1, n2; 

	n1 = in[0];
	n2 = in[1];

	for(int j = 0; j < 3; j++) {
		for(int i = 0; i < 7; i = i + 2) {
			n2 ^= f(n1 + key[i]);
			n1 ^= f(n2 + key[i + 1]);
		}
	}

	for(int i = 7; i > 0; i = i - 2) {
		n2 ^= f(n1 + key[i]);
		n1 ^= f(n2 + key[i - 1]);
	}


	out[0] = n2;
	out[1] = n1;

	printf("%d %d\n", out[0], out[1]);
}


/*
void Decrypt (unsigned char *L, unsigned char *H, unsigned char *K)
{   int i;

    for (i = 31; i >= 0; i--)
    {
        H[i+1]=H[i]^F(L[i],K[i]);
        L[i+1]=H[i+1];
    }
}*/

void Decrypt(uint32_t in[2], uint32_t out[2], uint32_t key[8]) {
	uint32_t n1, n2; 

	n1 = in[0];
	n2 = in[1];

	
	for(int i = 0; i < 7; i = i + 2) {
		n2 ^= f(n1 + key[i]);
		n1 ^= f(n2 + key[i + 1]);
	}
	

	for(int j = 0; j < 3; j++) {
		for(int i = 7; i > 0; i = i - 2) {
			n2 ^= f(n1 + key[i]);
			n1 ^= f(n2 + key[i - 1]);
		}
	}

	out[0] = n2;
	out[1] = n1;

	printf("%d %d\n", out[0], out[1]);
}
/*
void gostcfbencrypt(uint32_t const *in, uint32_t *out, int len,
	uint32_t iv[2], uint32_t const key[8]) {
	while (len--) {
		Encrypt(iv, iv, key);
		iv[0] = *out++ ^= iv[0];
		iv[1] = *out++ ^= iv[1];
	}
}
/
void gostcfbdecrypt(uint32_t const *in, uint32_t *out, int len,
uint32_t iv[2], uint32_t const key[8]) {
	uint32_t t;
	while (len--) {
		Encrypt(iv, iv, key);
		t = *out;
		*out++ ^= iv[0];
		iv[0] = t;
		t = *out;
		*out++ ^= iv[1];
		iv[1] = t;
		}
	}
*/
#define RAND32 ((uint32_t)rand() << 17 ^ (uint32_t)rand() << 9 ^ rand())

int main() {
	uint32_t key[8];
	uint32_t plain[2];
	uint32_t cipher[2];

	int i, j;
	unsigned char *input[1000] = malloc(sizeof (unsigned char)) * 1000;
	unsigned char *C[1000] = malloc(sizeof (unsigned char)) * 1000;
	unsigned char *output[1000] = malloc(sizeof (unsigned char)) * 1000;

	gets(input);
	plain[0] = CreateWord[*input];
	plain[1] = CreateWord[*input + 4];
	
	scanf("%d %d", &plain[0], &plain[1]);
		for (j = 0; j < 8; j++)
		key[j] = RAND32;
		

		printf("%3d\r", i);
		//fflush(stdout);
		Encrypt(plain, cipher, key);
		
	//C[4] = (uint8_t *) cipher[0];
	//C[4] = (uint8_t *) cipher[0];
	plain[1] = CreateWord[*input + 4];
		//for (j = 0; j < 99; j++)
			//Encrypt(cipher, cipher, key);


		//for (j = 0; j < 100; j++)
			Decrypt(cipher, cipher, key);

			if (plain[0] != cipher[0] || plain[1] != cipher[1]) {
				fprintf(stderr, "\nError! i = %d\n", i);
				return 1;
			}
	
	return 0;

}
