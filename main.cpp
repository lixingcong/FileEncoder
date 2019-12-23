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

//编码转换，source_charset是源编码，to_charset是目标编码
std::string myIconv(const char* fromCharset, const char* toCharset, const std::string& str) //sourceStr是源编码字符串
{
	std::string destString;

	iconv_t cd = iconv_open(toCharset, fromCharset); //获取转换句柄，void*类型
	if ((iconv_t) -1 == cd)
		return destString;

	size_t inSize = str.size();
	if (0 == inSize)
		return destString;

	const size_t originalInSize = inSize;

	char* inBuffer = const_cast<char*>(str.c_str());

	size_t outSize    = 2 * inSize; // 2倍长度够长了吧！！
	char*  outBuffer = new char[outSize];
	memset(outBuffer, 0, outSize);
	const char* originalOutBuffer = outBuffer; // 用于delete[]操作

	if ((size_t)(-1) != iconv(cd, &inBuffer, &inSize, &outBuffer, &outSize))
		destString = std::string(originalOutBuffer, outSize);

	std::cout << "iconv: " << fromCharset << " (" << originalInSize << ") to " << toCharset << " (" << outSize << ")" << std::endl;

	// free resoure
	iconv_close(cd);
	delete[] originalOutBuffer;

	return destString;
}

int main()
{
	//1、ANSI/GBK编码
	std::string strGbk = "我";
	int    num    = strGbk.size(); //获取两个字符数，也是我字所占的字节数

	unsigned char* p = (unsigned char*) strGbk.c_str();
	for (int i = 0; i < num; i++) {
		printf("%0x", *p);
		p++;
	} //输出ced2 所以我的GBK编码是0xced2
	printf("\n");

	std::cout << myIconv("utf-8", "gb2312", "2323的") << std::endl;

	return 0;
}

