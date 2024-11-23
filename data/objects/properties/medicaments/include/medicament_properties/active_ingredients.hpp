#pragma once

#include <ostream>

#include "medicaments/source/properties_definition.hpp"

namespace drug_lib::data::objects::medicaments
{
    class ActiveIngredients final : public DataProperty
    {
    public:
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

        ActiveIngredients() = default;

        explicit ActiveIngredients(const Json::Value& properties)
        {
            this->set_info(properties);
        }


        ~ActiveIngredients() override = default;

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value active_ingredients_array(Json::arrayValue);

            for (const auto& symptom : active_ingredients_)
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
            active_ingredients_.clear(); // Clear any existing substance

            for (const auto& symptom_json : property)
            {
                if (symptom_json.isObject())
                {
                    ActiveIngredient symptom(
                        symptom_json[ActiveIngredient::names_of_json_fields::name].asString(),
                        static_cast<int8_t>(symptom_json[ActiveIngredient::names_of_json_fields::risk_level].
                            asUInt())
                    );
                    active_ingredients_.push_back(std::move(symptom));
                }
            }
        }


        explicit ActiveIngredients(std::vector<ActiveIngredient> substances)
            : active_ingredients_(std::move(substances))
        {
        }


        [[nodiscard]] const std::vector<ActiveIngredient>& get_active_ingredients() const
        {
            return active_ingredients_;
        }

        void set_active_ingredients(std::vector<ActiveIngredient> substances)
        {
            active_ingredients_ = std::move(substances);
        }

        template <typename... Args>
        ActiveIngredients& emplace_back(Args&&... args)
        {
            active_ingredients_.emplace_back(std::forward<Args>(args)...);
            return *this;
        }

        template <typename ActiveIngredientDataC>
            requires std::is_same_v<std::remove_cvref_t<ActiveIngredientDataC>, ActiveIngredient>
        void push_back(ActiveIngredientDataC&& sym)
        {
            active_ingredients_.push_back(std::forward<ActiveIngredientDataC>(sym));
        }

        void pop_back()
        {
            active_ingredients_.pop_back();
        }

        ActiveIngredient operator[](const std::size_t index) const
        {
            if (index >= active_ingredients_.size())
            {
                throw std::out_of_range("index out of range");
            }
            return active_ingredients_[index];
        }

        ActiveIngredient& operator[](const std::size_t index)
        {
            if (index >= active_ingredients_.size())
            {
                throw std::out_of_range("index out of range");
            }
            return active_ingredients_[index];
        }

        [[nodiscard]] std::string get_name() const override
        {
            return properties::active_ingredients;
        }

    private:
        std::vector<ActiveIngredient> active_ingredients_;
    };
}
