#pragma once

namespace drug_lib::common::database
{
    class PqxxViewRecord final : public ViewRecord
    {
    public:
        [[nodiscard]] std::string extract(const int32_t idx) const & override
        {
            return std::string{row_[idx].view()};
        }

        [[nodiscard]] std::string_view view(const int idx) const & override
        {
            return row_[idx].view();
        }

        void set_row(pqxx::row&& row)
        {
            row_ = std::move(row);
        }

        PqxxViewRecord() = default;

    private:
        pqxx::row row_;
    };
}
