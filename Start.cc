#include <iostream>
#include <format>
#include <functional>

#include "Parser/Parser.hh"

void LogParserError(const std::string& msg) {
    std::cerr << msg << std::endl;

    std::exit(1);
}

int main(int argc, char* argv[]) {
    auto code = 
R"(    
component A {
    @serialize(1) string p1

    @serialize(2) string p2
}

component B {
    @serialize (1) string p1

    @serialize(2) string p2
}

)";

    std::cout << code << std::endl;

    auto parser = mona::Parser(code);

    parser.registerErrorCallback(LogParserError);

    parser.parse();

    return 0;
}