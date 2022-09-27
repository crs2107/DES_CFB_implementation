#ifndef DES_H
#define DES_H



using namespace std;

class DES 
{
public :
    static void DES_encrypt(unsigned char[8] , unsigned char[8] , unsigned char[8]) ;
    static void DES_decrypt(unsigned char[8] , unsigned char[8] , unsigned char[8]) ;

} ;

#endif

