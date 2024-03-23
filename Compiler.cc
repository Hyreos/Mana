#include "Compiler.hh"
#include "Parser/Parser.hh"
#include "Resolver/Resolver.hh"

#include <fstream>
#include <string>

namespace mana {
    Compiler::Compiler()
    {
    }

    Compiler::ModuleMap_T& Compiler::modules()
    {
        return m_loadedModules;
    }

    ast::Module* Compiler::loadModule(const std::filesystem::path& path) {
        // If this module was already loaded, then retrieve it from hashmap
        if (auto it = m_loadedModules.find(path); it != m_loadedModules.end()) return it->second.get();

        Parser parser;

        std::ifstream f { path.c_str(), std::ios::ate };

        if (!f) {
            std::cerr << "Can't open file " << path << std::endl;

            return nullptr;
        }

        auto sz = f.tellg();
        f.seekg(0, std::ios::beg);

        std::string data;
        data.resize(static_cast<size_t>(sz));
        
        f.read(&data[0], data.size());

        auto lmod = parser.parse(data);

        Resolver resolver(this);
        resolver.resolve(lmod.get());

        auto result = lmod.get();
        m_loadedModules[path] = std::move(lmod);
    
        return result;
    }
}