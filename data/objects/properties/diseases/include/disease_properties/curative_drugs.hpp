#pragma once

#include <ostream>

#include "diseases/source/properties_definition.hpp"

namespace drug_lib::data::objects::diseases
{
    class CurativeDrugs final : public DataProperty
    {
    public:
        CurativeDrugs() = default;

        explicit CurativeDrugs(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit CurativeDrugs(std::vector<int32_t> curative_drugs)
            : curative_drugs_(std::move(curative_drugs))
        {
        }

        ~CurativeDrugs() override = default;

        [[nodiscard]] const std::vector<int32_t>& get_curative_drugs() const
        {
            return curative_drugs_;
        }

        void set_curative_drugs(std::vector<int32_t> symptoms)
        {
            curative_drugs_ = std::move(symptoms);
        }

        template <typename... Args>
        CurativeDrugs& emplace_back(Args&&... args)
        {
            curative_drugs_.emplace_back(std::forward<Args>(args)...);
            return *this;
        }

        void push_back(const int32_t disease_id)
        {
            curative_drugs_.push_back(disease_id);
        }

        void pop_back()
        {
            curative_drugs_.pop_back();
        }

        int32_t operator[](const std::size_t index) const
        {
            if (index >= curative_drugs_.size())
            {
                throw std::out_of_range("index out of range");
            }
            return curative_drugs_[index];
        }

        int32_t& operator[](const std::size_t index)
        {
            return curative_drugs_[index];
        }

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value result(Json::arrayValue);
            for (const auto& ids : curative_drugs_)
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
            curative_drugs_.reserve(property.size());
            for (const auto& it : property)
            {
                curative_drugs_.push_back(it.asInt());
            }
        }

        [[nodiscard]] std::string get_name() const override
        {
            return properties::curative_drugs;
        }

        friend std::ostream& operator<<(std::ostream& os, const CurativeDrugs& obj)
        {
            os << "[";
            for (std::size_t idx = 0; idx < obj.curative_drugs_.size(); ++idx)
            {
                os << obj.curative_drugs_[idx];
                if (idx < obj.curative_drugs_.size() - 1)
                {
                    os << ", ";
                }
            }
            os << "]";
            return os;
        }

    private:
        std::vector<int32_t> curative_drugs_;
    };
}
