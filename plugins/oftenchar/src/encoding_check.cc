#include "encoding_check.h"

namespace oftenchar {

    bool is_valid_encoding(const std::string& text, const std::string& charset) {
        if (text.empty() || charset.empty()) {
            return false;
        }

    #ifdef _WIN32
        return is_valid_encoding_win(text, charset);
    #else
        return is_valid_encoding_iconv(text, charset);
    #endif
    }

} // namespace