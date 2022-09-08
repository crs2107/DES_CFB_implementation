#include<iostream>
#include <cstdlib>

using namespace std ;
//macro to define the operation of getting the b th bit from the number a and left shift to the c th position
#define GET_THE_BIT(a,b,c) (((a[(b-1)/8]>>(7-((b-1)%8)))&1)<<(7-c))



int sbox[8][4][16] = {
                    {14,  4,  13,  1,   2, 15,  11,  8,   3, 10,   6, 12,   5,  9,   0,  7,
                     0, 15,   7,  4,  14,  2,  13,  1,  10,  6,  12, 11,   9,  5,   3,  8,
                     4,  1,  14,  8,  13,  6,   2, 11,  15, 12,   9,  7,   3, 10,   5,  0,
                     15, 12,   8,  2,   4,  9,   1,  7,   5, 11,   3, 14,  10,  0,   6, 13},

                    {15,  1,   8, 14,   6, 11,   3,  4,   9,  7,   2, 13,  12,  0,   5, 10,
                     3, 13,   4,  7,  15,  2,   8, 14,  12,  0,   1, 10,   6,  9,  11,  5,
                     0, 14,   7, 11,  10,  4,  13,  1,   5,  8,  12,  6,   9,  3,   2, 15,
                     13,  8,  10,  1,   3, 15,   4,  2,  11,  6,   7, 12,   0,  5,  14,  9},

                    {10,  0,   9, 14,   6,  3,  15,  5,   1, 13,  12,  7,  11,  4,   2,  8,
                    13,  7,   0,  9,   3,  4,   6, 10,   2,  8,   5, 14,  12, 11,  15,  1,
                    13,  6,   4,  9,   8, 15,   3,  0,  11,  1,   2, 12,   5, 10,  14,  7,
                    1, 10,  13,  0,   6,  9,   8,  7,   4, 15,  14,  3,  11,  5,   2, 12} ,

                    {7, 13,  14,  3,   0,  6,   9, 10,   1,  2,   8,  5,  11, 12,   4, 15,
                    13,  8,  11,  5,   6, 15,   0,  3,   4,  7,   2, 12,   1, 10,  14,  9,
                    10,  6,   9,  0,  12, 11,   7, 13,  15,  1,   3, 14,   5,  2,   8,  4,
                    3, 15,   0,  6,  10,  1,  13,  8,   9,  4,   5, 11,  12,  7,   2, 14} ,

                    {2, 12,   4,  1,   7, 10,  11,  6,   8,  5,   3, 15,  13,  0,  14,  9,
                    14, 11,   2, 12,   4,  7,  13,  1,   5,  0,  15, 10,   3,  9,   8,  6,
                    4,  2,   1, 11,  10, 13,   7,  8,  15,  9,  12,  5,   6,  3,   0, 14,
                    11,  8,  12,  7,   1, 14,   2, 13,   6, 15,   0,  9,  10,  4,   5,  3},

                    { 12,  1,  10, 15,   9,  2,   6,  8,   0, 13,   3,  4,  14,  7,   5, 11,
                    10, 15,   4,  2,   7, 12,   9,  5,   6,  1,  13, 14,   0, 11,   3,  8,
                    9, 14,  15,  5,   2,  8,  12,  3,   7,  0,   4, 10,   1, 13,  11,  6,
                    4,  3,   2, 12,   9,  5,  15, 10,  11, 14,   1,  7,   6,  0,   8, 13},

                    {4, 11,   2, 14,  15,  0,   8, 13,   3, 12,   9,  7,   5, 10,   6,  1,
                    13,  0,  11,  7,   4,  9,   1, 10,  14,  3,   5, 12,   2, 15,   8,  6,
                    1,  4,  11, 13,  12,  3,   7, 14,  10, 15,   6,  8,   0,  5,   9,  2,
                    6, 11,  13,  8,   1,  4,  10,  7,   9,  5,   0, 15,  14,  2,   3, 12},

                    {13,  2,   8,  4,   6, 15,  11,  1,  10,  9,   3, 14,   5,  0,  12,  7,
                    1, 15,  13,  8,  10,  3,   7,  4,  12,  5,   6, 11,   0, 14,   9,  2,
                    7, 11,   4,  1,   9, 12,  14,  2,   0,  6,  10, 13,  15,  3,   5,  8,
                    2,  1,  14,  7,   4, 10,   8, 13,  15, 12,   9,  0,   3,  5,   6, 11}} ;

    int IP[64] = {58, 50, 42, 34, 26, 18, 10, 2,
                  60, 52, 44, 36, 28, 20, 12, 4,
                  62, 54, 46, 38, 30, 22, 14, 6,
                  64, 56, 48, 40, 32, 24, 16, 8,
                  57, 49, 41, 33, 25, 17, 9, 1,
                  59, 51, 43, 35, 27, 19, 11, 3,
                  61, 53, 45, 37, 29, 21, 13, 5,
                  63, 55, 47, 39, 31, 23, 15, 7} ;



    int invIP[64] = {40, 8, 48, 16, 56, 24, 64, 32,
                    39, 7, 47, 15, 55, 23, 63, 31,
                    38, 6, 46, 14, 54, 22, 62, 30,
                    37, 5, 45, 13, 53, 21, 61, 29,
                    36, 4, 44, 12, 52, 20, 60, 28,
                    35, 3, 43, 11, 51, 19, 59, 27,
                    34, 2, 42, 10, 50, 18, 58, 26,
                    33, 1, 41, 9, 49, 17, 57, 25} ;


