#pragma once


#include "common_object.hpp"
#include "properties_controller.hpp"
#include <iostream>
namespace drug_lib::data::objects
{
    class Disease final : public ObjectBase, public PropertiesHolder
    {
    public:
        struct field_name : common_fields_names
        {
            static constexpr auto name = "name";
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

        Json::Value to_json() override
        {
            Json::Value result;
            result[field_name::id] = id_;
            result[field_name::name] = name_;
            result[field_name::type] = type_;
            result[field_name::is_infectious] = is_infectious_;
            result[field_name::properties] = collection_.make_properties_field()->value();
            return result;
        }

        void from_json(const Json::Value& val) override
        {
            try
            {
                id_ = val[field_name::id].asInt();
                name_ = val[field_name::name].asString();
                type_ = val[field_name::type].asString();
                is_infectious_ = val[field_name::is_infectious].asBool();
            } catch (const std::exception& e)
            {
                std::cout << e.what() << "Basic\n";
            }
            try
            {
                create_collection(val[field_name::properties]);
            } catch (const std::exception& e)
            {
                std::cout << e.what() << "Collection\n";
            }

        }

        Disease() = default;

        Disease(const int32_t id, std::string name, std::string type, const bool is_infectious) :
            ObjectBase(id), name_(std::move(name)),
            type_(std::move(type)),
            is_infectious_(is_infectious)
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


        [[nodiscard]] bool is_infectious() const
        {
            return is_infectious_;
        }

        void set_infectious(const bool is_infectious)
        {
            is_infectious_ = is_infectious;
        }


        ~Disease() override = default;

        friend bool operator==(const Disease& lhs, const Disease& rhs)
        {
            return lhs.id_ == rhs.id_
                && lhs.name_ == rhs.name_
                && lhs.type_ == rhs.type_
                && lhs.is_infectious_ == rhs.is_infectious_ && lhs.collection_ == rhs.collection_;
        }

        friend bool operator!=(const Disease& lhs, const Disease& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        std::string name_;
        std::string type_;
        bool is_infectious_ = false;
    };
}
