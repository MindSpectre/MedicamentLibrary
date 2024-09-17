#pragma once
#pragma GCC system_header
#include "hash_creator_interface.hpp"

namespace ControlSystemV7::Common
{
    class SHA256Function : public IHashCreator
    {
    private:
        SHA256_CTX sha256;

    public:
        SHA256Function(/* args */)
        {
            SHA256_Init(&sha256);
        }

        ~SHA256Function(){}

        /// @brief создает 16 битный хэш для полученой соли и данной строки
        /// @param data самое сообщение
        /// @param salt соль
        /// @return 16 битный хэш с использованием ключа соли и сообщения
        std::string HashFucntion(const std::string &data, const std::string &salt) override
        {
            std::string preparedData = key + data + key;
            unsigned char hash[SHA256_DIGEST_LENGTH];
            int statuscreating = SHA256_Update(&sha256, preparedData.c_str(), preparedData.length());
            int statusupdating = SHA256_Update(&sha256, salt.c_str(), salt.length());
            int statusfinalizing = SHA256_Final(hash, &sha256);
            char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1];
            for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
            {
                sprintf(&hash_hex[i * 2], "%02x", hash[i]);
            }

            return std::string(hash_hex);
        }
    };

}
