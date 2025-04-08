#ifndef ENCODING_CHECK_H
#define ENCODING_CHECK_H

#include <string>

namespace simptradconv {

	bool is_valid_encoding(const std::string& text, const std::string& charset);

	#ifdef _WIN32
	bool is_valid_encoding_win(const std::string& text, const std::string& charset);
	#else
	bool is_valid_encoding_iconv(const std::string& text, const std::string& charset);
	#endif

}

#endif