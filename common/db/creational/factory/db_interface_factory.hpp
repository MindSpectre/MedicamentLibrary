#pragma once

#include <iostream>
#include <memory>

#include "db_interface.hpp"
#include "mock_db_client.hpp"
#include "pqxx_client.hpp"

namespace drug_lib::common::database::creational
{
    class DbInterfaceFactory
    {
    public:
        static std::unique_ptr<interfaces::DbInterface> create_pqxx_client(const PqxxConnectParams& _params)
        {
            std::unique_ptr<interfaces::DbInterface> connect;
            try
            {
                connect = std::make_unique<PqxxClient>(_params);
            }
            catch (const exceptions::ConnectionException& e)
            {
                std::cerr << e.what() << std::endl;

                try
                {
                    PqxxClient::create_database(_params);
                    connect = std::make_unique<PqxxClient>(_params);
                }
                catch (const std::exception& inner_e)
                {
                    std::string msg = "Failed to open database connection. Cascade of fails.";
                    msg.append(inner_e.what());
                    throw exceptions::ConnectionException(msg,
                                                          errors::db_error_code::CONNECTION_FAILED);
                }
            }
            return connect;
        }

        template <typename... Args>
        static std::unique_ptr<interfaces::DbInterface> create_mock_database(Args... argv)
        {
            return std::make_unique<MockDbClient>(argv...);
        }
    };
}
