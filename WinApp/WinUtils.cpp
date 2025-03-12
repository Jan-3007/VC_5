#include "VC5_global.h"



WinError
convert_wstring_to_char(const std::wstring& wstring, int wlen, char* ch_array, int len)
{
    const wchar_t* wstr = wstring.c_str();

    int written = WideCharToMultiByte(
        CP_UTF8,                            // [in]            UINT                               CodePage,
        0,                                  // [in]            DWORD                              dwFlags,
        wstr,                               // [in]            _In_NLS_string_(cchWideChar)LPCWCH lpWideCharStr,
        wlen,                               // [in]            int                                cchWideChar,
        ch_array,                           // [out, optional] LPSTR                              lpMultiByteStr,
        len,                                // [in]            int                                cbMultiByte,
        NULL,                               // [in, optional]  LPCCH                              lpDefaultChar,
        NULL                                // [out, optional] LPBOOL                             lpUsedDefaultChar
    );
    if(written == 0)
    {
        WinError err = GetLastError();
        // WideCharToMultiByte failed
        std::cout << std::format("{}: WideCharToMultiByte failed: {}\n", __func__, err);
        return ERROR_ERRORS_ENCOUNTERED;
    }

    return NO_ERROR;
}
