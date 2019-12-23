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
#include <fstream>

// 编码转换，fromCharset是源编码，toCharset是目标编码
std::string myIconv(const char* fromCharset, const char* toCharset, const std::string& str) //sourceStr是源编码字符串
{
	size_t inSize = str.size();
	if (0 == inSize)
		return std::string();

	iconv_t cd = iconv_open(toCharset, fromCharset); //获取转换句柄，void*类型
	if ((iconv_t) -1 == cd)
		return std::string();

	const size_t originalInSize = inSize;

	char* inBuffer = const_cast<char*>(str.c_str());

	size_t outSize    = 2 * inSize; // 2倍长度够长了吧！！
	char*  outBuffer = new char[outSize];
	memset(outBuffer, 0, outSize);
	const char* originalOutBuffer = outBuffer; // 用于delete[]操作

	std::string destString;
	if ((size_t)(-1) != iconv(cd, &inBuffer, &inSize, &outBuffer, &outSize))
		destString = std::string(originalOutBuffer);

	std::cout << "iconv: " << fromCharset << " (" << originalInSize << " Bytes) to " << toCharset << " (" << destString.size() << " Bytes)" << std::endl;

	// free resoure
	iconv_close(cd);
	delete[] originalOutBuffer;

	return destString;
}

int main(int argc, char** argv)
{
	std::ifstream ifs("/tmp/gbk.txt");
	std::string   inContent((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	std::string outContent = myIconv("gb2312", "utf-8", inContent);
	//std::cout << myIconv("utf-8", "gb2312", content) << std::endl;
	//std::cout << myIconv("gb2312", "utf-8", content) << std::endl;

	std::ofstream ofs("/tmp/result.txt");
	ofs << outContent;

	return 0;
}

