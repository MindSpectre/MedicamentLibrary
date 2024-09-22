//#include <boost/algorithm/string.hpp>
//#include "StationDatabasePostgres.h"
//#include <pqxx/pqxx>
//
//namespace ControlSystemV7::Common::Database
//{
//    void EscapeColumnValue(std::string &columnValue)
//    {
//        boost::algorithm::replace_all(columnValue, "\\", "\\\\"); // Escape backslashes
//        boost::algorithm::replace_all(columnValue, "\"", "\\\""); // Escape quotes
//        boost::algorithm::replace_all(columnValue, "\t", " ");
//        boost::algorithm::replace_all(columnValue, "\r", " ");
//        boost::algorithm::replace_all(columnValue, "\n", " ");
//        boost::algorithm::replace_all(columnValue, "'", " ");
//    }
//
//    std::string FieldTypeToString(FieldType type)
//    {
//        std::string fieldType;
//
//        switch (type)
//        {
//        case FieldType::INT8:
//        case FieldType::INT16:
//        case FieldType::INT32:
//        case FieldType::INT64:
//            fieldType = "INT";
//            break;
//        case FieldType::GUID:
//            fieldType = "uuid DEFAULT gen_random_uuid()";
//            break;
//        case FieldType::TIMESTAMP:
//            fieldType = "timestamp with time zone NOT NULL";
//            break;
//        case FieldType::TIMESTAMP_AUTOGEN:
//            fieldType = "timestamp with time zone NOT NULL DEFAULT now()";
//            break;
//        default:
//            fieldType = "TEXT";
//            break;
//        }
//
//        return fieldType;
//    }
//
//
//    bool StationDatabasePostgres::IsValidIdentifier(const std::string &identifier) const
//    {
//        static const std::regex validIdentifierRegex("^[a-zA-Z_][a-zA-Z0-9_]*$");
//        return std::regex_match(identifier, validIdentifierRegex);
//    }
//
//    void StationDatabasePostgres::CreateTable(const std::string &tableName, const std::vector<FieldDescription> &fieldList)
//    {
//        if (tableName.empty() || !IsValidIdentifier(tableName))
//        {
//            throw Exceptions::CreateTableException("Invalid table name: " + tableName);
//        }
//
//        std::string createTableQuery = "CREATE TABLE " + tableName + " (";
//        std::vector<std::string> ftsFields;
//        bool isFirstKey = true;
//
//        for (const auto &field : fieldList)
//        {
//            if (field.indexType == FieldIndexType::FTS)
//            {
//                ftsFields.push_back(field.name);
//                continue;
//            }
//
//            if (field.indexType != FieldIndexType::KEY)
//                continue;
//
//            if (!isFirstKey)
//                createTableQuery.append(", ");
//
//            createTableQuery.append(field.name + " " + FieldTypeToString(field.type));
//
//            isFirstKey = false;
//        }
//
//        createTableQuery.append(", database_version INT, json_data JSONB");
//        createTableQuery.append(")");
//        DBExecute(std::move(createTableQuery), {});
//
//        std::string createIndexQuery = "CREATE INDEX IF NOT EXISTS " + tableName + "_key_field_idx" +
//                                       " ON public." + tableName +
//                                       " USING btree (" + fieldList[0].name + " ASC NULLS LAST);";
//
//        std::string clusterQuery = "CLUSTER " + tableName + " USING " + tableName + "_key_field_idx";
//
//        pqxx::work txn2(*conn_);
//        txn2.exec(createIndexQuery);
//        txn2.exec(clusterQuery);
//        txn2.commit();
//
//        std::string createFtsIndexQuery = "CREATE INDEX " + tableName + "_fts" +
//                                          " ON " + tableName +
//                                          " USING gin(to_tsvector('simple', json_data::text));";
//        pqxx::work txn3(*conn_);
//        txn3.exec(createFtsIndexQuery);
//        txn3.commit();
//    }
//
//    void StationDatabasePostgres::RemoveTable(const std::string &tableName)
//    {
//        if (tableName.empty() || !IsValidIdentifier(tableName) || this->HasTable(tableName))
//        {
//            throw Exceptions::RemoveTableException("Invalid table name: " + tableName);
//        }
//
//        std::string removeTableQuery = "DROP TABLE IF EXISTS " + tableName;
//        if(strategy.Batcher.IsEnabled()){
//            strategy.Batcher.execute(removeTableQuery, {});
//        } else {
//            DBExecute(removeTableQuery, {});
//        }
//    }
//
//    bool StationDatabasePostgres::HasTable(const std::string &tableName) const
//    {
//        if (tableName.empty() || !IsValidIdentifier(tableName))
//        {
//            throw Exceptions::HasTableException("Invalid table name: " + tableName);
//        }
//
//        pqxx::nontransaction ntx(*conn_);
//        pqxx::result res = ntx.exec("SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name = '" + ntx.esc(tableName) + "')");
//
//        return res[0][0].as<bool>();
//    }
//
//    void StationDatabasePostgres::MakeUniqueConstraint(const std::string &tableName, std::vector<std::string> &&columnNames)
//    {
//        if (tableName.empty() || !IsValidIdentifier(tableName))
//        {
//            throw Exceptions::QueryException("Invalid table name: " + tableName);
//        }
//
//        std::string query = "ALTER TABLE " + tableName + " ADD CONSTRAINT ";
//        std::string sub_query = " UNIQUE (";
//        for (auto &&column : columnNames)
//        {
//            if (!IsValidIdentifier(column))
//            {
//                throw Exceptions::QueryException("Invalid column name: " + column);
//            }
//            sub_query.append(column).append(",");
//            query.append(column).append("_");
//        }
//        query.append(tableName).append(sub_query);
//        query.pop_back();
//        query.append(");");
//        if(strategy.Batcher.IsEnabled()){
//            strategy.Batcher.execute(query, {});
//        } else {
//            DBExecute(query, {});
//        }
//    }
//
//    std::vector<std::vector<FieldDescription>> StationDatabasePostgres::GetDataFTS(const std::string &tableName, const std::string &ftsQueryParams, double &queryExecTime)
//    {
//        queryExecTime = 0.0;
//        std::vector<std::vector<FieldDescription>> result;
//
//        auto start = std::chrono::high_resolution_clock::now();
//
//        std::string selectQuery = "SELECT * FROM " + tableName + " WHERE to_tsvector('simple', json_data::text) @@ to_tsquery('simple', '" + ftsQueryParams + "');";
//
//        pqxx::nontransaction ntx(*conn_);
//        pqxx::result res = ntx.exec(selectQuery);
//
//        auto end = std::chrono::high_resolution_clock::now();
//        queryExecTime = std::chrono::duration<double>(end - start).count();
//
//        for (const auto &row : res)
//        {
//            std::vector<FieldDescription> rowFields;
//            for (const auto &field : row)
//            {
//                rowFields.push_back(FieldDescription{FieldType::STRING, field.name(), field.c_str(), FieldIndexType::NONE});
//            }
//            result.push_back(std::move(rowFields));
//        }
//
//        return result;
//    }
//
//    bool StationDatabasePostgres::GetDataFTSBatched(const std::string &tableName,
//                                                    const std::string &ftsQueryParams,
//                                                    double &queryExecTime,
//                                                    std::function<void(const std::vector<std::vector<FieldDescription>> &)> onResult)
//    {
//        queryExecTime = 0.0;
//        std::vector<std::vector<FieldDescription>> result;
//
//        auto start = std::chrono::high_resolution_clock::now();
//
//        std::string selectQuery = "SELECT * FROM " + tableName + " WHERE to_tsvector('simple', json_data::text) @@ to_tsquery('simple', '" + ftsQueryParams + "');";
//
//        pqxx::nontransaction ntx(*conn_);
//        pqxx::result res = ntx.exec(selectQuery);
//
//        auto end = std::chrono::high_resolution_clock::now();
//        queryExecTime = std::chrono::duration<double>(end - start).count();
//
//        for (const auto &row : res)
//        {
//            std::vector<FieldDescription> rowFields;
//            for (const auto &field : row)
//            {
//                rowFields.push_back(FieldDescription{FieldType::STRING, field.name(), field.c_str(), FieldIndexType::NONE});
//            }
//            result.push_back(std::move(rowFields));
//
//            if (result.size() >= 8)
//            {
//                onResult(result);
//                result.clear();
//            }
//        }
//
//        if (!result.empty())
//        {
//            onResult(result);
//        }
//
//        return true;
//    }
//
//    std::vector<std::vector<FieldDescription>> StationDatabasePostgres::GetData(const std::string &tableName, std::vector<std::pair<std::string, std::string>> &&keyValues)
//    {
//        std::vector<std::vector<FieldDescription>> result;
//
//        std::string selectQuery = "SELECT * FROM " + tableName + " WHERE ";
//
//        int paramIndex = 1;
//        pqxx::params values;
//        for (const auto &kv : keyValues)
//        {
//            selectQuery.append(kv.first + " = $" + std::to_string(paramIndex++) + " AND ");
//            values.append(kv.second);
//        }
//        selectQuery.erase(selectQuery.size() - 5);
//
//        pqxx::work txn(*conn_);
//        pqxx::result res = txn.exec_params(selectQuery,  (values));
//
//        for (const auto &row : res)
//        {
//            std::vector<FieldDescription> rowFields;
//            for (const auto &field : row)
//            {
//                rowFields.push_back(FieldDescription{FieldType::STRING, field.name(), field.c_str(), FieldIndexType::NONE});
//            }
//            result.push_back(std::move(rowFields));
//        }
//
//        return result;
//    }
//
//    void StationDatabasePostgres::RemoveData(const std::string &tableName, std::vector<std::pair<std::string, std::string>> &&keyValues)
//    {
//        std::string deleteQuery = "DELETE FROM " + tableName + " WHERE ";
//
//        int paramIndex = 1;
//        pqxx::params values;
//        for (const auto &kv : keyValues)
//        {
//            deleteQuery.append(kv.first + " = $" + std::to_string(paramIndex++) + " AND ");
//            values.append(kv.second);
//        }
//        deleteQuery.erase(deleteQuery.size() - 5);
//        if(strategy.Batcher.IsEnabled()){
//            strategy.Batcher.execute(deleteQuery, values);
//        } else {
//            DBExecute(deleteQuery, values);
//        }
//    }
//    int StationDatabasePostgres::GetCount(const std::string &tableName, const std::string &fieldName, double &queryExecTime)
//    {
//        queryExecTime = 0.0;
//
//        auto start = std::chrono::high_resolution_clock::now();
//
//        std::string selectQuery = "SELECT count(" + fieldName + ") FROM " + tableName;
//
//        pqxx::nontransaction ntx(*conn_);
//        pqxx::result res = ntx.exec(selectQuery);
//
//        auto end = std::chrono::high_resolution_clock::now();
//        queryExecTime = std::chrono::duration<double>(end - start).count();
//
//        return res[0][0].as<int>();
//    }
//
//    void StationDatabasePostgres::StartTransaction()
//    {
//        std::string query = "BEGIN;";
//        if(strategy.Batcher.IsEnabled()){
//            strategy.Batcher.execute(query, {});
//        } else {
//            DBExecute(query, {});
//        }
//    }
//
//    void StationDatabasePostgres::CommitTransaction()
//    {
//       std::string query = "COMMIT;";
//        if(strategy.Batcher.IsEnabled()){
//            strategy.Batcher.execute(query, {});
//        } else {
//            DBExecute(query, {});
//        }
//    }
//
//    void StationDatabasePostgres::RollbackTransaction()
//    {
//        std::string query = "ROLLBACK;";
//        if(strategy.Batcher.IsEnabled()){
//            strategy.Batcher.execute(query, {});
//        } else {
//            DBExecute(query, {});
//        }
//    }
//}
