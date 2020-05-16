/* RSA类
 * 包括一些操作函数和变量
 */
#ifndef RSA_H
#define RSA_H

//1. 任意选取两个质数p和q，使得 n = p * q

// 密钥结构体
struct Public_key
{
    unsigned int n;
    unsigned int e;
};
struct Secret_key
{
    unsigned int n;
    unsigned int d;
};
struct Temp_val
{
    unsigned int p;
    unsigned int q;
    unsigned int ola_val;
};
class Rsa
{
    Public_key p_key;
    Secret_key s_key;
    Temp_val t_val;

    long RabinMillerKnl(unsigned int &n);   //Rabin-Miller 质数测试方法
    long RabinMiller(unsigned int &n, long loop=100);   // 多次重复检验，以增加验证正确的概率
    unsigned int RandomPrime(int bits);    // 生成质数
    unsigned int PowMod(unsigned int base, unsigned int pow, unsigned int n);//模幂运算
    unsigned int MulMod(unsigned int a, unsigned int b, unsigned int n); // 模乘运算
    unsigned int ola(unsigned int n);  // 求解欧拉函数值
    unsigned int max_gcd(unsigned int a,unsigned int b);  //求解两个数的最大公约数
    unsigned int RandomE(unsigned int n);   //随机产生与 n 互质，且小于 n 的数e
    unsigned int Euclid(unsigned int e, unsigned int t_n);//计算 d 的值即(d×e − 1) %ф(n) = 0

    void check_key();
public:
    Public_key get_public_key();
    Secret_key get_secret_key();  
    void RsaGetParam();   // 创建 Rsa 对象时调用，初始化参数
};


#endif