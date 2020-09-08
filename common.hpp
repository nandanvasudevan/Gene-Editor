#ifndef COMMON_HPP
#define COMMON_HPP
#include <string>
#include <iostream>
const std::string FONT_RED_BOLD_INVERSE = "\033[1;7;31m";
const std::string FONT_RED_BOLD = "\033[1;31m";
const std::string FONT_GREEN_BOLD_INVERSE = "\033[1;7;32m";
const std::string FONT_GREEN_BOLD = "\033[1;32m";
const std::string FONT_GREEN = "\033[32m";
const std::string FONT_CYAN_BOLD_INVERSE = "\033[1;7;36m";
const std::string FONT_CYAN_BOLD = "\033[1;36m";
const std::string FONT_CYAN = "\033[36m";
const std::string FONT_RESET = "\033[0m";

const std::string GLYPH_CHECK_MARK = "\033[1;32m ✔ \033[0m" /* \xE2\x9C\x93" */;
const std::string GLYPH_CROSS_MARK = "\033[1; ✗" /* \xE2\x9C\x93" */;
/* 
    void Log_ErrorMessage(const std::string sMessage, const std::string sDescription = "");
    void Log_SuccessMessage(const std::string sMessage, const std::string sDescription = ""); */
#endif