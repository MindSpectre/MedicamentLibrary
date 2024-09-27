#pragma once

#include <memory>

#include "hash_creator_interface.hpp"
#include "sha_256.h"



namespace drug_lib::common::hash_creator
{
    class HashCreatorFactory
    {
    public:
        static std::unique_ptr<HashCreator> CreateSHA256Coder()
        {
            return std::make_unique<SHA256Function>();
        }
    };
}
