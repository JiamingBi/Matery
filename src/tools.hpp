
#pragma once

#include <string>
#include <memory>

#include "ast.hpp"

extern std::string unescape(std::string raw);

// type cast unique pointers
template<typename TO, typename FROM>
inline std::unique_ptr<TO> static_unique_pointer_cast(std::unique_ptr<FROM>&& old) {
    return std::unique_ptr<TO>{static_cast<TO*>(old.release())};
}