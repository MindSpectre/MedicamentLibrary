#pragma once

#include <string>
#include <openssl/sha.h>
namespace ControlSystemV7::Common
{
    class IHashCreator
    {
    protected:
        /// @brief secret key для hmac
        std::string key = "af2bdbe1aa9b6ec1e2ade1d694f41fc71a831d0268e9891562113d8a62add1bf";
    public:
        virtual std::string HashFucntion(const std::string &data, const std::string &salt) = 0;
        IHashCreator(){}
    };

}
