#include<iostream>
#include<cstdlib>
#include<fstream>
#include "DES.h"

void CFB_encrypt(const char* file_name, const char* output_file_name)
{
    int count = 0 ;
    unsigned char y[8] ;
    ifstream f (file_name, ifstream::binary) ; 
    if (!f.is_open()) return  ;
    f.seekg (0,f.end);
    long size = f.tellg(); //calculating the size of the file. Though it is not necessary to do so
    f.seekg (0);

    cout<<"size of file is "<<size<<endl ;

    ofstream of(output_file_name,ofstream::binary) ;

    unsigned char k[8] = {'S','A','M','A','P','U','K','I'} ; //initial key
    unsigned char IV[8] = {'D','E','K','I','S','U','G','I'} ;//IV
    of.write((char *)k,8) ; //First write k into the cipher.txt
    of.write((char *)IV,8) ;//second write IV into the cipher.txt
    while (!f.eof()) {
        unsigned char plain_text[9] ;

        f.read((char *)plain_text, 8) ; //reading 8 bytes of data and putting it in plain_text[8]
        plain_text[8] = 0 ;


        if (f.gcount() < 8) { //here we are doing padding following the rule written in the book
            //cout << "Read less than 8 chars. read " << f.gcount() << endl ;
            for (unsigned i = f.gcount(); i < 8 ; i++) {
                if(i == f.gcount())
                {
                    plain_text[i] = 1<<7 ;
                }
                else
                {
                    plain_text[i] = 0 ;
                }
            }
        }

        unsigned char ci[8] ;

        if(count == 0) //the first block
        {
            DES::DES_encrypt(IV,ci,k) ;
            for(unsigned i=0 ; i<8 ; i++)
            {
                y[i] = ci[i] ^ plain_text[i] ;
            }
        }
        else //The general block
        {
            DES::DES_encrypt(y,ci,k) ;
            for(unsigned i=0 ; i<8 ; i++)
            {
                y[i] = ci[i] ^ plain_text[i] ;
            }
        }

        of.write((char *)y,8) ;
        count++ ;
    }
    if(size % 8 ==0) 
    {
        unsigned char temp[8] ; 
        DES::DES_encrypt(y,temp,k) ;
        for(unsigned i=0 ; i<8 ; i++)
        {
            if(i==0)
            {
                y[i] = temp[i]^(1<<7) ;
            }
            else
            {
                y[i] = temp[i] ^ 0 ;
            }
        }
        of.write((char *)y,8) ;
        
    }
    of.close() ;
    f.close() ;


}

void CFB_decrypt(const char* file_name, const char* output_file_name)
{
    int counter = 0 ;
    unsigned char x[8] ;
    ofstream df(output_file_name,ofstream::binary) ;

    ifstream cf (file_name, ifstream::binary) ;
    if (!cf.is_open()) return  ;

    unsigned char prev_cipher_text[8] ;//keeping track of y[i-1] in the i th rouond
    while (!cf.eof()) {
        unsigned char k[8] ;
        unsigned char IV[8]  ;
        unsigned char cipher_text[9] ;

        if(counter == 0) //here I am reading the initial key k
        {
            cf.read((char *)k, 8) ;
            k[8] = 0 ;
            counter ++ ;
        }

        else if(counter == 1) //here I am reading IV
        {
            cf.read((char *)IV , 8) ;
            IV[8] = 0 ;
            counter ++ ;
        }

        else //starts decrypting
        {
            cf.read((char *)cipher_text, 8) ;
            cipher_text[8] = 0 ;

            if (cf.gcount() == 0) {
                break ;
            }

            unsigned char ci[8] ;

            if(counter == 2) //First block
            {
                DES::DES_encrypt(IV,ci,k) ;
                for(unsigned i=0 ; i<8 ; i++)
                {
                    x[i] = ci[i] ^ cipher_text[i] ;
                }
                for(unsigned i=0 ; i<8 ; i++)
                {
                    prev_cipher_text[i] = cipher_text[i] ;
                
                }
                counter++ ;
            }
            else if(counter > 2)//General block
            {
                DES::DES_encrypt(prev_cipher_text,ci,k) ;
                for(unsigned i=0 ; i<8 ; i++)
                {
                    x[i] = ci[i] ^ cipher_text[i] ;
                }
                for(unsigned i=0 ; i<8 ; i++)
                {
                    prev_cipher_text[i] = cipher_text[i] ;
                
                }
                counter ++ ;
            }


            df.write((char *)x,8) ;
        }


    }
    cf.close() ;
    df.close() ;
}

int main(int argc , char** argv)
{
    CFB_encrypt(argv[1],"cipher.txt") ; //Taking input from the terminal
    CFB_decrypt("cipher.txt","decipher.txt") ;

    return 0 ;
}
