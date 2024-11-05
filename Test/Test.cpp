// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include "../StringConvert/StringConvert.h"

int main()
{
    std::string f;
    {
		// 创建输入文件流对象  
		std::ifstream infile("example.txt"); // 替换成你的文件名  
		std::string line;
		// 逐行读取文件内容  
		while (std::getline(infile, line))
		{
			if (f.empty())
			{
				f = line;
			}
			else
			{
				f += '\n';
				f += line;
			}
		}
		// 关闭文件  
		infile.close(); // 可选，析构时会自动关闭  
    }
    if (f.empty())
        f = "我愛你123abc√";
    const char* p = f.c_str();

    {
        char* fromCharset = NULL;
        if (StringConvert::Ins().DetectCharset(&fromCharset, p, strlen(p)))
        {
            char* us = NULL;
            size_t uLen = 0;
            if (StringConvert::Ins().ConvertCharset(&us, &uLen, p, strlen(p), fromCharset, "UTF-8", true))
            {
                char* as = NULL;
                size_t aLen = 0;        
                if (StringConvert::Ins().ConvertCharset(&as, &aLen, us, uLen, "UTF-8", fromCharset, true))
                {
                    if (strcmp(as, p) == 0)
                        std::cout << "[√]DetectCharset/ConvertCharset pass." << std::endl;
                    else
                        std::cerr << "[×]DetectCharset/ConvertCharset failed." << std::endl;
                    StringConvert::Ins().FreeOutStr(as);
                }

                StringConvert::Ins().FreeOutStr(us);
            }

            if (strcmp(fromCharset, "UTF-8") == 0)
            {
                char* as = NULL;
                size_t aLen = 0;
                if (StringConvert::Ins().Utf82Ansi(&as, &aLen, p, strlen(p), ""))
                {
                    f.assign(as, aLen);
                    p = f.c_str();
                    StringConvert::Ins().FreeOutStr(as);
                }
            }

            StringConvert::Ins().FreeOutStr(fromCharset);
        }
    }

    {
        wchar_t* ws = NULL;
        size_t wlen = 0;
        if (StringConvert::Ins().Ansi2Unicode(&ws, &wlen, p, strlen(p), ""))
        {
            char* as = NULL;
            size_t alen = 0;
            if (StringConvert::Ins().Unicode2Ansi(&as, &alen, ws, wlen, ""))
            {
                if (strcmp(as, p) == 0)
                    std::cout << "[√]Ansi2Unicode/Unicode2Ansi pass." << std::endl;
                else
                    std::cerr << "[×]Ansi2Unicode/Unicode2Ansi failed." << std::endl;
                StringConvert::Ins().FreeOutStr(as);
            }

            char* us = NULL;
            size_t ulen = 0;
            if (StringConvert::Ins().Unicode2Utf8(&us, &ulen, ws, wlen, ""))
            {
                wchar_t* ws2 = NULL;
                size_t wLen2 = 0;
                if (StringConvert::Ins().Utf82Unicode(&ws2, &wLen2, us, ulen, ""))
                {
                    if (wcscmp(ws2, ws) == 0)
                        std::cout << "[√]Unicode2Utf8/Utf82Unicode pass." << std::endl;
                    else
                        std::cerr << "[×]Unicode2Utf8/Utf82Unicode failed." << std::endl;

                    StringConvert::Ins().FreeOutStr(ws2);
                }

                StringConvert::Ins().FreeOutStr(us);
            }

            StringConvert::Ins().FreeOutStr(ws);
        }
    }

    {
        char* us = NULL;
        size_t ulen = 0;
        if (StringConvert::Ins().Ansi2Utf8(&us, &ulen, p, strlen(p), ""))
        {
            char* as = NULL;
            size_t alen = 0;
            if (StringConvert::Ins().Utf82Ansi(&as, &alen, us, ulen, ""))
            {
                if (strcmp(as, p) == 0)
                    std::cout << "[√]Ansi2Utf8/Utf82Ansi pass." << std::endl;
                else
                    std::cerr << "[×]Ansi2Utf8/Utf82Ansi failed." << std::endl;
                StringConvert::Ins().FreeOutStr(as);
            }

            StringConvert::Ins().FreeOutStr(us);
        }
    }

    {
        char* cs = NULL;
        size_t cLen = 0;
		if (StringConvert::Ins().ConvertCharset(&cs, &cLen, p, strlen(p), "UTF8", "GB2312", false))
        {
            std::cout << "[√]ConvertCharset exact pass." << std::endl;

            StringConvert::Ins().FreeOutStr(cs);
        }
    }


    {
        char* p64 = NULL;
        size_t len64 = 0;
        StringConvert::Ins().Base64Encode(&p64, &len64, p, strlen(p));
        char* p64_1 = NULL;
        size_t len64_1 = 0;
        if (StringConvert::Ins().Base64Decode(&p64_1, &len64_1, p64, len64))
        {
            if (strcmp(p64_1, p) == 0)
                std::cout << "[√]Base64Encode/Base64Decode pass." << std::endl;
            else
                std::cerr << "[×]Base64Encode/Base64Decode failed." << std::endl;

            StringConvert::Ins().FreeOutStr(p64_1);
        }
        StringConvert::Ins().FreeOutStr(p64);
    }

    {
        char* pHex = NULL;
        size_t hexLen = 0;
        if (StringConvert::Ins().Str2Hex(&pHex, &hexLen, p, strlen(p)))
        {
            char* hs = NULL;
            size_t hsLen = 0;
            if (StringConvert::Ins().Hex2Str(&hs, &hsLen, pHex, hexLen))
            {
                if (strcmp(hs, p) == 0)
                    std::cout << "[√]Str2Hex/Hex2Str pass." << std::endl;
                else
                    std::cerr << "[×]Str2Hex/Hex2Str failed." << std::endl;

                StringConvert::Ins().FreeOutStr(hs);
            }
            StringConvert::Ins().FreeOutStr(pHex);
        }        
    }

    return 0;
}
