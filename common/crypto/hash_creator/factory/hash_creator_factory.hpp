#pragma once

#include <memory>

#include "hash_creator_interface.hpp"
#include "pbkdf2.hpp"
#include "sha_256.h"


namespace drug_lib::common::crypto
{
    class HashCreatorFactory
    {
    public:
        static std::unique_ptr<HashCreator> CreateSHA256Coder()
        {
            return std::make_unique<SHA256Function>();
        }

        static std::unique_ptr<HashCreator> CreatePBKDF2Coder()
        {
            return std::make_unique<PBKDF2Hash>();
        }
    };
}
