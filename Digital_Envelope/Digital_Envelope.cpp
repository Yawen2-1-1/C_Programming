#include<iostream>
#include<string.h>
#include<ctime> 
#include<cstdlib> 
#include<fstream>
using namespace std;
#define MAX_PRIME_SIZE 20

/************* Global stuff *************/
unsigned char AES_Sbox[256] = { 0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
						     	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
						     	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
						     	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
						     	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
						     	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
						     	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
						     	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
						     	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
						     	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
						     	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
						     	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
						     	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
						     	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
						     	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
						     	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16 };
 
unsigned char AES_xtime[256];
unsigned char AES_Sbox_Inv[256];
unsigned char AES_ShiftRowTab_Inv[16];
unsigned char AES_ShiftRowTab[] = { 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 1, 6, 11 };

/************* AES Function Prototypes *************/
void Initialize ();
void print ( unsigned char *p, int len );
void print ( int *p, int len );
void print ( string p, int len );
void AddRoundKey ( unsigned char *state, unsigned char *key );
void SubBytes ( unsigned char *state, unsigned char *sbox );
void ShiftRows ( unsigned char *state, unsigned char *shifttab );
void MixColumns ( unsigned char *state );
void Inv_MixColumns ( unsigned char *state );
int KeyExpansion ( unsigned char *key, int keyLen );
void Encryption ( unsigned char *block, unsigned char *key, int keyLen );
void Decryption ( unsigned char *block, unsigned char *key, int keyLen );

/************* RSA Function Prototypes *************/
void print(int p, int q, int n, int e, int d);
void print_in_char(int *array, int size);
void print_array(int *array, int size);
int mod(int num, int exp, int m);
int* rsa(int *array, int n, int key1, int key2);
int get_d(int e, int p, int q);
void check(int *array1, int *array2, int size);

/************* MAIN START HERE *************/
int main() {
	
/************ step1. get string to be encrypted with AES ************/
	cout << "============ step1. get string to be encrypted with AES ============" << endl << endl;
	string plaintext, temp;
	ifstream in;
	in.open("input.txt");
	while(getline(in, temp)) {
		plaintext.append(temp);
		plaintext.append("\n");
	}
	in.close();
	
	Initialize ();
 	
 	// Calculate the length of message
 	size_t i = 0; 
 	int plaintextLen = 0;
 	while ( plaintext[i] != '\0' ) {
 		++plaintextLen;
 		++i;
	 }
 	
 	// Print the original message
	printf ( "%s\n", "原始訊息：" );
	for ( i = 0; i < plaintextLen; i++ )
  		printf ( "%c", plaintext[i] );
	printf ( "\n\n" );
	system("pause");
/************ step2. get AES key ************/
	cout << "============ step2. get AES key ============" << endl << endl;
	int keyLen = 32;
	int expKeyLen = 16 * (14 + 1);
	unsigned char key[expKeyLen];
  
	for ( i = 0; i < keyLen; i++ )
		key[i] = i;
		
 	// Print the original key
	printf( "%s\n", "原始金鑰：" ); 
	print ( key, keyLen );
	
  	// Print the expanded key
	int expandKeyLen = KeyExpansion ( key, keyLen );
	printf( "%s\n", "展開金鑰：" ); 
	print ( key, expandKeyLen );
	system("pause");
/************ step3. get RSA key ************/
	cout << "============ step3. get RSA key ============" << endl << endl;
	int p, q, n, e, d;
	//ex: 31 37 7
	cout << "set p: "; cin >> p;
	cout << "set q: "; cin >> q; 
	n = p * q;
	cout << "set e: "; cin >> e; 
	if(e > (p - 1) * (q - 1)) {
		cout << "invalid e" << endl;
		return 0;
	}
	d = get_d(e, p, q);
	print(p, q, n, e, d);
	
	int array1[expandKeyLen];
	for(int i = 0; i < expandKeyLen; i++)
		array1[i] = key[i];
		
	cout << "public key: (" << e << ", " << n << ")" << endl;
	cout << "private key: (" << d << ", " << n << ")" << endl << endl;
	system("pause");
/************ step4. encrypt ************/
	cout << "============ step4. encrypt ============" << endl << endl;
	cout << "original key:" << endl << endl;
	print ( array1, expandKeyLen );
	
	int *array2 = (int*)malloc(sizeof(int) * expandKeyLen);
	cout << "after RSA encrpyting with public key: (" << e << ", " << n << "): " << endl << endl;
	array2 = rsa(array1, expandKeyLen, e, n);						//RSA 加密 
	print ( array2, expandKeyLen );
	
	unsigned char aes_after[expandKeyLen];
	for(int i = 0; i < plaintextLen; i++) aes_after[i] = plaintext[i];
	aes_after[plaintextLen] = '\0';
	
	Encryption ( aes_after, key, expandKeyLen );
	printf( "%s\n", "AES加密完後(16進位)：" ); 
	print ( aes_after, plaintextLen );
	system("pause");
/************ step5. decrypt ************/
	cout << "============ step5. decrypt ============" << endl << endl;
	int *array3 = (int*)malloc(sizeof(int) * expandKeyLen);
	cout << "after RSA decrpyting with private key: (" << d << ", " << n << ")" << endl << endl;
	array3 = rsa(array2, expandKeyLen, d, n);						//RSA 解密 
	for(int i = 0; i < expandKeyLen; i++) key[i] = array3[i];
	print ( key, expandKeyLen );
	
	Decryption ( aes_after, key, expandKeyLen );
	printf( "%s\n", "AES解密完後：" );
	for ( i = 0; i < plaintextLen; i++ )
  		printf ( "%c", plaintext[i] );
	printf ( "\n\n" );
}

