/*
 * RSA 测试函数
 */
#include "rsa.h"
#include <iostream>
using namespace std;
int main()
{
    Rsa rsa;
    rsa.RsaGetParam();
    Public_key p_key = rsa.get_public_key();
    Secret_key s_key = rsa.get_secret_key();

    string key = "12345678";
    unsigned int* encrpt_des_key = new unsigned int[key.length()];
    cout<<"DES原密钥："<<key<<endl;
    // RSA加密
    cout<<"RSA加密：";
    for(int i = 0; i < key.length();i++)
    {
        char c = key[i];
        unsigned int val = atoi(&c);
        encrpt_des_key[i] = rsa.Encry(val,p_key);
        cout<<encrpt_des_key[i]<<"\t";
    }
    cout<<endl;

    // RSA解密
    string decode_des_key = "";
    cout<<"RSA解密：";
    for(int i = 0; i < key.length();i++)
    {
          decode_des_key += to_string(rsa.Decry(encrpt_des_key[i],s_key));
    }
    cout<<decode_des_key<<endl;
}