/*
 * 测试DES加密解密功能
 */
#include "des.h"
int main()
{
    Des des;
    string s= "romantic";
    string k = "12345678";
    cout<<"密钥："<<k<<endl;
    cout<<"内容："<<s<<endl;
    des.setKey(k);
    string cypher = des.encode(s);
    string plain = des.decode(cypher);
    cout<<"Encode:"<<cypher<<endl;
    cout<<"Decode:"<<plain<<endl;
}