void different_permutations(int arr[64],unsigned char x[8],unsigned char y[8])
{

    int i=0 ;
    for(i=0;i<8;i++)
    {
        y[i] = GET_THE_BIT(x,arr[8*i],0)|GET_THE_BIT(x,arr[8*i+1],1)|
               GET_THE_BIT(x,arr[8*i+2],2)|GET_THE_BIT(x,arr[8*i+3],3)|
               GET_THE_BIT(x,arr[8*i+4],4)|GET_THE_BIT(x,arr[8*i+5],5)|
               GET_THE_BIT(x,arr[8*i+6],6)|GET_THE_BIT(x,arr[8*i+7],7) ;
        
    }
    /*for(unsigned j=0 ; j<8 ; j++)
    {
        cout<<"ip[ "<<j<<"]="<<(unsigned)p[j]<<endl ;
    }*/

}

void expansion_function(unsigned char r[4],  char ex[8])
{
    /*for(unsigned i=0 ; i<4 ; i++)
    {
        cout<<"r[ "<<i<<"]="<<(unsigned)r[i]<<endl ;
    }*/
    int EXP[48] = {32, 1, 2, 3, 4, 5,
                    4, 5, 6, 7, 8, 9,
                    8, 9, 10, 11, 12, 13,
                    12, 13, 14, 15, 16, 17,
                    16, 17, 18, 19, 20, 21,
                    20, 21, 22, 23, 24, 25,
                    24, 25, 26, 27, 28, 29,
                    28, 29, 30, 31, 32, 1} ;

    unsigned char c[6] ;
    
    for(unsigned i=0 ; i<6 ; i++)
    {
        c[i] = GET_THE_BIT(r,EXP[8*i],0)|GET_THE_BIT(r,EXP[8*i+1],1)|
               GET_THE_BIT(r,EXP[8*i+2],2)|GET_THE_BIT(r,EXP[8*i+3],3)|
               GET_THE_BIT(r,EXP[8*i+4],4)|GET_THE_BIT(r,EXP[8*i+5],5)|
               GET_THE_BIT(r,EXP[8*i+6],6)|GET_THE_BIT(r,EXP[8*i+7],7);
    }
    /*for(unsigned s=0 ; s<6 ; s++)
    {
        cout<<"text["<<s<<"]= "<<(unsigned)c[s]<<endl ;
    }*/
    ex[0] = c[0]>>2 ;
    ex[1] = (c[0]&0x3)|(((c[1]>>4)&0x0F)<<2) ;
    ex[2] = (c[1]&0x0F)|(((c[2]>>6)&0x3)<<4) ;
    ex[3] = c[2] & 0x3F ;
    ex[4] = c[3]>>2 ;
    ex[5] = (c[3]&0x3)|(((c[4]>>4)&0x0F)<<2) ;
    ex[6] = (c[4]&0x0F)|(((c[5]>>6)&0x3)<<4) ;
    ex[7] = c[5] & 0x3F ;
    /*for(unsigned t=0 ; t<8 ; t++)
    {
        cout<<"expaned_text["<<t<<"]= "<<(int)ex[t]<<endl ;
    }*/

}

