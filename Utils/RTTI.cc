#include "RTTI.hh"

MANA_RTTI_TYPE(mana::rtti::Base)

namespace mana::rtti {
    bool TypeMetadata::match(const TypeMetadata* metadata) const
    {
        if (metadata == &InfoStructure<Base>::data) 
            return true;

        for (const auto* ti = this;
                ti != &InfoStructure<Base>::data;
                ti = ti->base)
            if (ti == metadata) 
                return true;

        return false;
    }
}
