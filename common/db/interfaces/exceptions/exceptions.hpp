#pragma once

#include <string>
#include <exception>
#include "StationDatabaseException.h"
namespace ControlSystemV7::Common::Services::Exceptions
{

    class TableExceptions : public std::exception
    {
    public:
        explicit TableExceptions(const std::string &msg) : msg_(msg) {}
        const char *what() const noexcept override { return msg_.c_str(); }
        int GetErrorCode() const
        {
            return static_cast<int>(error);
        }
        void SetErrorCode(ControlSystemV7::Common::Database::Exceptions::DBErrorCode errortype)
        {
            error = errortype;
        }
        void SetErrorCode(int errortype)
        {
            error = static_cast<ControlSystemV7::Common::Database::Exceptions::DBErrorCode>(errortype);
        }

    private:
        ControlSystemV7::Common::Database::Exceptions::DBErrorCode error = ControlSystemV7::Common::Database::Exceptions::DBErrorCode::UnknownError;
        std::string msg_;
    };

    class RecordExistenceException : public TableExceptions
    {
    public:
        explicit RecordExistenceException(const std::string &msg) : TableExceptions(msg) {}
    };

    class DatabaseException : public TableExceptions
    {
    public:
        explicit DatabaseException(const std::string &msg) : TableExceptions(msg) {}
    };
    class InvalidDataReceiven : public TableExceptions
    {
    public:
        explicit InvalidDataReceiven(const std::string &msg) : TableExceptions(msg) {}
    };
    class UndefinedException : public TableExceptions
    {
    public:
        explicit UndefinedException(const std::string &msg) : TableExceptions(msg) {}
    };
    
}
