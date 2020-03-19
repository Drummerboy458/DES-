/* DES类
 * 包括一些操作函数和变量
 */
#ifndef DES_H
#define DES_H

#include <bitset>
#include <iostream>
#include "Const.h"
#include <math.h>
using namespace std;
class Des
{
    private:
        bitset<64> key; //64位密钥
        bitset<48> subKey[16];//16轮的子密钥
    private:
        bitset<64> charToBitset(string s);//8字节大小字符数组转换成二进制
        string bitSetToChar(bitset<64> src); //64位转换为8个字符
        void generateKeys(); //生成16个子密钥
        bitset<28> leftShift(bitset<28> k, int shift);//循环移位操作
        bitset<32> f(bitset<32> R, bitset<48> k);//生成子密钥过程中的f函数
        bitset<64> encode_64(bitset<64> plain); //64位加密
        bitset<64> decode_64(bitset<64> cipher);//64位解密
    
    public:
        string encode(string plain); //流数据加密
        string decode(string cypher); //流数据解密
        bool setKey(string key); //实例化对象，设置密钥
};
#endif