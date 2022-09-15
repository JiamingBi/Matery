#include <iostream>
#include <fstream>
#include <filesystem>

#include "lexcer.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "error.hpp"
#include "mpp.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    /*char* ptr = (char*)mpp::MemoryAllocator::Allocate(128);
    *ptr='a';
    std::cout<<*ptr<<std::endl;*/
    std::filebuf file;
    fs::path path;
    if (argc > 1) {
        path = fs::path(argv[1]);
        if (!file.open(argv[1], std::ios::in))
            std::runtime_error("open() error: " + std::string(argv[1]));
    } else {
        path = fs::path("./sample.mt");
        if (!file.open("sample.mt", std::ios::in))
            std::runtime_error("open() error: sample.mt");
    }
    auto result_scanner = scanner(&file, path.filename().string());
    result_scanner.getnums(std::string(path));
    auto result_ast = parse(result_scanner);
    result_scanner.Free();
    AST::interpret(std::move(*result_ast));

    return 0;
}
