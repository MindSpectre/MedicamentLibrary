#pragma once

#include "handbook.hpp"
#include "patient.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class PatientsHandbook final : public HandbookBase<objects::Patient>
    {
    public:
        ~PatientsHandbook() override;
        void remove_all() override;
        std::vector<objects::Patient> get_all() override;
        void update_all_fields(const objects::Patient& record) override;
        void update_all_fields(const std::vector<objects::Patient>& records) override;

        PatientsHandbook()
        {
            table_name_ = handbook_tables_name::Patients;
            setup_constraints();
            fts_fields_ = {

            };
            connect_->setup_full_text_search(table_name_, fts_fields_);
        }
    };
}
