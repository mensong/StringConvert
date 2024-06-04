// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "../StringConvert/StringConvert.h"

int main()
{
    const char* p = "123我爱你456";

    {
        wchar_t* ws = NULL;
        size_t wlen = 0;
        if (StringConvert::Ins().Ansi2Unicode(&ws, &wlen, p, strlen(p), ""))
        {
            char* as = NULL;
            size_t alen = 0;
            StringConvert::Ins().Unicode2Ansi(&as, &alen, ws, wlen, "");
            if (strcmp(as, p) == 0)
                std::cout << "Ansi2Unicode / Unicode2Ansi pass." << std::endl;
            else
                std::cerr << "Ansi2Unicode / Unicode2Ansi failed." << std::endl;
            StringConvert::Ins().FreeOutStr(as);
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
            StringConvert::Ins().Utf82Ansi(&as, &alen, us, ulen, "");
            if (strcmp(as, p) == 0)
                std::cout << "Ansi2Utf8 / Utf82Ansi pass." << std::endl;
            else
                std::cerr << "Ansi2Utf8 / Utf82Ansi failed." << std::endl;
            StringConvert::Ins().FreeOutStr(as);
            StringConvert::Ins().FreeOutStr(us);
        }
    }

    {
        char* cs = NULL;
        size_t cLen = 0;
        if (StringConvert::Ins().ConvertCharset(&cs, &cLen, p, strlen(p),"UTF8",  "GB2312", false))
        {

            StringConvert::Ins().FreeOutStr(cs);
        }
    }


    return 0;
}
