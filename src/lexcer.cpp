#include "lexcer.hpp"

#include <string>
#include <iostream>

#include "tools.hpp"

void scanner::next(void) {
    if (!this->input->get(this->c)) {
        this->c = '\0';
        return;
    }
    if ((c == '\n') || (c == '\r')) {
        this->line = "";
        row++;
        col = 0;
    } else {
        if ((this->line.size() != 0) || ((c != '\t') && (c != ' ')))
            this->line += c;
        col++;
    }
}

token scanner::scan(void) {
    this->data = "";
    while (isspace(c) || (c == '\n') || (c == '\r')) next();
    switch (c) {
        case '\0': {
            return t_eof;
        }
        case '"': {
            // string
            do {
                data += c;
                next();
                if (c == '\\') {
                    data += c;
                    next();
                    data += c;
                    next();
                }
            } while (c != '"');
            data += c;
            next();

            data = unescape(data);
            return t_str;
        }
        case '\'': {
            // char
            next();
            if (c == '\\') {
                next();
                if (c == 'n') c = '\n';
                if (c == 'r') c = '\r';
            }
            data += c;
            next();
            if (c != '\'') {
                throw std::runtime_error(
                    "scanner: unknown token " + std::string(1, c));
            }
            next();
            return t_char;
        }
        case '#': {
            // comment
            while (c != '\n') {
                next();
            }
            return scan();
        }
        case '(': {
            next();
            return lpar;
        }
        case ')': {
            next();
            return rpar;
        }
        case '{': {
            next();
            return lbra;
        }
        case '}': {
            next();
            return rbra;
        }
        case '[': {
            next();
            return larr;
        }
        case ']': {
            next();
            return rarr;
        }
        case '=': {
            next();
            if (c == '=') {
                next();
                return equ;
            } else {
                return move;
            }
        }
        case '!': {
            next();
            if (c == '=') {
                next();
                return neq;
            } else {
                throw "Scanner: unknown character: '!'";
            }
        }
        case '>': {
            next();
            if (c == '=') {
                next();
                return ge;
            } else {
                return gt;
            }
        }
        case '<': {
            next();
            if (c == '=') {
                next();
                return le;
            } else {
                return lt;
            }
        }
        case '|': {
            next();
            if (c == '|') {
                next();
                return lor;
            } else {
                return bor;
            }
        }
        case '&': {
            next();
            if (c == '&') {
                next();
                return land;
            } else {
                return band;
            }
        }
        case '^': {
            next();
            return bxor;
        }
        case '+': {
            next();
            return add;
        }
        case '-': {
            next();
            return sub;
        }
        case '*': {
            next();
            return mul;
        }
        case '/': {
            next();
            if(c=='*'){
               // std::cout<<this->row<<" "<<this->nums<<std::endl;
                while(this->row<=this->nums){
                    next();
                   // std::cout<<"jajaj"<<std::endl;
                    if(c=='*'){
                        next();
                        if(c=='/'){
                            next();
                            return scan();
                        }
                    }
                }
                throw std::runtime_error("Parser Error: can't match /*");
            }
            return t_div;
        }
        case '%': {
            next();
            return rem;
        }
        case '.': {
            next();
            return dot;
        }
        case ',': {
            next();
            return comma;
        }
        case '`': {
            next();
            return gen;
        }
        case ':': {
            next();
            if (c == '=') {
                next();
                return copy;
            } else {
                return colon;
            }
        }
        case ';': {
            next();
            return eol;
        }
        default: {
            if (isdigit(c)) {
                data += c;
                bool is_float = false;
                next();
                while (isdigit(c) || ((c == '.') && (!is_float))) {
                    data += c;
                    if (c == '.') is_float = true;
                    next();
                }
                if (is_float)
                    return t_float;
                else
                    return t_int;
            }
            if (isalpha(c) || (c == '_')) {
                data += c;
                next();
                while (isalpha(c) || isdigit(c) || (c == '_')) {
                    data += c;
                    next();
                }
                if (data == "import") return t_import;
                if (data == "var") return t_var;
                if (data == "const") return t_const;
                if (data == "def") return t_fn;
                if (data == "class") return t_class;
                if (data == "struct") return t_enum;
                if (data == "union") return t_union;
                if (data == "if") return t_if;
                if (data == "else") return t_else;
                if (data == "match") return t_match;
                if (data == "while") return t_while;
                if (data == "for") return t_for;
                if (data == "break") return t_break;
                if (data == "continue") return t_continue;
                if (data == "return") return t_return;
                if (data == "void") return type_void;
                if (data == "bool") return type_bool;
                if (data == "int") return type_int32;
                if (data == "uint8") return type_uint8;
                if (data == "char") return type_char;
                if (data == "float") return type_fp32;
                if (data == "double") return type_fp64;
                if (data == "string") return type_str;
                return t_name;
            }
            LogError("unknown character: '" << c << "'" << (int)c << ')');
            return t_eof;
        }
    }
}
