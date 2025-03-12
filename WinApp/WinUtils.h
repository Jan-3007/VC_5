#pragma once


using WinError = DWORD;

using uint = unsigned int;




WinError
convert_wstring_to_char(const std::wstring& wstring, int wlen, char* ch_array, int len);