/************* AES Function Definitions *************/
void Initialize () 
{
	size_t i = 0;
	
	for ( i = 0; i < 256; i++ )
    	AES_Sbox_Inv[AES_Sbox[i]] = i;
   
	for ( i = 0; i < 16; i++ )
    	AES_ShiftRowTab_Inv[AES_ShiftRowTab[i]] = i;
 
	for ( i = 0; i < 128; i++ ) {
    	AES_xtime[i] = i << 1;
    	AES_xtime[128 + i] = (i << 1) ^ 0x1b;
	}
}

// Function print: print the results
void print ( unsigned char *p, int len ) 
{
	size_t i = 0;
	for ( i = 0; i < len; i++ )
    	printf ( "%X ", p[i] );
    
	printf( "\n\n" );
}

void print ( int *p, int len ) 
{
	size_t i = 0;
	for ( i = 0; i < len; i++ )
    	printf ( "%X ", p[i] );
    
	printf( "\n\n" );
}

void print ( string p, int len ) 
{
	size_t i = 0;
	for ( i = 0; i < len; i++ )
    	printf ( "%X ", p[i] );
    
	printf( "\n\n" );
}

// Function AddRoundKey
void AddRoundKey ( unsigned char *state, unsigned char *key ) 
{
	size_t i = 0;
	
	for ( i = 0; i < 16; i++ )
    	state[i] ^= key[i];
}

// Function SubBytes
void SubBytes ( unsigned char *state, unsigned char *sbox ) 
{
	size_t i = 0;
	
	for ( i = 0; i < 16; i++ )
    	state[i] = sbox[state[i]];
}

// Function ShiftRows
void ShiftRows ( unsigned char *state, unsigned char *shifttab ) 
{
	unsigned char tmp[16] = {'\0'};
	memcpy( tmp, state, 16 );
	
	size_t i = 0;
	for ( i = 0; i < 16; i++ )
    	state[i] = tmp[shifttab[i]];
}
 
// Function MixColumns
void MixColumns ( unsigned char *state ) 
{
	size_t i = 0;
	
  	for ( i = 0; i < 16; i += 4 ) {
	    unsigned char s0 = state[i + 0];
		unsigned char s1 = state[i + 1];
	    unsigned char s2 = state[i + 2];
		unsigned char s3 = state[i + 3];
	    unsigned char h = s0 ^ s1 ^ s2 ^ s3;
	    
	    state[i + 0] ^= h ^ AES_xtime[s0 ^ s1];
	    state[i + 1] ^= h ^ AES_xtime[s1 ^ s2];
	    state[i + 2] ^= h ^ AES_xtime[s2 ^ s3];
	    state[i + 3] ^= h ^ AES_xtime[s3 ^ s0];
	}
}

// Function Inv_MixColumns
void Inv_MixColumns ( unsigned char *state ) 
{
	size_t i = 0;
	
  	for ( i = 0; i < 16; i += 4 ) {
	    unsigned char s0 = state[i + 0];
		unsigned char s1 = state[i + 1];
	    unsigned char s2 = state[i + 2];
		unsigned char s3 = state[i + 3];
	    unsigned char h = s0 ^ s1 ^ s2 ^ s3;
	    unsigned char xh = AES_xtime[h];
	    unsigned char h1 = AES_xtime[AES_xtime[xh ^ s0 ^ s2]] ^ h;
	    unsigned char h2 = AES_xtime[AES_xtime[xh ^ s1 ^ s3]] ^ h;
	    
	    state[i + 0] ^= h1 ^ AES_xtime[s0 ^ s1];
	    state[i + 1] ^= h2 ^ AES_xtime[s1 ^ s2];
	    state[i + 2] ^= h1 ^ AES_xtime[s2 ^ s3];
	    state[i + 3] ^= h2 ^ AES_xtime[s3 ^ s0];
	}
}
 
