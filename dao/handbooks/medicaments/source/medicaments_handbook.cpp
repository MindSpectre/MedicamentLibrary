#include "medicaments_handbook.hpp"

namespace drug_lib::dao
{
    void MedicamentsHandbook::insert(const objects::MedicamentBase& record)
    {
        try
        {
            connect_->add_data(table_name_, record.to_record());
        }
        catch (...)
        {
        }

    }

    void MedicamentsHandbook::insert(const std::vector<objects::MedicamentBase>& record)
    {
    }

    void MedicamentsHandbook::update(const objects::MedicamentBase& record)
    {
    }

    void MedicamentsHandbook::update(const std::vector<objects::MedicamentBase>& record)
    {
    }

    void MedicamentsHandbook::remove(const common::database::FieldConditions& conditions)
    {
    }

    std::vector<objects::MedicamentBase>& MedicamentsHandbook::get(
        const common::database::FieldConditions& conditions)
    {
    }

    void MedicamentsHandbook::set_connection(
        const std::shared_ptr<common::database::interfaces::DbInterface> connect)
    {
        connect_ = connect;
    }

    void MedicamentsHandbook::drop_connection()
    {
        connect_.reset();
    }

    std::shared_ptr<drug_lib::common::database::interfaces::DbInterface> drug_lib::dao::MedicamentsHandbook::
    get_connection()
    {
        return connect_;
    }

    drug_lib::dao::MedicamentsHandbook::~MedicamentsHandbook()
    = default;
}

