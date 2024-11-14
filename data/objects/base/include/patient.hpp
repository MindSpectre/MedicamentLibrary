#pragma once

#include "common_object.hpp"
#include "properties_controller.hpp"

namespace drug_lib::data::objects
{
    class Patient final : public ObjectBase, public PropertiesHolder
    {
    public:
        struct field_name : common_fields_names
        {
            static constexpr auto gender = "gender";
            static constexpr auto birth_date = "birth_date";
            static constexpr auto contact_information = "contact_information";
        };

        Patient() = default;


        Patient(const int32_t id, std::string name, std::string gender, std::chrono::year_month_day const birth_date,
                std::string contact_information)
            : id_(id),
              name_(std::move(name)),
              gender_(std::move(gender)),
              birth_date_(birth_date),
              contact_information_(std::move(contact_information))
        {
        }

        ~Patient() override = default;

        [[nodiscard]] common::database::Record to_record() const override
        {
            common::database::Record record;
            record.push_back(std::make_unique<common::database::Field<int32_t>>(field_name::id, id_));
            record.push_back(std::make_unique<common::database::Field<std::string>>(field_name::name, name_));
            record.push_back(std::make_unique<common::database::Field<std::string>>(field_name::gender, gender_));
            record.push_back(
                std::make_unique<common::database::Field<std::chrono::system_clock::time_point>>(
                    field_name::birth_date, std::chrono::sys_days{birth_date_}));
            record.push_back(
                std::make_unique<common::database::Field<std::string>>(field_name::contact_information,
                                                                       contact_information_));
            record.push_back(collection_.make_properties_field());
            return record;
        }

        void from_record(const common::database::Record& record) override
        {
            for (const auto& field : record.fields())
            {
                if (const auto& field_name = field->get_name();
                    field_name == field_name::id)
                {
                    id_ = field->as<int32_t>();
                }
                else if (field_name == field_name::name)
                {
                    name_ = field->as<std::string>();
                }

                else if (field_name == field_name::gender)
                {
                    gender_ = field->as<std::string>();
                }
                else if (field_name == field_name::birth_date)
                {
                    auto time_point = field->as<std::chrono::system_clock::time_point>();
                    birth_date_ = std::chrono::floor<std::chrono::days>(time_point);
                }
                else if (field_name == field_name::contact_information)
                {
                    contact_information_ = field->as<std::string>();
                }
                else if (field_name == field_name::properties)
                {
                    create_collection(field);
                }
                else
                {
                    throw std::invalid_argument("Unknown field name: " + field_name);
                }
            }
        }

        void from_record(const std::unique_ptr<common::database::ViewRecord>& viewed) override
        {
            for (std::size_t i = 0; i < viewed->size(); i++)
            {
                if (const auto& field_name = viewed->name(i); field_name == field_name::id)
                {
                    id_ = std::stoi(viewed->extract(i));
                }
                else if (field_name == field_name::name)
                {
                    name_ = viewed->extract(i);
                }
                else if (field_name == field_name::gender)
                {
                    gender_ = viewed->extract(i);
                }
                else if (field_name == field_name::birth_date)
                {
                    std::istringstream iss(viewed->extract(i));
                    std::tm tm = {};

                    if (iss.fail())
                    {
                        throw std::runtime_error("Failed to parse date: " + viewed->extract(i));
                    }
                    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                    birth_date_ = std::chrono::floor<std::chrono::days>(tp);
                }
                else if (field_name == field_name::contact_information)
                {
                    contact_information_ = viewed->extract(i);
                }
                else if (field_name == field_name::properties)
                {
                    create_collection(viewed->extract(i));
                }
                else
                {
                    throw std::invalid_argument("Unknown field name: " + field_name);
                }
            }
        }

        [[nodiscard]] int32_t get_id() const
        {
            return id_;
        }

        void set_id(const int32_t id)
        {
            id_ = id;
        }

        [[nodiscard]] const std::string& get_name() const
        {
            return name_;
        }

        void set_name(const std::string& name)
        {
            name_ = name;
        }

        [[nodiscard]] const std::string& get_gender() const
        {
            return gender_;
        }

        void set_gender(const std::string& gender)
        {
            gender_ = gender;
        }

        [[nodiscard]] const std::chrono::year_month_day& get_birth_date() const
        {
            return birth_date_;
        }

        void set_birth_date(const std::chrono::year_month_day& birth_date)
        {
            birth_date_ = birth_date;
        }

        [[nodiscard]] const std::string& get_contact_information() const
        {
            return contact_information_;
        }

        void set_contact_information(const std::string& contact_information)
        {
            contact_information_ = contact_information;
        }

    private:
        int32_t id_ = -1;
        std::string name_;
        std::string gender_;
        std::chrono::year_month_day birth_date_{};
        std::string contact_information_;
    };
}
