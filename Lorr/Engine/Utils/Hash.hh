//
// Created on Monday 18th October 2021 by e-erdal
//

#pragma once

#define HASH_INTERFACE(intf)                                                                                                                                   \
    template<>                                                                                                                                                 \
    struct eastl::hash<intf>                                                                                                                                     \
    {                                                                                                                                                          \
        size_t operator()(const intf &) const noexcept                                                                                                         \
        {                                                                                                                                                      \
            return typeid(intf).hash_code();                                                                                                                   \
        }                                                                                                                                                      \
    }

//! This method uses memcmp, operator will return wrong results if there are alignment issues!
#define OPR_INTERFACE_CMP(intf)                                                                                                                                \
    inline bool operator==(const intf &lhs, const intf &rhs)                                                                                                   \
    {                                                                                                                                                          \
        return !memcmp(&lhs, &rhs, sizeof(intf));                                                                                                              \
    }