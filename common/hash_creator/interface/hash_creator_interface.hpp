#pragma once

#include <string>

namespace drug_lib::common::hash_creator
{
    class HashCreator
    {
    protected:
        /// @brief secret key для hmac
        std::string key = "af2bdbe1aa9b6ec1e2ade1d694f41fc71a831d0268e9891562113d8a62add1bf";

    public:
        virtual ~HashCreator() = default;
        virtual std::string hash_function(const std::string& data, const std::string& salt) = 0;
        HashCreator() = default;
    };
}
