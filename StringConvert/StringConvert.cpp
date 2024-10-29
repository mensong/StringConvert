#include "pch.h"
#include "StringConvert.h"
#include "win_iconv/Convertor.h"
#include "uchardet/src/uchardet.h"
#include "base64.hpp"

template<typename T>
void CopyMem(T** out, size_t* outSize, T* in, size_t inSize)
{
    (*outSize) = inSize;
    (*out) = (T*)malloc(((*outSize) + 1) * sizeof(T));
    (*out)[(*outSize)] = (T)0;
    memcpy_s((*out), ((*outSize) + 1) * sizeof(T), in, inSize * sizeof(T));
}

STRINGCONVERT_API void FreeOutStr(void* outStr)
{
    if (outStr)
		free(outStr);
}

STRINGCONVERT_API bool DetectCharset(char** outCharset, const char* inStr, size_t inSize)
{
    size_t outSize = 0;

    //判断是否为UTF-8
    char* testUtf8 = NULL;
    size_t testUtf8Len = 0;    
    bool preReturn = false;    
    if (Utf82Ansi(&testUtf8, &testUtf8Len, inStr, inSize))
    {
        bool isUtf8 = true;
        //检查两个字符串中的?是否在相同位置，如果不是，则代表Utf82Ansi转换失败
        for (size_t i = 0; i < testUtf8Len && i < inSize; i++)
        {
            if (testUtf8[i] == '?' && inStr[i] != '?')
            {//不是UTF-8，说明也不是ansi了
                isUtf8 = false;
                break;
            }
        }
        if (isUtf8)
        {
            if (strcmp(testUtf8, inStr) == 0)
                CopyMem(outCharset, &outSize, const_cast<char*>("ASCII"), 5);
            else
				CopyMem(outCharset, &outSize, const_cast<char*>("UTF-8"), 5);
            preReturn = true;
        }        
    }

    if (testUtf8)
		FreeOutStr(testUtf8);
    if (preReturn)
        return true;

	uchardet_t handle = uchardet_new();
	int retval = uchardet_handle_data(handle, inStr, inSize);
    if (retval != 0)
    {
        uchardet_delete(handle);
        return false;
    }
    uchardet_data_end(handle);

    size_t candidates = uchardet_get_n_candidates(handle);
    if (candidates < 1)
    {
        uchardet_delete(handle);
        return false;
    }

    const char* charset = uchardet_get_encoding(handle, 0);
    
    CopyMem(outCharset, &outSize, const_cast<char*>(charset), strlen(charset));

    uchardet_delete(handle);

    return true;
}

STRINGCONVERT_API bool ConvertCharset(char** outStr, size_t* outSize, const char* inStr, size_t inSize,
    const char* fromCharset, const char* toCharset, bool exact/* = true*/)
{
    std::string out;
    bool b = GL::ConvertCharset(out, std::string(inStr, inSize), fromCharset, toCharset, exact);
    if (!b) return false;

    CopyMem(outStr, outSize, &out[0], out.size());

    return b;
}

STRINGCONVERT_API bool Unicode2Ansi(char** outStr, size_t* outSize, const wchar_t* inStr, size_t inSize, const char* locale)
{
    std::string out;
    bool b = GL::Unicode2Ansi(out, std::wstring(inStr, inSize), locale);
    if (!b) return false;

    CopyMem(outStr, outSize, &out[0], out.size());

    return b;
}

STRINGCONVERT_API bool Ansi2Unicode(wchar_t** outStr, size_t* outSize, const char* inStr, size_t inSize, const char* locale)
{
    std::wstring out;
    bool b = GL::Ansi2Unicode(out, std::string(inStr, inSize), locale);
    if (!b) return false;

    CopyMem(outStr, outSize, &out[0], out.size());

    return b;
}

STRINGCONVERT_API bool Unicode2Utf8(char** outStr, size_t* outSize, const wchar_t* inStr, size_t inSize, const char* locale)
{
    std::string out;
    bool b = GL::Unicode2Utf8(out, std::wstring(inStr, inSize), locale);
    if (!b) return false;

    CopyMem(outStr, outSize, &out[0], out.size());

    return b;
}

STRINGCONVERT_API bool Utf82Unicode(wchar_t** outStr, size_t* outSize, const char* inStr, size_t inSize, const char* locale)
{
    std::wstring out;
    bool b = GL::Utf82Unicode(out, std::string(inStr, inSize), locale);
    if (!b) return false;

    CopyMem(outStr, outSize, &out[0], out.size());

    return b;
}

STRINGCONVERT_API bool Ansi2Utf8(char** outStr, size_t* outSize, const char* inStr, size_t inSize, const char* locale)
{
    std::string out;
    bool b = GL::Ansi2Utf8(out, std::string(inStr, inSize), locale);
    if (!b) return false;

    CopyMem(outStr, outSize, &out[0], out.size());

    return b;
}

STRINGCONVERT_API bool Utf82Ansi(char** outStr, size_t* outSize, const char* inStr, size_t inSize, const char* locale)
{
    std::string out;
    bool b = GL::Utf82Ansi(out, std::string(inStr, inSize), locale);
    if (!b) return false;

    CopyMem(outStr, outSize, &out[0], out.size());

    return b;
}

STRINGCONVERT_API bool Str2Hex(char** outStr, size_t* outSize, const char* inStr, size_t inSize)
{
    std::string out;
    bool b = GL::Str2Hex(out, std::string(inStr, inSize));
    if (!b) return false;

    CopyMem(outStr, outSize, &out[0], out.size());

    return b;
}

STRINGCONVERT_API bool WStr2Hex(wchar_t** outStr, size_t* outSize, const wchar_t* inStr, size_t inSize)
{
    std::wstring out;
    bool b = GL::Str2Hex(out, std::wstring(inStr, inSize));
    if (!b) return false;

    CopyMem(outStr, outSize, &out[0], out.size());

    return b;
}

STRINGCONVERT_API bool Hex2Str(char** outStr, size_t* outSize, const char* inStr, size_t inSize)
{
    std::string out;
    bool b = GL::Hex2Str(out, std::string(inStr, inSize));
    if (!b) return false;

    CopyMem(outStr, outSize, &out[0], out.size());

    return b;
}

STRINGCONVERT_API bool Hex2WStr(wchar_t** outStr, size_t* outSize, const wchar_t* inStr, size_t inSize)
{
    std::wstring out;
    bool b = GL::Hex2Str(out, std::wstring(inStr, inSize));
    if (!b) return false;

    CopyMem(outStr, outSize, &out[0], out.size());

    return b;
}

STRINGCONVERT_API void Base64Encode(char** outStr, size_t* outSize, const char* inStr, size_t inSize)
{
    std::string out = boost::beast::detail::base64_encode(std::string(inStr, inSize));

    CopyMem(outStr, outSize, &out[0], out.size());
}

STRINGCONVERT_API bool Base64Decode(char** outStr, size_t* outSize, const char* inStr, size_t inSize)
{
    std::string out = boost::beast::detail::base64_decode(std::string(inStr, inSize));

    CopyMem(outStr, outSize, &out[0], out.size());

    return true;
}
