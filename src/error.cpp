#include "error.hpp"

#include <sstream>
#include <iostream>

const char *InterpreterException::what() const throw() {
    std::stringstream ss;
    if (ast)
        ss << "File \"" << ast->filename << "\" " << ast->row << ':' << ast->col << ": " << ast->line << std::endl;
    ss << "AST Error: " << message << std::endl;
    std::cout << ss.str();
    return "";
}

std::string err_type_mismatch(
    std::string var, std::string ltype, std::string rtype) {
    std::stringstream ss;
    ss << "类型不匹配 (" << var << "): " << ltype << " != " << rtype;
    return ss.str();
}

std::string err_par_size_mismatch(std::string fn, int lsize, int rsize) {
    std::stringstream ss;
    ss << "parameter size mismatch (" << fn << "): " << lsize << " != " << rsize;
    return ss.str();
}
