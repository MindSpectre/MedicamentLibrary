#pragma once

#include "hash_creator_interface.hpp"
#include <openssl/sha.h>

//TODO: Class are deprecated. gotta be reviewed
namespace drug_lib::common::hash_creator
{
    class SHA256Function final : public HashCreator
    {
    private:
        SHA256_CTX sha256{};

    public:
        SHA256Function(/* args */)
        {
            SHA256_Init(&sha256);
        }

        ~SHA256Function() override = default;

        /// @brief Создает 16 битный хэш для полученой соли и данной строки
        /// @param data Cамо сообщение
        /// @param salt соль
        /// @return 16 битный хэш с использованием ключа соли и сообщения
        std::string hash_function(const std::string& data, const std::string& salt) override
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

            return {hash_hex};
        }
    };
}