// Function KeyExpansion: generate the expanded key
int KeyExpansion ( unsigned char *key, int keyLen ) 
{
	int Rcon = 1;
	int len = keyLen;
	size_t i = 0, j = 0;
	int ks = 16 * (14 + 1);
	unsigned char temp[4] = {'\0'};
	unsigned char temp2[4] = {'\0'};
  
	for ( i = len; i < ks; i += 4 ) {
    	memcpy( temp, &key[i-4], 4 );
    	
	    if ( i % len == 0 ) {
	    	temp2[0] = AES_Sbox[temp[1]] ^ Rcon;
	    	temp2[1] = AES_Sbox[temp[2]];
	    	temp2[2] = AES_Sbox[temp[3]];
	    	temp2[3] = AES_Sbox[temp[0]];
	      
	    	memcpy( temp, temp2, 4 );
	      
	    	if ( (Rcon <<= 1) >= 256 )
	    		Rcon ^= 0x11b;
	    }
	    else if ( (len > 24) && (i % len == 16) ) {
	    	temp2[0] = AES_Sbox[temp[0]];
	    	temp2[1] = AES_Sbox[temp[1]];
	    	temp2[2] = AES_Sbox[temp[2]];
	    	temp2[3] = AES_Sbox[temp[3]];
	      
	    	memcpy( temp, temp2, 4 );
	    }
	    
	    for ( j = 0; j < 4; j++ )
	    	key[i + j] = key[i + j - len] ^ temp[j];
	}
  	
  	// return the length of expanded key
	return ks;
}
 
// Function Encryption: encrypt the plaintext,
// through AddRoundKey, SubBytes, ShiftRows, and MixColumns
void Encryption ( unsigned char *block, unsigned char *key, int keyLen )
{
	int len = keyLen;
	size_t i = 0;
  
	// First Round
	AddRoundKey ( block, &key[0] );
  
	for ( i = 16; i < len - 16; i += 16 ) {
	    SubBytes ( block, AES_Sbox );
	    ShiftRows ( block, AES_ShiftRowTab );
	    MixColumns ( block );
	    AddRoundKey ( block, &key[i] );
	}
  
	// Final Round
	SubBytes ( block, AES_Sbox );
	ShiftRows ( block, AES_ShiftRowTab );
	AddRoundKey ( block, &key[i] );
}
 
// Function Decryption: decrypt the ciphertext,
// through AddRoundKey, Inversed SubBytes, Inversed ShiftRows, and Inversed Mixcolumns
void Decryption ( unsigned char *block, unsigned char *key, int keyLen ) 
{
	int len = keyLen;
	size_t i = 0;
  
	// First Round
	AddRoundKey ( block, &key[len - 16] );
	ShiftRows ( block, AES_ShiftRowTab_Inv );
	SubBytes ( block, AES_Sbox_Inv );
  
	for(i = len - 32; i >= 16; i -= 16) {
	    AddRoundKey ( block, &key[i] );
	    Inv_MixColumns ( block );
		ShiftRows ( block, AES_ShiftRowTab_Inv );
	    SubBytes ( block, AES_Sbox_Inv );
	}
  
	// Final Round
	AddRoundKey ( block, &key[0] );
}

/************* RSA Function Definitions *************/
void print(int p, int q, int n, int e, int d) {
	cout << "p = " << p << endl;
	cout << "q = " << q << endl;
	cout << "n = " << n << endl;
	cout << "e = " << e << endl;
	cout << "d = " << d << endl;
}

int mod(int num, int exp, int m) {
	if(exp == 1) return num % m;
	int temp;
	temp = (mod(num, exp / 2, m) * mod(num, exp / 2, m)) % m;		//x^2 = x * x
	if(exp % 2 == 1) temp = (temp * (num % m)) % m;					//x^3 = x^2 * x
	//cout << num << "^" << exp << " mod " << m << " = " << temp << endl;
	return temp;
}

int* rsa(int *array, int n, int key1, int key2) {
	static int *result;
	result = (int*)malloc(sizeof(int) * n);
	for(int i = 0; i < n; i++)
		result[i] = mod(array[i], key1, key2);
	return result;
}

int get_d(int e, int p, int q) {
	int n = (p - 1) * (q - 1);
	for(int d = 2; d < n; d++) {
		if((d * e - 1) % n == 0)
			return d;
	}
}

void check(int *array1, int *array2, int size) {
	for(int i = 0; i < size; i++) {
		if(array1[i] != array2[i]) {
			cout << "RSA FAIL\n";
			return;
		}
	}
	cout << "RSA SUCCESS\n";
}
