#pragma once

#include <string>
#include <vector>

#include "ast.hpp"

extern AST::ValueType *runtime_handler(AST::Name fn, AST::FuncCall *call, AST::SymTable *st);
extern AST::ValueType *runtime_enum_handler(AST::ValueType *vt, AST::FuncCall *call, AST::SymTable *st);
extern void runtime_bind(AST::SymTable *st);
extern void runtime_imports(std::vector<std::string> imports, AST::SymTable *st);
