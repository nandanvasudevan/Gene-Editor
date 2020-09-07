#ifndef COMMON_HPP
#define COMMON_HPP
#include <string>
#include <iostream>
const std::string FONT_RED_BOLD_INVERSE = "\033[1;7;31m";
const std::string FONT_RED_BOLD = "\033[1;31m";
const std::string FONT_GREEN_BOLD_INVERSE = "\033[1;7;32m";
const std::string FONT_GREEN_BOLD = "\033[1;32m";
const std::string FONT_GREEN = "\033[32m";
const std::string FONT_RESET = "\033[0m";
/* 
    void Log_ErrorMessage(const std::string sMessage, const std::string sDescription = "");
    void Log_SuccessMessage(const std::string sMessage, const std::string sDescription = ""); */
#endif