#pragma once

#include "disease.hpp"
#include "handbook_base.hpp"

namespace drug_lib::dao
{
    using namespace drug_lib::data;

    class DiseaseHandbook final : public HandbookBase<objects::Disease>
    {
    public:
        DiseaseHandbook() = default;

        explicit DiseaseHandbook(std::shared_ptr<common::database::interfaces::DbInterface> connect)
        {
            connect_ = std::move(connect);
            this->setup();
        }

        void set_connection(std::shared_ptr<common::database::interfaces::DbInterface> connect) override
        {
            connect_ = std::move(connect);
            this->setup();
        }

    private:
        void tear_down() override;
        void setup() & override;
    };
}
