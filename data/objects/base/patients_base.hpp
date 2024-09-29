#pragma once

#include "common_obj.hpp"
#include "db_field.hpp"
#include "db_conditions.hpp"
#include "db_record.hpp"

namespace drug_lib::data::objects
{
    class Patient final : public ObjectBase, public PropertiesHolder
    {
    public:
        Patient() = default;

        // Конструктор с параметрами
        Patient(const int32_t id, std::string name, std::string surname)
            : id_(id), name_(std::move(name)), surname_(std::move(surname))
        {
        }

        ~Patient() override = default;

        [[nodiscard]] common::database::Record to_record() const override
        {
            common::database::Record record;
            record.add_field(std::make_shared<common::database::Field<int32_t>>("id", id_));
            record.add_field(std::make_shared<common::database::Field<std::string>>("name", name_));
            record.add_field(std::make_shared<common::database::Field<std::string>>("surname", surname_));
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
                if (field_name == "name")
                {
                    name_ = field->as<std::string>();
                }
                if (field_name == "surname")
                {
                    surname_ = field->as<std::string>();
                }
                if (field_name == properties::properties)
                {
                    create_collection(field);
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

        [[nodiscard]] std::string get_name() const
        {
            return name_;
        }

        void set_name(const std::string& name)
        {
            name_ = name;
        }

        [[nodiscard]] std::string get_surname() const
        {
            return surname_;
        }

        void set_surname(const std::string& surname)
        {
            surname_ = surname;
        }

    private:
        int32_t id_ = -1;
        std::string name_;
        std::string surname_;
    };
}
