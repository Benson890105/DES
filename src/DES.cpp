#include <iostream>
#include <bitset>
#include <string>
using namespace std;

bitset<64> key;
bitset<48> subkey[16];

int IP1[]={ 58, 50, 42, 34, 26, 18, 10, 2,
            60, 52, 44, 36, 28, 20, 12, 4,
            62, 54, 46, 38, 30, 22, 14, 6,
            64, 56, 48, 40, 32, 24, 16, 8,
            57, 49, 41, 33, 25, 17,  9, 1,
            59, 51, 43, 35, 27, 19, 11, 3,
            61, 53, 45, 37, 29, 21, 13, 5,
            63, 55, 47, 39, 31, 23, 15, 7
            };//Initial permutation

int IP2[]={ 40, 8, 48, 16, 56, 24, 64, 32,
            39, 7, 47, 15, 55, 23, 63, 31,
            38, 6, 46, 14, 54, 22, 62, 30,
            37, 5, 45, 13, 53, 21, 61, 29,
            36, 4, 44, 12, 52, 20, 60, 28,
            35, 3, 43, 11, 51, 19, 59, 27,
            34, 2, 42, 10, 50, 18, 58, 26,
            33, 1, 41,  9, 49, 17, 57, 25
            };//Final permutation

/*-----------------------------------------------------------------------------------*/

int ExpandDbox[]={ 32,  1,  2,  3,  4,  5,
             4,  5,  6,  7,  8,  9,
             8,  9, 10, 11, 12, 13,
            12, 13, 14, 15, 16, 17,
            16, 17, 18, 19, 20, 21,
            20, 21, 22, 23, 24, 25,
            24, 25, 26, 27, 28, 29,
            28, 29, 30, 31, 32,  1
            };
//Expansion D-box: 32bits to 48bits


