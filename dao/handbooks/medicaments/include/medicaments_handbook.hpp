#pragma once

#include "handbook.hpp"
#include "medicament.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class MedicamentsHandbook final : public HandbookBase<objects::Medicament>
    {
    public:
        void force_insert(const objects::Medicament& record) override;
        void force_insert(const std::vector<objects::Medicament>& records) override;
        void remove_by_id(int32_t id) override;
        void remove_by_name(const std::string& name) override;
        std::vector<objects::Medicament> get_by_id(int32_t id) override;
        std::vector<objects::Medicament> get_by_name(const std::string& name) override;
        std::vector<objects::Medicament> search(const std::string& pattern) override;
        void tear_down() override;
        std::vector<objects::Medicament> select() override;

    private:
        void setup(std::shared_ptr<common::database::interfaces::DbInterface> client) & override;
    };
}
