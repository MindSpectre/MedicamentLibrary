#pragma once

#include <string>

namespace drug_lib::common::hash_creator
{
    class HashCreator
    {
    protected:
        /// @brief Secret key for HMAC
        std::string key;

    public:
        virtual ~HashCreator() = default;

        HashCreator() : key("af2bdbe1aa9b6ec1e2ade1d694f41fc71a831d0268e9891562113d8a62add1bf")
        {
        }

        virtual std::string hash_function(const std::string& data, const std::string& salt) = 0;

        /// @brief Sets the secret key for HMAC
        void set_key(const std::string& newKey)
        {
            key = newKey;
        }
    };
}
