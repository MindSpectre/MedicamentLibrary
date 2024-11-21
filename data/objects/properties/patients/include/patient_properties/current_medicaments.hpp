#pragma once

#include <ostream>

#include "patients/source/properties_definition.hpp"

namespace drug_lib::data::objects::patients
{
    class CurrentMedicaments final : public DataProperty
    {
    public:
        CurrentMedicaments() = default;

        explicit CurrentMedicaments(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit CurrentMedicaments(std::vector<int32_t> current_medicaments)
            : current_medicaments_(std::move(current_medicaments))
        {
        }

        ~CurrentMedicaments() override = default;

        [[nodiscard]] const std::vector<int32_t>& get_current_medicaments() const
        {
            return current_medicaments_;
        }

        void set_current_medicaments(std::vector<int32_t> symptoms)
        {
            current_medicaments_ = std::move(symptoms);
        }

        template <typename... Args>
        CurrentMedicaments& emplace_back(Args&&... args)
        {
            current_medicaments_.emplace_back(std::forward<Args>(args)...);
            return *this;
        }

        void push_back(const int32_t disease_id)
        {
            current_medicaments_.push_back(disease_id);
        }

        void pop_back()
        {
            current_medicaments_.pop_back();
        }

        int32_t operator[](const std::size_t index) const
        {
            if (index >= current_medicaments_.size())
            {
                throw std::out_of_range("index out of range");
            }
            return current_medicaments_[index];
        }

        int32_t& operator[](const std::size_t index)
        {
            return current_medicaments_[index];
        }

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value result(Json::arrayValue);
            for (const auto& ids : current_medicaments_)
            {
                result.append(ids);
            }
            return result;
        }

        void set_info(const Json::Value& property) override
        {
            if (!property.isArray())
            {
                throw std::invalid_argument("Property is not an array");
            }
            current_medicaments_.reserve(property.size());
            for (const auto& it : property)
            {
                current_medicaments_.push_back(it.asInt());
            }
        }

        [[nodiscard]] std::string get_name() const override
        {
            return properties::current_medicaments;
        }

        friend std::ostream& operator<<(std::ostream& os, const CurrentMedicaments& obj)
        {
            os << "[";
            for (std::size_t idx = 0; idx < obj.current_medicaments_.size(); ++idx)
            {
                os << obj.current_medicaments_[idx];
                if (idx < obj.current_medicaments_.size() - 1)
                {
                    os << ", ";
                }
            }
            os << "]";
            return os;
        }

    private:
        std::vector<int32_t> current_medicaments_;
    };
}
