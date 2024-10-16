#include "medicaments_handbook.hpp"

#include <iostream>
#include <pqxx/except>

namespace drug_lib::dao
{
    MedicamentsHandbook::~MedicamentsHandbook()
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

    void MedicamentsHandbook::remove_all()
    {
        remove({});
    }

    std::vector<objects::Medicament> MedicamentsHandbook::get_all()
    {
        return get({});
    }

    void MedicamentsHandbook::update_all_fields(const objects::Medicament& record)
    {
        upsert({record}, {});
    }

    void MedicamentsHandbook::update_all_fields(const std::vector<objects::Medicament>& records)
    {
        upsert(records, {});
    }
}
