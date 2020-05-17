#include "rsa.h"
#include "tools.h"
#include "math.h"
#include <time.h> 
#include <iostream>
using namespace std;

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
    t_val.p = RandomPrime(8);
    t_val.q = RandomPrime(8);
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
unsigned int Rsa::Encry(unsigned int des_key, Public_key cKey)
{
    return PowMod(des_key, cKey.e, cKey.n);
}
unsigned int Rsa::Decry(unsigned int encrpt_des_key, Secret_key cKey )
{
    return PowMod(encrpt_des_key, cKey.d, cKey.n);
}