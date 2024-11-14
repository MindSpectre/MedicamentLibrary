#pragma once

#include <ostream>

#include "patients/source/properties_definition.hpp"

namespace drug_lib::data::objects::patients
{
    class CurrentDiseases final : public DataProperty
    {
    public:
        CurrentDiseases() = default;

        explicit CurrentDiseases(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit CurrentDiseases(std::vector<int32_t> current_diseases)
            : current_diseases_(std::move(current_diseases))
        {
        }

        ~CurrentDiseases() override = default;

        [[nodiscard]] const std::vector<int32_t>& get_current_diseases() const
        {
            return current_diseases_;
        }

        void set_current_diseases(std::vector<int32_t> symptoms)
        {
            current_diseases_ = std::move(symptoms);
        }

        template <typename... Args>
        CurrentDiseases& emplace_back(Args&&... args)
        {
            current_diseases_.emplace_back(std::forward<Args>(args)...);
            return *this;
        }

        void push_back(const int32_t disease_id)
        {
            current_diseases_.push_back(disease_id);
        }

        void pop_back()
        {
            current_diseases_.pop_back();
        }

        int32_t operator[](const std::size_t index) const
        {
            if (index >= current_diseases_.size())
            {
                throw std::out_of_range("index out of range");
            }
            return current_diseases_[index];
        }

        int32_t& operator[](const std::size_t index)
        {
            return current_diseases_[index];
        }

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value result(Json::arrayValue);
            for (const auto& ids : current_diseases_)
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
            current_diseases_.reserve(property.size());
            for (const auto& it : property)
            {
                current_diseases_.push_back(it.asInt());
            }
        }

        [[nodiscard]] std::string get_name() const override
        {
            return properties::current_diseases;
        }

        friend std::ostream& operator<<(std::ostream& os, const CurrentDiseases& obj)
        {
            os << "[";
            for (std::size_t idx = 0; idx < obj.current_diseases_.size(); ++idx)
            {
                os << obj.current_diseases_[idx];
                if (idx < obj.current_diseases_.size() - 1)
                {
                    os << ", ";
                }
            }
            os << "]";
            return os;
        }

    private:
        std::vector<int32_t> current_diseases_;
    };
}
