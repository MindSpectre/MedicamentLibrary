#include "organizations_handbook.hpp"

#include <iostream>
#include <pqxx/except>

namespace drug_lib::dao
{
    OrganizationsHandbook::~OrganizationsHandbook()
    {
        try
        {
            connect_->drop_connect();
        }
        catch (const pqxx::broken_connection& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    void OrganizationsHandbook::remove_all()
    {
        remove({});
    }

    std::vector<objects::Organization> OrganizationsHandbook::get_all()
    {
        return get({});
    }

    void OrganizationsHandbook::update_all_fields(const objects::Organization& record)
    {
    }

    void OrganizationsHandbook::update_all_fields(const std::vector<objects::Organization>& records)
    {
    }
}
