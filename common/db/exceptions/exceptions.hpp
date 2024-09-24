#pragma once

#include <stdexcept>
#include <string>

#include "error_codes.hpp"

namespace drug_lib::common::database::exceptions
{
    class DatabaseException : public std::runtime_error
    {
    protected:
        errors::DbErrorCode m_error_code = errors::DbErrorCode::UNKNOWN_ERROR;

    public:
        explicit DatabaseException(const std::string& message, const errors::DbErrorCode error_code) :
            std::runtime_error(message), m_error_code(error_code)
        {
        }

        [[nodiscard]] errors::DbErrorCode get_error() const { return m_error_code; }
    };

    class ConnectionException final : public DatabaseException
    {
    public:
        explicit ConnectionException(const std::string& message, const errors::DbErrorCode code)
            : DatabaseException("ConnectionException: " + message, code)
        {
        }
    };

    class QueryException final : public DatabaseException
    {
    public:
        explicit QueryException(const std::string& message, const errors::DbErrorCode code)
            : DatabaseException("QueryException: " + message, code)
        {
        }
    };

    class TransactionException final : public DatabaseException
    {
    public:
        explicit TransactionException(const std::string& message, const errors::DbErrorCode code)
            : DatabaseException("TransactionException: " + message, code)
        {
        }
    };

    class InvalidIdentifierException final : public DatabaseException
    {
    public:
        explicit InvalidIdentifierException(const std::string& message, const errors::DbErrorCode code)
            : DatabaseException("InvalidIdentifierException: " + message, code)
        {
        }
    };

    // Additional exceptions can be added here
}
