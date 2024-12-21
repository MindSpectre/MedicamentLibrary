#pragma once

#include <ostream>
#include "data_property.hpp"
#include "medicaments/source/properties_definition.hpp"

namespace drug_lib::data::objects::medicaments
{
    class ActiveIngredient
    {
    public:
        ~ActiveIngredient() = default;

        ActiveIngredient(std::string name, const int8_t risk_level) :
            name_(std::move(name)),
            risk_level_(risk_level)
        {
        }

        [[nodiscard]] const std::string& get_name() const
        {
            return name_;
        }

        void set_name(const std::string& name)
        {
            this->name_ = name;
        }

        [[nodiscard]] int8_t get_risk_level() const
        {
            return risk_level_;
        }

        void set_risk_level(const int8_t risk_level)
        {
            this->risk_level_ = risk_level;
        }

        struct names_of_json_fields
        {
            static constexpr auto name = "name";
            static constexpr auto risk_level = "risk_level";
        };

        friend std::ostream& operator<<(std::ostream& os, const ActiveIngredient& obj)
        {
            return os
                << "name_: " << obj.name_
                << " risk_level_: " << obj.risk_level_;
        }

    private:
        std::string name_;
        int8_t risk_level_{0};
    };

    class ActiveIngredients final : public ArrayProperty<ActiveIngredient>
    {
    public:
        ActiveIngredients() = default;

        explicit ActiveIngredients(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit ActiveIngredients(std::vector<ActiveIngredient> substances)
            : ArrayProperty(std::move(substances))
        {
        }

        ~ActiveIngredients() override = default;


        [[nodiscard]] std::string get_name() const override
        {
            return properties::active_ingredients;
        }

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value active_ingredients_array(Json::arrayValue);

            for (const auto& symptom : data_)
            {
                Json::Value symptom_json;
                symptom_json[ActiveIngredient::names_of_json_fields::name] = symptom.get_name();
                symptom_json[ActiveIngredient::names_of_json_fields::risk_level] = symptom.get_risk_level();
                active_ingredients_array.append(symptom_json);
            }

            return active_ingredients_array;
        }

        void set_info(const Json::Value& property) override
        {
            data_.clear(); // Clear any existing substance

            for (const auto& symptom_json : property)
            {
                if (symptom_json.isObject())
                {
                    ActiveIngredient symptom(
                        symptom_json[ActiveIngredient::names_of_json_fields::name].asString(),
                        static_cast<int8_t>(symptom_json[ActiveIngredient::names_of_json_fields::risk_level].
                            asUInt())
                    );
                    data_.push_back(std::move(symptom));
                }
            }
        }
    };
}