void feeding_into_s_boxes_and_permutation(char x[8] , unsigned char f[4])
{
    /*for(unsigned i=0 ; i<8 ; i++)
    {
        cout<<"x[ "<<i<<"]="<<(int)x[i]<<endl ;
    }*/
    char y[8] ;
    
    for(unsigned i = 0; i<8 ; i++)
    {
        int col = (x[i]&0x1E)>>1 ;
        int row = (x[i]&0x1)|((x[i]>>5)<<1) ;
        y[i] = sbox[i][row][col] ;
    }
    /*for(unsigned k=0; k<8 ; k++)
    {
        cout<<"y["<<k<<"]= "<<(unsigned)y[k]<<endl ;
    }*/

    unsigned char e[4] ;

    for(unsigned j=0 ; j<4 ; j++)
    {
        e[j] = (y[2*j]<<4)|y[(2*j)+1] ;
    }
    /*for(unsigned t=0; t<4 ; t++)
    {
        cout<<"e["<<t<<"]= "<<(unsigned)e[t]<<endl ;
    }*/

    int permute[32] = {16, 7, 20, 21, 29, 12, 28, 17,
                        1, 15, 23, 26, 5, 18, 31, 10,
                        2, 8, 24, 14, 32, 27, 3, 9,
                        19, 13, 30, 6, 22, 11, 4, 25} ;

    for(unsigned j=0 ; j<4 ; j++)
    {
        f[j]= GET_THE_BIT(e,permute[8*j],0)|GET_THE_BIT(e,permute[8*j+1],1)|
              GET_THE_BIT(e,permute[8*j+2],2)|GET_THE_BIT(e,permute[8*j+3],3)|
              GET_THE_BIT(e,permute[8*j+4],4)|GET_THE_BIT(e,permute[8*j+5],5)|
              GET_THE_BIT(e,permute[8*j+6],6)|GET_THE_BIT(e,permute[8*j+7],7);

    }
    /*for(unsigned q=0; q<4 ; q++)
    {
        cout<<"f["<<q<<"]= "<<(unsigned)f[q]<<endl ;
    }*/


}


void DES_encrypt(unsigned char m[8] , unsigned char c[8])
{
    unsigned char p[8] ;
    different_permutations(IP,m,p) ;

    unsigned char l[4] , r[4] ;
    for(unsigned i=0 ; i<4 ; i++)
    {
        l[i] = p[i] ;
    }
    for(unsigned j=0 ; j<4 ; j++)
    {
        r[j] = p[j+4] ;
    }
    int count = 0 ;
    while(count<16)
    {
        char expaned_text[8] ;
        expansion_function(r,expaned_text) ;
        unsigned char f[4] ;
        feeding_into_s_boxes_and_permutation(expaned_text,f) ;
        unsigned char t[4] ;
        for(unsigned i=0 ; i<4 ; i++)
        {
            t[i] = l[i];
        }
        for(unsigned n=0 ; n<4 ; n++)
        {
            l[n] = r[n]  ;
            r[n] = t[n]^f[n] ;
        }
        count++ ;
    }


    unsigned char temp[8] ;
    for(unsigned q=0 ; q<4 ; q++)
    {
        temp[q] = r[q] ;
    }
    for(unsigned r=0 ; r<4 ; r++)
    {
        temp[r+4] = l[r] ;
    }

    different_permutations(invIP,temp,c) ;
    /*for(unsigned i=0 ; i<8 ; i++)
    {
        cout<<"c[ "<<i<<"]="<<(unsigned)c[i]<<endl ;
    }*/


}








int main()
{
    unsigned char plain_text[8] = {'S','O','U','M','E','N','D','U'} ;
    cout<<"The plain text is"<<endl ;
    for(unsigned k=0; k<8 ; k++)
    {
        cout<<""<<(char)plain_text[k] ;
    }
    cout<<endl ;

    unsigned char cipher_text[8] ;
    DES_encrypt(plain_text,cipher_text) ;

    cout<<"The cipher text is"<<endl ;
    for(unsigned t=0; t<8 ; t++)
    {
        cout<<"cipher_text["<<t<<"]= "<<(unsigned)cipher_text[t]<<endl ;
    }

    unsigned char deciphered_text[8] ;
    DES_encrypt(cipher_text,deciphered_text) ;

    cout<<"The deciphered text is"<<endl ;
    for(unsigned t=0; t<8 ; t++)
    {
        cout<<""<<(char)deciphered_text[t];
    }
    cout<<endl ;


    return 0 ;
}