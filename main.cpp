/*
 * Created on: 2019年 12月 23日
 * Author: lixingcong
 */

#include <iostream>
#include <string>
#include <iconv.h>
#include <cstring>
#include <fstream>
#include <regex>
#include <vector>
#include <sstream>

#if defined(_MSC_VER) && (_MSC_VER > 1600)
#pragma execution_character_set("utf-8")
#endif

// 编码转换，fromCharset是源编码，toCharset是目标编码
std::string myIconv(const char* fromCharset, const char* toCharset, const std::string& str) //sourceStr是源编码字符串
{
	size_t inSize = str.size();
	if (0 == inSize)
		return std::string();

	iconv_t cd = iconv_open(toCharset, fromCharset); //获取转换句柄，void*类型
	if ((iconv_t) -1 == cd)
		return std::string();

#if defined(__linux__)
	char* inBuffer = const_cast<char*>(str.c_str());
#elif defined(_WIN32)
	const char* inBuffer = str.c_str();
#else
	std::cerr << "Unsupport OS!" << std::endl;
	return std::string();
#endif

	const size_t originalInSize = inSize;
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

// C++11 std::ECMAScript syntax
std::string myRegex(const char* findRegex, const char* replaceTo, const std::string& str)
{
	std::cout << "Replacing: " << findRegex << " ---> " << replaceTo << std::endl;
	std::regex re(findRegex);
	return std::regex_replace(str, re, replaceTo, std::regex_constants::format_default);
}

void printHelp(const char* exe)
{
	std::cout << exe << " SRC DEST SRC_CHARSET DEST_CHARSET [REGEX REPLACE_TO]" << std::endl;
	std::cout << "\t" << "[REGEX REPLACE_TO] should be a array, or could be empty" << std::endl;
	std::cout << "\t\t" << "abc:xyz;123:456" << std::endl;
	std::cout << "\t\t" << "it means find 'abc' then replace to 'xyz', and find '123' then replace to '456'" << std::endl;
	std::cout << std::endl;
	std::cout << "\t" << "full example:" << std::endl;
	std::cout << "\t" << exe << " /tmp/src.txt /tmp/dest.txt gbk2312 utf-8 123:567;ABC:DEF" << std::endl;
	std::cout << "\t" << exe << " /tmp/src.txt /tmp/dest.txt gbk2312 utf-8" << std::endl;
}

struct ReplacePair
{
	std::string findRegex;
	std::string replaceTo;
};

bool ifFileExisted(const char* filename) {
	std::ifstream f(filename);
	return f.good();
}

int main(int argc, char** argv)
{
	// Retrieve the (non-option) argument:
	if ((argc < 5) || (argv[argc - 1] == nullptr)) {  // there is NO input...
		printHelp(argv[0]);
		return 1;
	}

	const char* srcFileName = argv[1];
	const char* destFileName = argv[2];
	const char* fromCharset = argv[3];
	const char* toCharset = argv[4];
	std::vector<ReplacePair> replaceStrPairs;

	if (6 == argc){
		std::string replaceParams(argv[5]);
		std::replace(replaceParams.begin(), replaceParams.end(), ';', ' ');  // replace ';' by ' '
		std::stringstream ss1(replaceParams);
		std::string replacement;
		while (ss1 >> replacement){
			std::replace(replacement.begin(), replacement.end(), ':', ' ');  // replace ':' by ' '
			std::stringstream ss2(replacement);

			ReplacePair p;
			ss2 >> p.findRegex >> p.replaceTo;
			replaceStrPairs.push_back(p);
		}
	}

	if (!ifFileExisted(srcFileName)){
		std::cerr << "Open " << srcFileName << " FAIL" << std::endl;
		return 1;
	}

	std::ifstream ifs(srcFileName);
	std::string   inContent((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	std::string outContent = myIconv(fromCharset, toCharset, inContent);

	for (auto it = replaceStrPairs.begin(); it != replaceStrPairs.end(); ++it){
		outContent = myRegex((*it).findRegex.c_str(), (*it).replaceTo.c_str(), outContent);
	}

	std::ofstream ofs(destFileName);
	ofs << outContent;

	return 0;
}

