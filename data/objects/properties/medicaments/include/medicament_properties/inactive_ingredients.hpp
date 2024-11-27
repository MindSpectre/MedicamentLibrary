#pragma once

#include <ostream>

#include "medicaments/source/properties_definition.hpp"

namespace drug_lib::data::objects::medicaments
{
    class InactiveIngredient
    {
    public:
        ~InactiveIngredient() = default;

        InactiveIngredient(std::string name, const int8_t risk_level) :
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

        friend std::ostream& operator<<(std::ostream& os, const InactiveIngredient& obj)
        {
            return os
                << "name_: " << obj.name_
                << " risk_level_: " << obj.risk_level_;
        }

    private:
        std::string name_;
        int8_t risk_level_{0};
    };

    class InactiveIngredients final : public ArrayProperty<InactiveIngredient>
    {
    public:
        InactiveIngredients() = default;

        explicit InactiveIngredients(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit InactiveIngredients(std::vector<InactiveIngredient> substances)
            : ArrayProperty(std::move(substances))
        {
        }

        ~InactiveIngredients() override = default;

        [[nodiscard]] std::string get_name() const override
        {
            return properties::inactive_ingredients;
        }


        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value inactive_ingredients_array(Json::arrayValue);

            for (const auto& symptom : data_)
            {
                Json::Value symptom_json;
                symptom_json[InactiveIngredient::names_of_json_fields::name] = symptom.get_name();
                symptom_json[InactiveIngredient::names_of_json_fields::risk_level] = symptom.get_risk_level();
                inactive_ingredients_array.append(symptom_json);
            }

            return inactive_ingredients_array;
        }

        void set_info(const Json::Value& property) override
        {
            data_.clear(); // Clear any existing substance

            for (const auto& symptom_json : property)
            {
                if (symptom_json.isObject())
                {
                    InactiveIngredient symptom(
                        symptom_json[InactiveIngredient::names_of_json_fields::name].asString(),
                        static_cast<int8_t>(symptom_json[InactiveIngredient::names_of_json_fields::risk_level].
                            asUInt())
                    );
                    data_.push_back(std::move(symptom));
                }
            }
        }
    };
}
