#pragma once


#include "common_object.hpp"
#include "properties_controller.hpp"

namespace drug_lib::data::objects
{
    class Disease final : public ObjectBase, public PropertiesHolder
    {
    public:
        struct field_name : common_fields_names
        {
            static constexpr auto type = "type";
            static constexpr auto is_infectious = "is_infectious";
        };

        [[nodiscard]] common::database::Record to_record() const override
        {
            common::database::Record record;
            record.push_back(std::make_unique<common::database::Field<int32_t>>(field_name::id, id_));
            record.push_back(std::make_unique<common::database::Field<std::string>>(field_name::name, name_));
            record.push_back(std::make_unique<common::database::Field<std::string>>(field_name::type, type_));
            record.push_back(
                std::make_unique<common::database::Field<bool>>(field_name::is_infectious, is_infectious_));
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
                else if (field_name == field_name::is_infectious)
                {
                    is_infectious_ = field->as<bool>();
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
                else if (field_name == field_name::is_infectious)
                {
                    is_infectious_ = viewed->extract(i) == "true";
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

        Json::Value serialize() override
        {
        }

        Disease() = default;

        Disease(const int32_t id, std::string name, std::string type, const bool is_infectious)
            : id_(id),
              name_(std::move(name)),
              type_(std::move(type)),
              is_infectious_(is_infectious)
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


        [[nodiscard]] bool is_is_infectious() const
        {
            return is_infectious_;
        }

        void set_is_infectious(const bool is_infectious)
        {
            is_infectious_ = is_infectious;
        }


        ~Disease() override = default;

    private:
        int32_t id_ = -1;
        std::string name_;
        std::string type_;
        bool is_infectious_ = false;
    };
}
