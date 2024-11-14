#pragma once

#include <ostream>

#include "diseases/source/properties_definition.hpp"

namespace drug_lib::data::objects::diseases
{
    class Symptoms final : public DataProperty
    {
    public:
        class Symptom final
        {
        public:
            Symptom() = default;

            Symptom(std::string name, const uint8_t severity, std::string duration,
                    std::string type, std::string description)
                : name_(std::move(name)),
                  severity_(severity),
                  duration_(std::move(duration)),
                  type_(std::move(type)),
                  description_(std::move(description))
            {
            }

            Symptom(const Symptom& other) = default;

            Symptom(Symptom&& other) noexcept
                : name_(std::move(other.name_)),
                  severity_(other.severity_),
                  duration_(std::move(other.duration_)),
                  type_(std::move(other.type_)),
                  description_(std::move(other.description_))
            {
            }

            Symptom& operator=(const Symptom& other)
            {
                if (this == &other)
                    return *this;
                name_ = other.name_;
                severity_ = other.severity_;
                duration_ = other.duration_;
                type_ = other.type_;
                description_ = other.description_;
                return *this;
            }

            Symptom& operator=(Symptom&& other) noexcept
            {
                if (this == &other)
                    return *this;
                name_ = std::move(other.name_);
                severity_ = other.severity_;
                duration_ = std::move(other.duration_);
                type_ = std::move(other.type_);
                description_ = std::move(other.description_);
                return *this;
            }

            ~Symptom() = default;

            [[nodiscard]] const std::string& get_name() const
            {
                return name_;
            }

            void set_name(const std::string& name)
            {
                name_ = name;
            }

            [[nodiscard]] uint8_t get_severity() const
            {
                return severity_;
            }

            void set_severity(const uint8_t severity)
            {
                severity_ = severity;
            }

            [[nodiscard]] const std::string& get_duration() const
            {
                return duration_;
            }

            void set_duration(const std::string& duration)
            {
                duration_ = duration;
            }

            [[nodiscard]] const std::string& get_type() const
            {
                return type_;
            }

            void set_type(const std::string& type)
            {
                type_ = type;
            }

            [[nodiscard]] const std::string& get_description() const
            {
                return description_;
            }

            void set_description(const std::string& description)
            {
                description_ = description;
            }

            struct names_of_json_fields
            {
                static constexpr auto name = "name";
                static constexpr auto severity = "severity";
                static constexpr auto duration = "duration";
                static constexpr auto type = "type";
                static constexpr auto description = "description";
            };

            friend std::ostream& operator<<(std::ostream& os, const Symptom& obj)
            {
                return os
                    << "name_: " << obj.name_
                    << " severity_: " << obj.severity_
                    << " duration_: " << obj.duration_
                    << " type_: " << obj.type_
                    << " description_: " << obj.description_;
            }

        private:
            std::string name_;
            uint8_t severity_ = 0;
            std::string duration_;
            std::string type_;
            std::string description_;
        };

        Symptoms() = default;

        explicit Symptoms(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit Symptoms(std::vector<Symptom> symptoms)
            : symptoms_(std::move(symptoms))
        {
        }

        ~Symptoms() override = default;


        [[nodiscard]] const std::vector<Symptom>& get_symptoms() const
        {
            return symptoms_;
        }

        void set_symptoms(std::vector<Symptom> symptoms)
        {
            symptoms_ = std::move(symptoms);
        }

        template <typename... Args>
        Symptoms& emplace_back(Args&&... args)
        {
            symptoms_.emplace_back(std::forward<Args>(args)...);
            return *this;
        }

        template <typename SymptomC>
            requires std::is_same_v<std::remove_cvref_t<SymptomC>, Symptom>
        void push_back(SymptomC&& sym)
        {
            symptoms_.push_back(std::forward<SymptomC>(sym));
        }

        void pop_back()
        {
            symptoms_.pop_back();
        }

        Symptom operator[](const std::size_t index) const
        {
            if (index >= symptoms_.size())
            {
                throw std::out_of_range("index out of range");
            }
            return symptoms_[index];
        }

        Symptom& operator[](const std::size_t index)
        {
            if (index >= symptoms_.size())
            {
                throw std::out_of_range("index out of range");
            }
            return symptoms_[index];
        }

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value symptoms_array(Json::arrayValue);

            for (const auto& symptom : symptoms_)
            {
                Json::Value symptom_json;
                symptom_json[Symptom::names_of_json_fields::name] = symptom.get_name();
                symptom_json[Symptom::names_of_json_fields::severity] = symptom.get_severity();
                symptom_json[Symptom::names_of_json_fields::duration] = symptom.get_duration();
                symptom_json[Symptom::names_of_json_fields::type] = symptom.get_type();
                symptom_json[Symptom::names_of_json_fields::description] = symptom.get_description();
                symptoms_array.append(symptom_json);
            }

            return symptoms_array;
        }

        void set_info(const Json::Value& property) override
        {
            symptoms_.clear(); // Clear any existing symptoms

            for (const auto& symptom_json : property)
            {
                if (symptom_json.isObject())
                {
                    Symptom symptom(
                        symptom_json[Symptom::names_of_json_fields::name].asString(),
                        static_cast<uint8_t>(symptom_json[Symptom::names_of_json_fields::severity].asUInt()),
                        symptom_json[Symptom::names_of_json_fields::duration].asString(),
                        symptom_json[Symptom::names_of_json_fields::type].asString(),
                        symptom_json[Symptom::names_of_json_fields::description].asString()
                    );
                    symptoms_.push_back(std::move(symptom));
                }
            }
        }

        [[nodiscard]] std::string get_name() const override
        {
            return properties::symptoms;
        }

    private:
        // PRESCRIPTION DETAILS
        std::vector<Symptom> symptoms_;
    };
}
