#pragma once

#include <ostream>

#include "diseases/source/properties_definition.hpp"

namespace drug_lib::data::objects::medicaments
{
    class SideEffects final : public DataProperty
    {
    public:
        SideEffects() = default;

        explicit SideEffects(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit SideEffects(std::vector<std::string> side_effects)
            : side_effects_(std::move(side_effects))
        {
        }

        ~SideEffects() override = default;

        [[nodiscard]] const std::vector<std::string>& get_side_effects() const
        {
            return side_effects_;
        }

        void set_side_effects(std::vector<std::string> symptoms)
        {
            side_effects_ = std::move(symptoms);
        }

        template <typename... Args>
        SideEffects& emplace_back(Args&&... args)
        {
            side_effects_.emplace_back(std::forward<Args>(args)...);
            return *this;
        }

        void push_back(std::string disease_id)
        {
            side_effects_.push_back(std::move(disease_id));
        }

        void pop_back()
        {
            side_effects_.pop_back();
        }

        std::string operator[](const std::size_t index) const
        {
            if (index >= side_effects_.size())
            {
                throw std::out_of_range("index out of range");
            }
            return side_effects_[index];
        }

        std::string& operator[](const std::size_t index)
        {
            return side_effects_[index];
        }

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value result(Json::arrayValue);
            for (const auto& ids : side_effects_)
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
            side_effects_.reserve(property.size());
            for (const auto& it : property)
            {
                side_effects_.push_back(it.asString());
            }
        }

        [[nodiscard]] std::string get_name() const override
        {
            return properties::side_effects;
        }

        friend std::ostream& operator<<(std::ostream& os, const SideEffects& obj)
        {
            os << "[";
            for (std::size_t idx = 0; idx < obj.side_effects_.size(); ++idx)
            {
                os << obj.side_effects_[idx];
                if (idx < obj.side_effects_.size() - 1)
                {
                    os << ", ";
                }
            }
            os << "]";
            return os;
        }

    private:
        std::vector<std::string> side_effects_;
    };
}
