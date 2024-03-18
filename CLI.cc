#include <iostream>
#include <functional>
#include <fstream>
#include <utility>

#include "Parser/Parser.hh"

void LogParserError(const std::string& msg) {
    std::cerr << "FATAL: " << msg << std::endl;
    CHECK(false);
}

class CLIMain {
public:
    CLIMain(int argc, char* argv[]) {
        for (auto i = 1; i < argc; i++) {
            std::string_view str = argv[i];

            if (str[0] == '-') {
                const char* argname = "f";
                size_t n = 1;
                
                if (str[1] == '-') {
                    argname = "file";
                    n = 2;
                }

                auto vsepp = str.find("=");
                
                if (vsepp != std::string_view::npos) {
                    auto arg = str.substr(n, vsepp - n);
                    auto value = str.substr(vsepp + 1);

                    if (arg == "file" || arg == "f") {
                        auto& file = value; 

                        std::ifstream f { std::string(file), std::ios::ate };

                        if (!f) {
                            std::cerr << "Can't open file \"" << file << "\"" << std::endl;

                            return;
                        }

                        auto sz = f.tellg();

                        f.seekg(0, std::ios::beg);

                        std::string data;
                        data.resize(static_cast<size_t>(sz));
                        
                        f.read(&data[0], data.size());

                        auto parser = mana::Parser();

                        parser.registerErrorCallback(LogParserError);

                        parser.parse(data); 
                    }
                }
            }
        }
    }
};

int main(int argc, char* argv[]) {
    CLIMain(argc, argv);

    return 0;
}