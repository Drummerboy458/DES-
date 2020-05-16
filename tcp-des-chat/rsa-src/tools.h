/* 
 * Rsa算法需要的一些辅助函数
 */
#ifndef TOOLS_H
#define TOOLS_H
#include "stdlib.h"
// 模乘运算
unsigned int MulMod(unsigned int a, unsigned int b, unsigned int n)
{
    return (a% n) * (b% n) % n;
}
//模幂运算
unsigned int PowMod(unsigned int base, unsigned int pow, unsigned int n)
{
    unsigned int a=base, b=pow, c=1; 
    while(b)
    {
        while(!(b & 1)) 
        {
            b>>=1;
            a=MulMod(a, a, n); 
        }
        b--;
        c=MulMod(a, c, n);
    }
    return c;
}
//Rabin-Miller 质数测试方法
long RabinMillerKnl(unsigned int &n)
{
    unsigned int a, q, k, v;
    q = n - 1;
    k=0;
    while(!(q & 1)) 
    {
        ++k;
        q>>=1; 
    }
    a=2 + rand() % (n - 3); 
    v=PowMod(a, q, n);
    if(v == 1)
        return 1;

    for(int j=0;j<k;j++) 
    {
        unsigned int z=1; 
        for(int w=0;w<j;w++) 
            z*=2; 
        if(PowMod(a, z*q, n)==n-1) 
            return 1;
    }
    return 0;
}
 // 多次重复检验，以增加验证正确的概率
long RabinMiller(unsigned int &n,long loop)
{
    for(long i=0; i < loop; i++) 
    {
        if(!RabinMillerKnl(n)) 
            return 0; 
    }
    return 1;
}


#endif