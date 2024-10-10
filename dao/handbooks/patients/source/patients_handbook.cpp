#include "patients_handbook.hpp"

#include <iostream>
#include <pqxx/except>

namespace drug_lib::dao
{
    PatientsHandbook::~PatientsHandbook()
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

    void PatientsHandbook::remove_all()
    {
        remove({});
    }

    std::vector<objects::Patient> PatientsHandbook::get_all()
    {
        return get({});
    }
}
