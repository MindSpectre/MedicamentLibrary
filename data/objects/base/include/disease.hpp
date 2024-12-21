#pragma once
#include <utility>
#include <algorithm>

#include "common_object.hpp"
#include "properties_controller.hpp"
namespace drug_lib::data::objects
{
    namespace disease::field_name
    {
        static constexpr char name[] = "name";
        static constexpr char type[] = "type";
        static constexpr char is_infectious[] = "is_infectious";
    };
    class Disease final : public ObjectBase, public PropertiesHolder
    {
    public:

        [[nodiscard]] common::database::Record to_record() const override
        {
            common::database::Record record;
            record.push_back(std::make_unique<common::database::Field<common::database::Uuid>>(shared::field_name::id, id_));
            record.push_back(std::make_unique<common::database::Field<std::string>>(disease::field_name::name, name_));
            record.push_back(std::make_unique<common::database::Field<std::string>>(disease::field_name::type, type_));
            record.push_back(
                std::make_unique<common::database::Field<bool>>(disease::field_name::is_infectious, is_infectious_));
            record.push_back(collection_.make_properties_field());
            return record;
        }

        void from_record(const common::database::Record& record) override
        {
            for (const auto& field : record.fields())
            {
                if (const auto& field_name = field->get_name();
                    field_name == shared::field_name::id)
                {
                    id_ = field->as<common::database::Uuid>();
                }
                else if (field_name == disease::field_name::name)
                {
                    name_ = field->as<std::string>();
                }
                else if (field_name == disease::field_name::type)
                {
                    type_ = field->as<std::string>();
                }
                else if (field_name == disease::field_name::is_infectious)
                {
                    is_infectious_ = field->as<bool>();
                }
                else if (field_name == shared::field_name::properties)
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
                    field_name == shared::field_name::id)
                {
                    id_ = viewed->extract(i);
                }
                else if (field_name == disease::field_name::name)
                {
                    name_ = viewed->extract(i);
                }
                else if (field_name == disease::field_name::type)
                {
                    type_ = viewed->extract(i);
                }
                else if (field_name == disease::field_name::is_infectious)
                {
                    //WARNING this is wierd stuff
                    is_infectious_ = viewed->extract(i) == "t";
                }
                else if (field_name == shared::field_name::properties)
                {
                    create_collection(viewed->extract(i));
                }
                else
                {
                    throw std::invalid_argument("Unknown field name: " + field_name);
                }
            }
        }

        [[nodiscard]] [[nodiscard]] Json::Value to_json() const  override
        {
            Json::Value result = ObjectBase::to_json();
            result[disease::field_name::name] = name_;
            result[disease::field_name::type] = type_;
            result[disease::field_name::is_infectious] = is_infectious_;
            result[shared::field_name::properties] = collection_.make_properties_field()->value();
            return result;
        }

        void from_json(const Json::Value& val) override
        {
            ObjectBase::from_json(val);
            name_ = val[disease::field_name::name].asString();
            type_ = val[disease::field_name::type].asString();
            try
            {
                is_infectious_ = val[disease::field_name::is_infectious].asBool();
            } catch ([[maybe_unused]] const Json::LogicError& e)
            {
                std::string is_infectious_string = val[disease::field_name::is_infectious].asString();
                std::ranges::transform(is_infectious_string, is_infectious_string.begin(), ::tolower);
                if (is_infectious_string != "true" && is_infectious_string != "false")
                {
                    throw;
                }
                is_infectious_ = is_infectious_string == "true";
            }
            create_collection(val[shared::field_name::properties]);

        }

        Disease() = default;

        Disease(common::database::Uuid id, std::string name, std::string type, const bool is_infectious) :
            ObjectBase(std::move(id)), name_(std::move(name)),
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
