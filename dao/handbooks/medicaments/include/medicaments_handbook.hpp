#pragma once

#include "handbook.hpp"
#include "medicaments.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class MedicamentsHandbook final : public HandbookBase<objects::MedicamentBase>
    {
    public:
        void insert(const objects::MedicamentBase& record) override;
        void insert(const std::vector<objects::MedicamentBase>& record) override;
        void update(const objects::MedicamentBase& record) override;
        void update(const std::vector<objects::MedicamentBase>& record) override;
        void remove(const common::database::FieldConditions& conditions) override;
        std::vector<objects::MedicamentBase>&
        get(const common::database::FieldConditions& conditions) override;
        void set_connection(std::shared_ptr<common::database::interfaces::DbInterface> connect) override;
        void drop_connection() override;
        std::shared_ptr<common::database::interfaces::DbInterface> get_connection() override;
        ~MedicamentsHandbook() override;

    private:
        std::string_view table_name_ = handbook_tables_name::Medicaments;
        std::shared_ptr<common::database::interfaces::DbInterface> connect_;
    };
}
