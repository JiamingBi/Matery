#pragma once

#include <string>
#include <memory>

#include "lexcer.hpp"
#include "ast.hpp"

extern std::unique_ptr<AST::Program> parse(scanner);