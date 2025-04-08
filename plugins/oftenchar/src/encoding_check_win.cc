#ifdef _WIN32

#include "encoding_check.h"
#include <windows.h>
#include <unordered_map>

bool is_valid_encoding_win(const std::string& text, const std::string& charset) {
    static const std::unordered_map<std::string, UINT> codepages = {
        {"GBK", 936}, {"BIG5", 950}, {"UTF-8", CP_UTF8},
        {"GB2312", 936}, {"CP936", 936}, {"SHIFT-JIS", 932}
    };

    auto it = codepages.find(charset);
    if (it == codepages.end()) {
        return false;
    }

    // UTF-8 → UTF-16
    int wlen = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, nullptr, 0);
    if (wlen == 0) return false;

    std::wstring utf16(wlen, 0);
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, &utf16[0], wlen);

    // UTF-16 → 目标编码
    return WideCharToMultiByte(it->second, WC_ERR_INVALID_CHARS, 
                             utf16.c_str(), -1, nullptr, 0, nullptr, nullptr) > 0;
}

#endif