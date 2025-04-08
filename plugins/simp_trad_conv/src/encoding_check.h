#ifndef ENCODING_CHECK_H
#define ENCODING_CHECK_H

#include <string>

bool is_valid_encoding(const std::string& text, const std::string& charset);
bool is_valid_encoding_win(const std::string& text, const std::string& charset);
bool is_valid_encoding_iconv(const std::string& text, const std::string& charset);

#endif