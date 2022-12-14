#pragma once

#include <string>
#include <exception>

#include "lexcer.hpp"

class ErrInfo {
 public:
    int row;
    int col;
    std::string line;
    std::string filename;
    ErrInfo() {}
    explicit ErrInfo(scanner *Scanner) : row(Scanner->row), col(Scanner->col), line(Scanner->line), filename(Scanner->filename) {}
};

class InterpreterException : public std::exception {
 public:
    std::string message;
    ErrInfo *ast;

    InterpreterException(std::string msg, ErrInfo *info) :
        message(msg), ast(info) {}
    virtual ~InterpreterException() throw() {}

    virtual const char* what() const throw();
};

extern std::string err_type_mismatch(
    std::string var, std::string ltype, std::string rtype);

extern std::string err_par_size_mismatch(
    std::string fn, int lsize, int rsize);