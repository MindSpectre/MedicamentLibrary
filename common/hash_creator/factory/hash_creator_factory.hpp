#pragma once

#include <memory>

#include "hash_creator_interface.hpp"
#include "sha_256.h"



namespace ControlSystemV7::Common
{
    class IHashCreatorFactory
    {
    public:
        static std::unique_ptr<IHashCreator> CreateSHA256Coder()
        {
            return std::make_unique<SHA256Function>();
        }
    };
}