int S_BOX[8][4][16] = {
        {
            {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
            { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
            { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
            {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6,  13}
        },//S1

        {
            {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
            { 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
            { 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
            {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}
        },//S2

        {
            {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
            {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
            {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
            {1,  10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}
        },//S3

        {
            { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
            {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
            {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
            { 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}
        },//S4

        {
            { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
            {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
            { 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
            {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}
        },//S5

        {
            {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
            {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
            { 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
            { 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}
        },//S6

        {
            { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
            {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
            { 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
            { 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}
        },//S7

        {
            {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
            { 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
            { 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
            { 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
        }//S8
};

int P[] = { 16,  7, 20, 21, 29, 12, 28, 17,
             1, 15, 23, 26,  5, 18, 31, 10,
             2,  8, 24, 14, 32, 27,  3,  9,
            19, 13, 30,  6, 22, 11,  4, 25
            };//32bits換位


/*-----------------------------------------------------------------------------------*/
int PC1[]={57, 49, 41, 33, 25, 17,  9,
             1, 58, 50, 42, 34, 26, 18,
            10,  2, 59, 51, 43, 35, 27,
            19, 11,  3, 60, 52, 44, 36,
            63, 55, 47, 39, 31, 23, 15,
             7, 62, 54, 46, 38, 30, 22,
            14,  6, 61, 53, 45, 37, 29,
            21, 13,  5, 28, 20, 12,  4
            };//將64bits key 變成56bits

int PC2[]={14, 17, 11, 24,  1,  5,
             3, 28, 15,  6, 21, 10,
            23, 19, 12,  4, 26,  8,
            16,  7, 27, 20, 13,  2,
            41, 52, 31, 37, 47, 55,
            30, 40, 51, 45, 33, 48,
            44, 49, 39, 56, 34, 53,
            46, 42, 50, 36, 29, 32
            };//壓縮換位56bits變為48bits

int shiftbits[]={1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
//每次左移的位數



/*-----------------------------------------------------------------------------------*/

bitset<32> f(bitset<32> R, bitset<48> k) {
        bitset<48> expandR;
        for(int i=0; i<48; i++){
            expandR[47-i] = R[32-ExpandDbox[i]];
        }

        expandR = expandR ^ k;

        bitset<32> output;
        int x = 0;
        for(int i=0; i<48; i=i+6){
            int row = expandR[47-i]*2 + expandR[47-i-5];
            int col = expandR[47-i-1]*8 + expandR[47-i-2]*4 + expandR[47-i-3]*2 + expandR[47-i-4];
            int num = S_BOX[i/6][row][col];
            bitset<4> binary(num);
            output[31-x] = binary[3];
            output[31-x-1] = binary[2];
            output[31-x-2] = binary[1];
            output[31-x-3] = binary[0];
            x += 4;
}
        bitset<32> tmp = output;
        for(int i=0; i<32; i++) {
        output[31-i] = tmp[32-P[i]];
}
    return output;
}//f函數


bitset<28> leftshift(bitset<28> k, int shift) {
        bitset<28> tmp = k;
        for(int i=27; i>=0; --i){
            if(i-shift<0)
                k[i] = tmp[i-shift+28];
            else k[i] = tmp[i-shift];
        }
        return k;
}//左移函數

void generatekeys() {
        bitset<56> realkey;
        bitset<28> left;
        bitset<28> right;
        bitset<48> compresskey;
        for(int i=0; i<56; i++) {
            realkey[55-i] = key[64-PC1[i]];
        }
        for(int round=0; round<16; round++) {
            for(int i=28; i<56; i++) {left[i-28] = realkey[i];}
            for(int i=0; i<28; i++) {right[i] = realkey[i];}

            left = leftshift(left, shiftbits[round]);
            right = leftshift(right, shiftbits[round]);

            for(int i=28; i<56; i++) {realkey[i] = left[i-28];}
            for(int i=0; i<28; i++) {realkey[i] = right[i];}

            for(int i=0; i<48; i++) {
                compresskey[47-i] = realkey[56-PC2[i]];
                subkey[round] = compresskey;
            }
        }
}//金鑰生成

bitset<64> charToBitset(const char s[8])
{
	bitset<64> bits;
	for(int i=0; i<8; ++i)
		for(int j=0; j<8; ++j)
			bits[i*8+j] = ((s[i]>>j) & 1);
	return bits;
}//convert char to binary

//DES 加密
bitset<64> encrypt(bitset<64>& plain) {
        bitset<64> cipher;
        bitset<64> currentBits;
        bitset<32> left;
        bitset<32> right;
        bitset<32> newLeft;
        for(int i=0; i<64; i++) {currentBits[63-i] = plain[64-IP1[i]];}

		for(int i=32; i<64; i++) {left[i-32] = currentBits[i];}
        for(int i=0; i<32; i++) {right[i] = currentBits[i];}

        for(int round=0; round<16; round++) {
            newLeft = right;
            right = left ^ f(right, subkey[round]);
            left = newLeft;
        }

        for(int i=0; i<32; i++) {cipher[i] = left[i];}
        for(int i=32; i<64; i++) {cipher[i] = right[i-32];}

        currentBits = cipher;

        for(int i=0; i<64; i++) {
            cipher[63-i] = currentBits[64-IP2[i]];
        }
return cipher;
}


//DES   解密
bitset<64> decrypt(bitset<64>& cipher) {
        bitset<64> plain;
        bitset<64> currentBits;
        bitset<32> left;
        bitset<32> right;
        bitset<32> newLeft;
        for(int i=0; i<64; i++) {currentBits[63-i] = cipher[64-IP1[i]];}

        for(int i=32; i<64; i++) {left[i-32] = currentBits[i];}
        for(int i=0; i<32; i++) {right[i] = currentBits[i];}

        for(int round=0; round<16; round++) {
            newLeft = right;
            right = left ^ f(right, subkey[15-round]);
            left = newLeft;
        }

        for(int i=0; i<32; i++) {plain[i] = left[i];}
        for(int i=32; i<64; i++) {plain[i] = right[i-32];}

        currentBits = plain;
        for(int i=0; i<64; i++) {
            plain[63-i] = currentBits[64-IP2[i]];
        }
return plain;
}


/*-----------------------------------------------------------------------------*/


int main() {
        string s;
        string k = "abcdefgh";
        string m;
        cout<<"輸入明文字串：";
        getline(cin, s);

        key = charToBitset(k.c_str());
        generatekeys();
        cout<<"輸入明文字串的大小："<<s.size()<<endl<<endl;
        cout<<"若輸入明文字串的大小不足8，則在字串後加上'0'補齊，使得字串大小為8的倍數，再送入DES作加密。"<<endl;
        cout<<"若輸入明文字串的大小剛好為8的倍數，則直接將若輸入字串送入DES作加密。"<<endl<<endl;
        while(s.size()%8 != 0) {
            s += "0";
        }

        cout<<"加密的明文字串："<<endl<<s<<endl<<endl;

        cout<<"明文的二進制(binary):"<<endl;
        for(int i=0; i < s.size()/8;  i++){
                for(int j=0; j<8; j++) {m[j] = s[i*8+j];}
                bitset<64> plain = charToBitset(m.c_str());
                cout<<plain<<endl;
        }

        cout<<endl<<"密文的二進制(binary):"<<endl;
        for(int i=0; i < s.size()/8;  i++){
                for(int j=0; j<8; j++) {m[j] = s[i*8+j];}
                bitset<64> plain = charToBitset(m.c_str());
                bitset<64> cipher = encrypt(plain);
                cout<<cipher<<endl;
            }

        cout<<endl<<"密文解密為明文的二進制(binary):"<<endl;
        for(int i=0; i < s.size()/8;  i++){
                for(int j=0; j<8; j++) {m[j] = s[i*8+j];}
                bitset<64> plain = charToBitset(m.c_str());
                bitset<64> cipher = encrypt(plain);
                bitset<64> ciphera = decrypt(cipher);
                cout<<ciphera<<endl;
        }

        cout<<endl<<"將密文解密為明文的二進制轉為字串:"<<endl;
        for(int i=0; i < s.size()/8;  i++){
                for(int j=0; j<8; j++) {m[j] = s[i*8+j];}
                bitset<64> plain = charToBitset(m.c_str());
                bitset<64> cipher = encrypt(plain);
                bitset<64> ciphera = decrypt(cipher);
                string c = (char*)&ciphera;
                string f;
                f = f.assign(c, 0, sizeof(ciphera));
                cout<<f;
        }
return 0;
}