#pragma once

#include "common_obj.hpp"
#include "property_factory.hpp"

namespace drug_lib::data::objects
{
    class Organization final : public ObjectBase, public PropertiesHolder
    {
    public:
        struct fields : _cm_fields
        {
            static constexpr std::string type = "organization";
            static constexpr std::string country = "country";
        };

        [[nodiscard]] common::database::Record to_record() const override
        {
            common::database::Record record;
            record.push_back(std::make_unique<common::database::Field<int32_t>>(fields::id, id_));
            record.push_back(std::make_unique<common::database::Field<std::string>>(fields::name, name_));
            record.push_back(std::make_unique<common::database::Field<std::string>>(fields::country, country_));
            record.push_back(collection_.make_properties_field());
            return record;
        }

        void from_record(const common::database::Record& record) override
        {
            for (const auto& field : record.fields())
            {
                if (const auto& field_name = field->get_name(); field_name == fields::id)
                {
                    id_ = field->as<int32_t>();
                }
                else if (field_name == fields::name)
                {
                    name_ = field->as<std::string>();
                }
                else if (field_name == fields::country)
                {
                    country_ = field->as<std::string>();
                }
                else if (field_name == fields::properties)
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
                if (const auto& field_name = viewed->name(i); field_name == fields::id)
                {
                    id_ = std::stoi(viewed->extract(i));
                }
                else if (field_name == fields::name)
                {
                    name_ = viewed->extract(i);
                }
                else if (field_name == fields::country)
                {
                    country_ = viewed->extract(i);
                }
                else if (field_name == fields::properties)
                {
                    create_collection(viewed->extract(i));
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
