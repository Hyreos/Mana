#pragma once

#include <memory>
#include <functional>

namespace mona {
    template<typename T>
    using DeletedUnique_T = std::unique_ptr<T, std::function<void(void*)>>;

    template<typename T, typename...Args>
    DeletedUnique_T<T> MakeUniquePtr(Args&&... args) {
        auto* ptr = new T(std::forward<Args>(args)...);
        
        return DeletedUnique_T<T> {
            ptr,
            [ptr](void*) {
                delete ptr;
            }
        };
    }
}