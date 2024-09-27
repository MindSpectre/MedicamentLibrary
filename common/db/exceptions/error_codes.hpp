#pragma once

namespace drug_lib::common::database::errors
{
    enum class DbErrorCode
    {
        // Success Codes (2xx)
        SUCCESS = 200, // No error, operation succeeded

        // Client-Side Errors (4xx)
        INVALID_QUERY = 400, // Query syntax is invalid
        RECORD_NOT_FOUND = 404, // No records found for the given query
        DUPLICATE_RECORD = 409, // Duplicate record exists
        CONSTRAINT_VIOLATION = 422, // Foreign key, unique or other constraint violation
        PERMISSION_DENIED = 403, // Insufficient permissions for the operation
        DATA_CONVERSION_ERROR = 412, // Error while converting data types
        INVALID_DATA = 413,
        // Server-Side Errors (5xx)
        CONNECTION_FAILED = 500, // Failed to connect to the database
        DISCONNECTION_FAILED = 501, // Failed to disconnect from the database
        QUERY_EXECUTION_FAILED = 502, // Failed to execute a query
        PREPARED_STATEMENT_FAILED = 503, // Failed to prepare a statement
        TRANSACTION_START_FAILED = 504, // Failed to start a transaction
        TRANSACTION_COMMIT_FAILED = 505, // Failed to commit a transaction
        TRANSACTION_ROLLBACK_FAILED = 506, // Failed to rollback a transaction
        CONNECTION_TIMEOUT = 507, // Connection to the database timed out
        CONNECTION_POOL_EXHAUSTED = 508, // No available connections in the pool
        DEADLOCK_DETECTED = 509, // Deadlock detected during transaction
        SYSTEM_ROLLBACK = 510,
        // Miscellaneous Errors (6xx)
        NULL_POINTER_EXCEPTION = 600, // Attempted to dereference a null pointer
        UNKNOWN_ERROR = 601 // An unknown or unspecified error occurred
    };

    inline std::string decode_error(const DbErrorCode code)
    {
        switch (code)
        {
        case DbErrorCode::SUCCESS:
            return "200: Operation succeeded.";
        case DbErrorCode::INVALID_QUERY:
            return "400: Invalid query syntax.";
        case DbErrorCode::RECORD_NOT_FOUND:
            return "404: No records found.";
        case DbErrorCode::DUPLICATE_RECORD:
            return "409: Duplicate record exists.";
        case DbErrorCode::CONSTRAINT_VIOLATION:
            return "422: Constraint violation occurred.";
        case DbErrorCode::PERMISSION_DENIED:
            return "403: Permission denied.";
        case DbErrorCode::DATA_CONVERSION_ERROR:
            return "412: Data conversion error.";
        case DbErrorCode::CONNECTION_FAILED:
            return "500: Failed to connect to the database.";
        case DbErrorCode::DISCONNECTION_FAILED:
            return "501: Failed to disconnect from the database.";
        case DbErrorCode::QUERY_EXECUTION_FAILED:
            return "502: Failed to execute the query.";
        case DbErrorCode::PREPARED_STATEMENT_FAILED:
            return "503: Failed to prepare the statement.";
        case DbErrorCode::TRANSACTION_START_FAILED:
            return "504: Failed to start the transaction.";
        case DbErrorCode::TRANSACTION_COMMIT_FAILED:
            return "505: Failed to commit the transaction.";
        case DbErrorCode::TRANSACTION_ROLLBACK_FAILED:
            return "506: Failed to rollback the transaction.";
        case DbErrorCode::CONNECTION_TIMEOUT:
            return "507: Connection to the database timed out.";
        case DbErrorCode::CONNECTION_POOL_EXHAUSTED:
            return "508: Connection pool exhausted.";
        case DbErrorCode::DEADLOCK_DETECTED:
            return "509: Deadlock detected.";
        case DbErrorCode::NULL_POINTER_EXCEPTION:
            return "600: Null pointer exception.";
        case DbErrorCode::UNKNOWN_ERROR:
            return "601: Unknown database error.";
        default:
            return "Unrecognized error code.";
        }
    }
}
