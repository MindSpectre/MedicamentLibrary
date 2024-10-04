#pragma once

#include "common_obj.hpp"
#include "property_factory.hpp"

namespace drug_lib::data::objects
{
    class Organization final : public ObjectBase, public PropertiesHolder
    {
    public:
        [[nodiscard]] common::database::Record to_record() const override
        {
            common::database::Record record;
            record.add_field(std::make_shared<common::database::Field<int32_t>>("id", id_));
            record.add_field(std::make_shared<common::database::Field<std::string>>("name", name_));
            record.add_field(std::make_shared<common::database::Field<std::string>>("country", country_));
            record.add_field(collection_.make_properties_field());
            return record;
        }

        void from_record(const common::database::Record& record) override
        {
            for (const auto& [field_name, field] : record.fields())
            {
                if (field_name == "id")
                {
                    id_ = field->as<int32_t>();
                }
                else if (field_name == "name")
                {
                    name_ = field->as<std::string>();
                }
                else if (field_name == "country")
                {
                    country_ = field->as<std::string>();
                }
                else if (field_name == properties::properties)
                {
                    create_collection(field);
                }
                else
                {
                    throw std::invalid_argument("Unknown field name: " + field_name);
                }
            }
        }

        Organization() = default;

        // Конструктор с параметрами
        Organization(const int32_t id, std::string name, std::string country)
            : id_(id), name_(std::move(name)), country_(std::move(country))
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

        [[nodiscard]] std::string get_name() const
        {
            return name_;
        }

        void set_name(const std::string& name)
        {
            name_ = name;
        }

        [[nodiscard]] std::string get_country() const
        {
            return country_;
        }

        void set_country(const std::string& country)
        {
            country_ = country;
        }

        ~Organization() override = default;

    private:
        int32_t id_ = -1;
        std::string name_;
        std::string country_;
    };
}
