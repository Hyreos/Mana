#include "CCAttribute.hh"

namespace mana::ast {
    const CCAttribute* CCAttribute::clone(CloneContext& ctx) const
    {
        return ctx.create<CCAttribute>();
    }

    std::string CCAttribute::name() const
    {
        return "cc";
    }

    void CCAttribute::print(std::ostream& stream, size_t ident) const
    {
        stream << "@" << name();
    }
}