/*
 * %{Cpp:License:FileName}
 *
 * FIXME: 功能简要概述
 *
 * Created on: 2019年 12月 23日
 * Author: lixingcong
 */

#include <iostream>
#include <string>
#include <iconv.h>
#include <cstring>

using namespace std;

//编码转换，source_charset是源编码，to_charset是目标编码
std::string code_convert(char* source_charset, char* to_charset, const std::string& sourceStr) //sourceStr是源编码字符串
{
	iconv_t cd = iconv_open(to_charset, source_charset); //获取转换句柄，void*类型
	if (cd == 0)
		return "";

	size_t inlen  = sourceStr.size();
	size_t outlen = 255;
	char*  inbuf  = (char*) sourceStr.c_str();
	char   outbuf[255]; //这里实在不知道需要多少个字节，这是个问题
	//char *outbuf = new char[outlen]; 另外outbuf不能在堆上分配内存，否则转换失败，猜测跟iconv函数有关
	memset(outbuf, 0, outlen);

	char* poutbuf = outbuf; //多加这个转换是为了避免iconv这个函数出现char(*)[255]类型的实参与char**类型的形参不兼容
	if (iconv(cd, &inbuf, &inlen, &poutbuf, &outlen) == -1)
		return "";

	std::string strTemp(outbuf); //此时的strTemp为转换编码之后的字符串
	iconv_close(cd);
	return strTemp;
}

int main()
{
	//1、ANSI/GBK编码
	string strGbk = "我";
	int    num    = strGbk.size(); //获取两个字符数，也是我字所占的字节数

	unsigned char* p = (unsigned char*) strGbk.c_str();
	for (int i = 0; i < num; i++) {
		printf("%0x", *p);
		p++;
	} //输出ced2 所以我的GBK编码是0xced2
	printf("\n");

	return 0;
}
