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
    long size = f.tellg();
    f.seekg (0);

    cout<<"size of file is "<<size<<endl ;

    ofstream of(output_file_name,ofstream::binary) ;

    unsigned char k[8] = {'S','A','M','A','P','U','K','I'} ;
    unsigned char IV[8] = {'D','E','K','I','S','U','G','I'} ;
    of.write((char *)k,8) ;
    of.write((char *)IV,8) ;
    while (!f.eof()) {
        unsigned char plain_text[9] ;

        f.read((char *)plain_text, 8) ;
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

        if(count == 0)
        {
            DES::DES_encrypt(IV,ci,k) ;
            for(unsigned i=0 ; i<8 ; i++)
            {
                y[i] = ci[i] ^ plain_text[i] ;
            }
        }
        else
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

    unsigned char prev_cipher_text[8] ;
    while (!cf.eof()) {
        unsigned char k[8] ;
        unsigned char IV[8]  ;
        unsigned char cipher_text[9] ;

        if(counter == 0)
        {
            cf.read((char *)k, 8) ;
            k[8] = 0 ;
            counter ++ ;
        }

        else if(counter == 1)
        {
            cf.read((char *)IV , 8) ;
            IV[8] = 0 ;
            counter ++ ;
        }

        else
        {
            cf.read((char *)cipher_text, 8) ;
            cipher_text[8] = 0 ;

            if (cf.gcount() == 0) {
                break ;
            }

            unsigned char ci[8] ;

            if(counter == 2)
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
            else if(counter > 2)
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
    CFB_encrypt(argv[1],"cipher.txt") ;
    CFB_decrypt("cipher.txt","decipher.txt") ;

    return 0 ;
}
