#include "rsa.h"
#include <time.h> 
#include "math.h"
#include <iostream>
#include "stdlib.h"

using namespace std;
unsigned int Rsa:: PowMod(unsigned int base, unsigned int pow, unsigned int n)
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

unsigned int Rsa:: MulMod(unsigned int a, unsigned int b, unsigned int n)
{
    return (a% n) * (b% n) % n;
}
long Rsa::RabinMillerKnl(unsigned int &n)
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

long Rsa::RabinMiller(unsigned int &n,long loop)
{
    for(long i=0; i < loop; i++) 
    {
        if(!RabinMillerKnl(n)) 
            return 0; 
    }
    return 1;
}

unsigned int Rsa::RandomPrime(int bits)
{
    unsigned int base;
    do
    {
        base = (unsigned int)1 << (bits - 1); ;
        base += rand() % base; //再加上一个随机数
        base|=1; //保证最低位是 1,即保证是奇数 
    } while(!RabinMiller(base, 30)); //进行拉宾-米勒测试 30 次
    return base; //全部通过认为是质数
}
unsigned int Rsa::ola(unsigned int n)
{
	int res = n, a = n;
	for(int i=2; i<=sqrt(a); ++i)
    {
		if(a%i==0)
        {
			res = res/i*(i-1);
			while(a%i==0) 
                a = a/i;
		}
	}
	if(a>1)
        res=res/a*(a-1);
	return res;
}

unsigned int Rsa::max_gcd(unsigned int a,unsigned int b)
{
    int t;
    if(a < b)   // 使 a > b
    {
        t = a;
        a = b;
        b = t;
    }
    if(a == b)
        return a;
    while(a % b)
    {
        t = b;
        b = a % b;
        a = t;
    }
    return b;
}
unsigned int Rsa::RandomE(unsigned int n)
{
    unsigned int b;
    do
    {
        b = 1 + rand() % (n - 1);
    } while (max_gcd(n,b)!= 1);
    
    return b;
}
void Rsa::check_key()
{
    cout<<"p："<< t_val.p<<endl;
    cout<<"q："<< t_val.q<<endl;
    cout<<"n："<<p_key.n<<endl;
    cout<<"ola(n)："<<t_val.ola_val<<endl;
    cout<<"e："<<p_key.e<<endl;
    cout<<"d："<<s_key.d<<endl;
}
void Rsa::RsaGetParam()
{
    srand((unsigned)time(0)); 
    t_val.p = RandomPrime(3);
    t_val.q = RandomPrime(3);
    p_key.n = s_key.n = t_val.p * t_val.q;

    t_val.ola_val = ola(p_key.n);
    p_key.e = RandomE(t_val.ola_val);
    s_key.d = Euclid(p_key.e,t_val.ola_val);

    //check_key();
}
 unsigned int Rsa::Euclid(unsigned int e, unsigned int t_n)
{
    unsigned long Max=0xffffffffffffffff-t_n; 
    unsigned int i=1;
    while(1) 
    {
        if(((i*t_n)+1)%e==0) 
            return ((i*t_n)+1)/e;
        i++;
        unsigned int Tmp=(i+1)*t_n; 
        if(Tmp>Max)
            return 0; 
    }
    return 0;
}
// 获取公钥
Public_key Rsa:: get_public_key()
{
    return this->p_key;
}
// 获取私钥
Secret_key Rsa:: get_secret_key()
{
    return this->s_key;
}