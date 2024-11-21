#pragma once

#include "common_object.hpp"
#include "properties_controller.hpp"

namespace drug_lib::data::objects
{
    class Organization final : public ObjectBase, public PropertiesHolder
    {
    public:
        struct field_name : common_fields_names
        {
            static constexpr auto type = "type";
            static constexpr auto country = "country";
            static constexpr auto contact_details = "contact_details";
        };

        [[nodiscard]] common::database::Record to_record() const override
        {
            common::database::Record record;
            record.push_back(std::make_unique<common::database::Field<int32_t>>(field_name::id, id_));
            record.push_back(std::make_unique<common::database::Field<std::string>>(field_name::name, name_));
            record.push_back(std::make_unique<common::database::Field<std::string>>(field_name::type, type_));
            record.push_back(std::make_unique<common::database::Field<std::string>>(field_name::country, country_));
            record.push_back(
                std::make_unique<common::database::Field<std::string>>(field_name::contact_details, contact_details_));
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
                else if (field_name == field_name::type)
                {
                    type_ = field->as<std::string>();
                }
                else if (field_name == field_name::country)
                {
                    country_ = field->as<std::string>();
                }
                else if (field_name == field_name::contact_details)
                {
                    contact_details_ = field->as<std::string>();
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
                if (const auto& field_name = viewed->name(i);
                    field_name == field_name::id)
                {
                    id_ = std::stoi(viewed->extract(i));
                }
                else if (field_name == field_name::name)
                {
                    name_ = viewed->extract(i);
                }
                else if (field_name == field_name::type)
                {
                    type_ = viewed->extract(i);
                }
                else if (field_name == field_name::country)
                {
                    country_ = viewed->extract(i);
                }
                else if (field_name == field_name::contact_details)
                {
                    contact_details_ = viewed->extract(i);
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
            result[field_name::type] = type_;
            result[field_name::contact_details] = contact_details_;
            result[field_name::country] = country_;
            result[field_name::properties] = collection_.make_properties_field()->value();
            return result;
        }

        Organization() = default;

        // Конструктор с параметрами
        Organization(const int32_t id, std::string name, std::string type, std::string country,
                     std::string contact_details)
            : id_(id),
              name_(std::move(name)),
              type_(std::move(type)),
              country_(std::move(country)),
              contact_details_(std::move(contact_details))
        {
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

        [[nodiscard]] const std::string& get_type() const
        {
            return type_;
        }

        void set_type(const std::string& type)
        {
            type_ = type;
        }

        [[nodiscard]] const std::string& get_country() const
        {
            return country_;
        }

        void set_country(const std::string& country)
        {
            country_ = country;
        }

        [[nodiscard]] const std::string& get_contact_details() const
        {
            return contact_details_;
        }

        void set_contact_details(const std::string& contact_details)
        {
            contact_details_ = contact_details;
        }

        ~Organization() override = default;

    private:
        int32_t id_ = -1;
        std::string name_;
        std::string type_;
        std::string country_;
        std::string contact_details_;
    };
}
