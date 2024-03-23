#include <iostream>
#include <functional>
#include <fstream>
#include <utility>

#include "Parser/Parser.hh"

#include "Compiler.hh"
#include "Resolver/DependencyGraph.hh"

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

                        mana::Compiler compiler;
                        compiler.loadModule(file);

                        std::vector<const mana::ast::Module*> modules;

                        for (auto& [path, mod] : compiler.modules()) {
                            modules.push_back(mod.get());
                        }

                        std::cout << "Number of Modules: " << modules.size() << std::endl;

                        std::cout << "================================================" << std::endl;

                        mana::DependencyGraph graph(modules);

                        /*DependencyGraph graph(modules);

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

                        for (auto& err : parser.errorList()) {
                            printf("(line: %llu, column: %llu): %s\n", err.stats.lineIndex, err.stats.columnIndex, err.message.c_str());
                        }*/
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