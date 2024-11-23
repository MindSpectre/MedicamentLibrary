#pragma once

#include <ostream>

#include "medicaments/source/properties_definition.hpp"

namespace drug_lib::data::objects::medicaments
{
    class InactiveIngredients final : public DataProperty
    {
    public:
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

        InactiveIngredients() = default;

        explicit InactiveIngredients(const Json::Value& properties)
        {
            this->set_info(properties);
        }


        ~InactiveIngredients() override = default;

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value inactive_ingredients_array(Json::arrayValue);

            for (const auto& symptom : inactive_ingredients_)
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
            inactive_ingredients_.clear(); // Clear any existing substance

            for (const auto& symptom_json : property)
            {
                if (symptom_json.isObject())
                {
                    InactiveIngredient symptom(
                        symptom_json[InactiveIngredient::names_of_json_fields::name].asString(),
                        static_cast<int8_t>(symptom_json[InactiveIngredient::names_of_json_fields::risk_level].
                            asUInt())
                    );
                    inactive_ingredients_.push_back(std::move(symptom));
                }
            }
        }


        explicit InactiveIngredients(std::vector<InactiveIngredient> substances)
            : inactive_ingredients_(std::move(substances))
        {
        }


        [[nodiscard]] const std::vector<InactiveIngredient>& get_inactive_ingredients() const
        {
            return inactive_ingredients_;
        }

        void set_inactive_ingredients(std::vector<InactiveIngredient> substances)
        {
            inactive_ingredients_ = std::move(substances);
        }

        template <typename... Args>
        InactiveIngredients& emplace_back(Args&&... args)
        {
            inactive_ingredients_.emplace_back(std::forward<Args>(args)...);
            return *this;
        }

        template <typename InactiveIngredientDataC>
            requires std::is_same_v<std::remove_cvref_t<InactiveIngredientDataC>, InactiveIngredient>
        void push_back(InactiveIngredientDataC&& sym)
        {
            inactive_ingredients_.push_back(std::forward<InactiveIngredientDataC>(sym));
        }

        void pop_back()
        {
            inactive_ingredients_.pop_back();
        }

        InactiveIngredient operator[](const std::size_t index) const
        {
            if (index >= inactive_ingredients_.size())
            {
                throw std::out_of_range("index out of range");
            }
            return inactive_ingredients_[index];
        }

        InactiveIngredient& operator[](const std::size_t index)
        {
            if (index >= inactive_ingredients_.size())
            {
                throw std::out_of_range("index out of range");
            }
            return inactive_ingredients_[index];
        }

        [[nodiscard]] std::string get_name() const override
        {
            return properties::inactive_ingredients;
        }

    private:
        std::vector<InactiveIngredient> inactive_ingredients_;
    };
}
