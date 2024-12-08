#pragma once

#include <utility>

#include "common_object.hpp"
#include "properties_controller.hpp"

namespace drug_lib::data::objects
{
    class Organization final : public ObjectBase, public PropertiesHolder
    {
    public:
        struct field_name : common_fields_names
        {
            static constexpr auto name = "name";
            static constexpr auto type = "type";
            static constexpr auto country = "country";
            static constexpr auto contact_details = "contact_details";
        };

        [[nodiscard]] common::database::Record to_record() const override
        {
            common::database::Record record;
            record.push_back(std::make_unique<common::database::Field<common::database::Uuid>>(field_name::id, id_));
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
                    id_ = field->as<common::database::Uuid>();
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
                    id_ = viewed->extract(i);
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

        [[nodiscard]] Json::Value to_json() const override
        {
            Json::Value result = ObjectBase::to_json();
            result[field_name::name] = name_;
            result[field_name::type] = type_;
            result[field_name::contact_details] = contact_details_;
            result[field_name::country] = country_;
            result[field_name::properties] = collection_.make_properties_field()->value();
            return result;
        }

        void from_json(const Json::Value& val) override
        {
            ObjectBase::from_json(val);
            name_ = val[field_name::name].asString();
            type_ = val[field_name::type].asString();
            contact_details_ = val[field_name::contact_details].asString();
            country_ = val[field_name::country].asString();
            create_collection(val[field_name::properties]);
        }

        Organization() = default;

        // Конструктор с параметрами
        Organization(common::database::Uuid id, std::string name, std::string type, std::string country,
                     std::string contact_details) :
            ObjectBase(std::move(id)), name_(std::move(name)),
            type_(std::move(type)),
            country_(std::move(country)),
            contact_details_(std::move(contact_details))
        {
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
        std::string name_;
        std::string type_;
        std::string country_;
        std::string contact_details_;
    };
}
