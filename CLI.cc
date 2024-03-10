#include <iostream>
#include <format>
#include <functional>
#include <fstream>

#include "Parser/Parser.hh"

void LogParserError(const std::string& msg) {
    std::cerr << msg << std::endl;

    std::exit(1);
}

class CLIMain {
public:
    CLIMain(int argc, char* argv[]) {
        for (auto i = 1; i < argc; i++) {
            std::string_view str = argv[i];

            if (str[0] == '-') {
                auto vsepp = str.find("=");
                
                if (vsepp != std::string_view::npos) {
                    auto arg = str.substr(1, vsepp - 1);
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

                        std::cout << "source: " << std::endl;

                        std::cout << "(" << std::endl << data << std::endl << ")" << std::endl;

                        auto parser = mona::Parser(data);

                        parser.registerErrorCallback(LogParserError);

                        parser.parse();
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