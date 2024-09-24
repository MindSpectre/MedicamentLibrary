#pragma once

#include <stdexcept>
#include <string>

namespace drug_lib::common::database::exceptions
{
    class DatabaseException : public std::runtime_error
    {
    public:
        explicit DatabaseException(const std::string& message)
            : std::runtime_error(message) {}
    };

    class ConnectionException : public DatabaseException
    {
    public:
        explicit ConnectionException(const std::string& message)
            : DatabaseException("ConnectionException: " + message) {}
    };

    class QueryException : public DatabaseException
    {
    public:
        explicit QueryException(const std::string& message)
            : DatabaseException("QueryException: " + message) {}
    };

    class TransactionException : public DatabaseException
    {
    public:
        explicit TransactionException(const std::string& message)
            : DatabaseException("TransactionException: " + message) {}
    };

    class InvalidIdentifierException : public DatabaseException
    {
    public:
        explicit InvalidIdentifierException(const std::string& message)
            : DatabaseException("InvalidIdentifierException: " + message) {}
    };

    // Additional exceptions can be added here
}
