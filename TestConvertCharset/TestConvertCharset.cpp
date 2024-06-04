// TestConvertCharset.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "../StringConvert/StringConvert.h"

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cout << "\"input file\" \"output file\" \"output charset\"" << std::endl;
        return 1;
    }

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    const char* outputCharset = argv[3];

#define BUFFER_SIZE 2048
    char buffer[BUFFER_SIZE];
    FILE* fp = fopen(inputFile, "r");
    if (!fp)
    {
        std::cerr << inputFile  << " can not read" << std::endl;
        return 2;
    }

    std::string fileContext;
    while (1)
    {
        size_t len = fread(buffer, 1, BUFFER_SIZE, fp);
        if (len == 0)
            break;
        fileContext.append(buffer, len);
    }
    fclose(fp);

    std::string charset;
    char* inputCharset = NULL;
    if (StringConvert::Ins().DetectCharset(&inputCharset, &fileContext[0], fileContext.size()))
    {
        charset = inputCharset;
        std::cout << "input=" << inputFile << " charset=" << inputCharset << std::endl;
        StringConvert::Ins().FreeOutStr(inputCharset);
    }

    char* output = NULL;
    size_t outputLen = 0;
    bool res = StringConvert::Ins().ConvertCharset(&output, &outputLen, &fileContext[0], fileContext.size(), 
        &charset[0], outputCharset, true);
    if (!res)
    {
        std::cerr << "Convert charset error" << std::endl;
        return 3;
    }

    fp = fopen(outputFile, "w");
    if (!fp)
    {
        std::cerr << outputFile << " can not write" << std::endl;
        return 4;
    }
    fwrite(output, 1, outputLen, fp);
    fclose(fp);

    std::cout << "Convert success: output=" << outputFile << " charset=" << outputCharset << std::endl;

    StringConvert::Ins().FreeOutStr(output);

    return 0;
}
