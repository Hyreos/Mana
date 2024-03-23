#pragma once

#include "AST/Module.hh"

#include <filesystem>
#include <unordered_map>

namespace mana {
    class Compiler {
    public:
        using ModuleMap_T = std::unordered_map<std::filesystem::path, std::unique_ptr<ast::Module>>;
        
        Compiler();

        ast::Module* loadModule(const std::filesystem::path& path);

        ModuleMap_T& modules();
    private:
        ModuleMap_T m_loadedModules;
    };
}