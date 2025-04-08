#if defined(__APPLE__) || defined(__linux__)

#include "encoding_check.h"
#include <iconv.h>
#include <errno.h>
#include <vector>

bool is_valid_encoding_iconv(const std::string& text, const std::string& charset) {
    // 1. 打开转换器
    iconv_t cd = iconv_open(charset.c_str(), "UTF-8");
    if (cd == (iconv_t)-1) {
        return false;
    }

    // 2. 准备输入/输出缓冲区
    size_t in_bytes = text.size();
    char* in_buf = const_cast<char*>(text.data());
    size_t out_bytes = in_bytes * 4;  // 足够大的输出缓冲区
    std::vector<char> dummy(out_bytes);
    char* out_buf = dummy.data();  // 获取可修改的指针

    // 3. 执行转换
    bool success = (iconv(cd, &in_buf, &in_bytes, 
                         &out_buf, &out_bytes) != (size_t)-1);

    // 4. 清理资源
    iconv_close(cd);
    return success;
}

#endif