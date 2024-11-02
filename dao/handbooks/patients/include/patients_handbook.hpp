#pragma once

#include "handbook.hpp"
#include "patient.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class PatientsHandbook final : public HandbookBase<objects::Patient>
    {
    public:
        void force_insert(const objects::Patient& record) override;
        void force_insert(const std::vector<objects::Patient>& records) override;
        void remove_by_id(int32_t id) override;
        void remove_by_name(const std::string& name) override;
        std::vector<objects::Patient> get_by_id(int32_t id) override;
        std::vector<objects::Patient> get_by_name(const std::string& name) override;
        std::vector<objects::Patient> search(const std::string& pattern) override;
        void tear_down() override;
        std::vector<objects::Patient> select() override;
    };
}
