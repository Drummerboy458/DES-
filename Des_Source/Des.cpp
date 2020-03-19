#include"Des.h"
//构造函数
bool Des::setKey(string key)
{
    //先判断是否为64位即8个字节
    if(sizeof(key) != 8)
        return false;
    else
        this->key = charToBitset(key);
    generateKeys(); //生成16个子密钥
    return true;
}
bitset<64> Des:: charToBitset(string s)
 {
    bitset<64> bits;
	for(int i=0; i<8; i++)
    {
		for(int j=0; j<8; j++)
		{
            bits[56 - 8 * i + j] = ((s[i]>>j) & 1);
        }
    }
	return bits;
 }
string Des:: bitSetToChar(bitset<64> src)
{
    string res;
    for(int i=0; i<8; ++i)
    {
        int value = 0;
        for(int j=0; j<8; ++j)
        {
            value += src[56 - 8 * i + j] * pow(2,j);
        }
       res += char(value);
    }
    return res;
}
//左右28位的循环移位操作函数
bitset<28> Des::leftShift(bitset<28> k, int shift)
{
    bitset<28> temp = k;
	for(int i = 27; i >= 0; i--)
	{
		if(i-shift<0)
			k[i] = temp[i-shift+28];
		else
			k[i] = temp[i-shift];
	}
	return k;
}
void Des::generateKeys()
{
    bitset<56> new_res;
    //第一步去掉奇偶校验位，64位密钥根据PC1表置换为56位
    for(int i = 0; i < 56; i++)
    {
        new_res[55 - i] = key[64 - PC_1[i]];
    }
    //第二步 16次循环，56位分为左右各28位，分别进行循环移位，拼接之后再按PC2表进行置换得到子密钥
    bitset<28> left;
    bitset<28> right;
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 28; j++)
        {
            left[27 - j] = new_res[55 - j];
        }
         for(int j = 0; j < 28; j++)
        {
            right[j] = new_res[j];
        }
        //下一步对左右两部分进行循环左移
        left = leftShift(left,ShiftBits[i]);
        right = leftShift(right,ShiftBits[i]);

        //拼接在一起
        for(int j = 0; j < 28; j++)
        {
            new_res[55 - j] = left[27 - j]; 
        }
         for(int j = 0; j < 28; j++)
        {
            new_res[j] = right[j]; 
        }
        //根据PC2表置换，去掉某些位，得到48位子密钥
        bitset<48> real_key;
        for(int j = 0; j < 48; j++)
        {
            real_key[j] = new_res[56 - PC_2[j]];
        }
        this->subKey[i] = real_key;
    }
}
//右边的32位与某一密钥k进行f运算
bitset<32> Des:: f(bitset<32> R, bitset<48> k)
{
    //第一步，将32位数据根据扩展表扩展为48位
    bitset<48> expand;
    for(int i = 0; i < 48; i++)
    {
        expand[47 - i] = R[32 - Expand[i]];
    }
    //第二步，两者异或作为S盒的输入
    expand = expand ^ k;
    //第三步，将expand分为8组，每组6bit，查找S盒置换
    //6位中高低2位作为行数，中间4位作为列数，查找对应值即为替换后的值
    //PS:行列数是从0开始的
    bitset<32> res; //S盒选择压缩后的结果
    int h = 0;
    for(int i = 0; i < 48; i += 6)
    {
        int row = expand[47 - i]*2 + expand[42 - i];
        int col = expand[47 - i - 1]*8 + expand[47 - i - 2]*4 + expand[47- i - 3]*2 + expand[47 - i - 4];
        int value = S_BOX[i / 6][row][col];
        bitset<4> temp = value;
        res[31 - h] = temp[3];
        res[31 - h - 1] = temp[2];
        res[31 - h - 2] = temp[1];
        res[31 - h - 3] = temp[0];
        h += 4;
    }
    //最后一步，将32位数进行P置换
    bitset<32> real_res;
    for(int i = 0; i < 32; i++)
    {
        real_res[31 - i] = res[32 - P[i]];
    }
    return real_res;
}
//将64位明文加密得到64位密文
bitset<64> Des:: encode_64(bitset<64> plain)
{
    bitset<64> first_ip_displace;
    //第一步是初始置换IP，位数是从左往右的，最左为第一位
    for(int i = 0; i < 64;i++)
    {
        first_ip_displace[63 - i] = plain[64 - IP[i]];
    }
    //第二步，将64位明文分成左右各32位

    bitset<32> left;
    bitset<32> right;
    for(int i = 32; i < 64;i++)
    {
        left[i-32] = first_ip_displace[i];
    }
    for(int i = 0; i < 32;i++)
    {
        right[i] = first_ip_displace[i];
    }
    //第三步 进行16轮迭代
    bitset<32> new_left;
    for(int i = 0; i < 16;i++)
    {
        //left为上一步的右边，右边是下一轮的左边和旧的右边异或的结果
        new_left = right;
        right = left ^ f(right,subKey[i]);
        left = new_left;
    }
    bitset<64> new_res;
    //第四步 合并left和right，上一步结束时left和right是相反的
    //所以，此时应该left是低32位，right是高32位
    for(int i = 0;i < 32;i++)
    {
        new_res[i] = left[i];
    }
    for(int i = 32;i < 64;i++)
    {
        new_res[i] = right[i - 32];
    }
    //最后一步，逆初始置换y=IP-1(L16R16)
    bitset<64> cypher;
    for(int i = 0; i < 64; i++)
    {
        cypher[63 - i] = new_res[64 - IP_1[i]];
    }
    //得到该64位的密文，返回
    return cypher;
}
//将64位明文加密得到64位密文
bitset<64> Des::decode_64(bitset<64> cypher)
{
    bitset<64> first_ip_displace;
    //第一步是初始置换IP，位数是从左往右的，最左为第一位
    for(int i = 0; i < 64;i++)
    {
        first_ip_displace[63 - i] = cypher[64 - IP[i]];
    }
    //第二步，将64位明文分成左右各32位

    bitset<32> left;
    bitset<32> right;
    for(int i = 32; i < 64;i++)
    {
        left[i-32] = first_ip_displace[i];
    }
    for(int i = 0; i < 32;i++)
    {
        right[i] = first_ip_displace[i];
    }
    //第三步 进行16轮迭代，解密时逆序应用子密钥
    bitset<32> old_left,new_left;
    for(int i = 0; i < 16;i++)
    {
        new_left = right;
        right = left ^ f(right,subKey[15 - i]);
        left = new_left;
    }
    bitset<64> new_res;
    //第四步 合并left和right，上一步结束时left和right是相反的
    //所以，此时应该left是低32位，right是高32位
    for(int i = 0;i < 32;i++)
    {
        new_res[i] = left[i];
    }
    for(int i = 32;i < 64;i++)
    {
        new_res[i] = right[i - 32];
    }
    //最后一步，逆初始置换y=IP-1(L16R16)
    bitset<64> plain;
    for(int i = 0; i < 64; i++)
    {
        plain[63 - i] = new_res[64 - IP_1[i]];
    }
    //得到该64位的明文，返回
    return plain;
}
string Des::encode(string plain)
{
    int size = plain.size(); //总的长度
    int round = size / 8;
    //cout<<"明文长度"<<size<<"字节"<<endl;
    int i = 0;
    int h = 0;
    string res; //加密后的结果
    while(i < round) //8字节
    {
        //分成8个字节单元的组
        string s;
        for(int j = 0; j < 8; j++)
        {
            s += plain[i * 8 + j];
        }
        bitset<64> temp = charToBitset(s);
        bitset<64> cypher = encode_64(temp);
        res += bitSetToChar(cypher);
        h += 8;
        i++;
    }
    //cout<<h<<endl;
    //剩余不足8字节,补0凑成8字节
    if(h < size)
    {
        string temp1;
        for(int i = h; i < size; i++)
        {
            temp1 += plain[i];
        }
        bitset<64> temp2 = charToBitset(temp1);
        bitset<64> cypher = encode_64(temp2);
        res += bitSetToChar(cypher);
    }
    //cout<<"Encode:"<<res<<endl;
    return res;
}
string Des::decode(string cypher)
{
    int size = cypher.size(); //总的长度
    int round = size / 8;
    //cout<<"密文长度"<<size<<"字节"<<endl;
    int i = 0;
    int h = 0;
    string res; //加密后的结果
    while(i < round) //8字节
    {
        //分成8个字节单元的组
        string s;
        for(int j = 0; j < 8; j++)
        {
            s += cypher[i * 8 + j];
        }
        bitset<64> temp = charToBitset(s);
        bitset<64> plain = decode_64(temp);
        res += bitSetToChar(plain);
        h += 8;
        i++;
    }
    //cout<<h<<endl;
    //剩余不足8字节,补0凑成8字节
    if(h < size)
    {
        string temp1;
        for(int i = h; i < size; i++)
        {
            temp1 += cypher[i];
        }
        bitset<64> temp2 = charToBitset(temp1);
        bitset<64> plain = decode_64(temp2);
        res += bitSetToChar(plain);
    }
    //cout<<"Decode:"<<res<<endl;
    return res;
}
