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
            static constexpr auto name = "name";
            static constexpr auto gender = "gender";
            static constexpr auto birth_date = "birth_date";
            static constexpr auto contact_information = "contact_information";
        };

        Patient() = default;


        Patient(const int32_t id, std::string name, std::string gender, std::chrono::year_month_day const birth_date,
                std::string contact_information) :
            ObjectBase(id), name_(std::move(name)),
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

        Json::Value to_json() override
        {
            Json::Value result;
            result[field_name::id] = id_;
            result[field_name::name] = name_;
            result[field_name::gender] = gender_;
            result[field_name::birth_date]["year"] = static_cast<int>(birth_date_.year());
            result[field_name::birth_date]["month"] = static_cast<uint>(birth_date_.month());
            result[field_name::birth_date]["day"] = static_cast<uint>(birth_date_.day());
            result[field_name::properties] = collection_.make_properties_field()->value();

            return result;
        }

        void from_json(const Json::Value& val) override
        {
            id_ = val[field_name::id].asInt();
            name_ = val[field_name::name].asString();
            gender_ = val[field_name::gender].asString();

            if (const Json::Value& birthDate = val[field_name::birth_date]; birthDate.isMember("year") && birthDate.
                isMember("month") && birthDate.isMember("day"))
            {
                const int32_t year = birthDate["year"].asInt();
                const uint32_t month = birthDate["month"].asUInt();
                const uint32_t day = birthDate["day"].asUInt();
                birth_date_ = std::chrono::year_month_day{
                    std::chrono::year{year},
                    std::chrono::month{month},
                    std::chrono::day{day}
                };
            }
            create_collection(val[field_name::properties]);
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
        std::string name_;
        std::string gender_;
        std::chrono::year_month_day birth_date_{};
        std::string contact_information_;
    };
}
