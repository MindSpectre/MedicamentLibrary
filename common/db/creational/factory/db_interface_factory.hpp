#pragma once

#include <memory>
#include "db_interface.hpp"
#include "pqxx_client.hpp"

namespace drug_lib::common::database::creational
{
    class DbInterfaceFactory
    {
    public:
        static std::shared_ptr<interfaces::DbInterface> create_pqxx_client(const PqxxConnectParams& _params)
        {
            return std::make_shared<PqxxClient>(_params);
        }

        // static std::shared_ptr<interfaces::DbInterface> create_mock_database()
        // {
        //     return std::make_shared<interfaces::DbInterface>();
        // }
    };
}
