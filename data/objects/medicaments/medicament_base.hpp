#pragma once

#include "db_field.hpp"
#include "db_record.hpp"

#include <string>
#include <vector>
#include <memory>
#include <json/json.h>

namespace drug_lib::data::objects
{
    class MedicamentBase
    {
    public:
        // Конструктор по умолчанию
        MedicamentBase() = default;

        // Конструктор с параметрами
        MedicamentBase(std::string name, std::string manufacturer, double price)
            : name_(std::move(name)), manufacturer_(std::move(manufacturer)), price_(price) {}

        virtual ~MedicamentBase() = default;

        // Методы для получения и установки данных медикамента
        const std::string &get_name() const { return name_; }
        void set_name(const std::string &name) { name_ = name; }

        const std::string &get_manufacturer() const { return manufacturer_; }
        void set_manufacturer(const std::string &manufacturer) { manufacturer_ = manufacturer; }

        double get_price() const { return price_; }
        void set_price(double price) { price_ = price; }

        // Метод для преобразования медикамента в запись (Record)
        virtual drug_lib::common::database::Record to_record() const
        {
            drug_lib::common::database::Record newrec;
            newrec.add_field(std::make_shared<drug_lib::common::database::Field<std::string>>("name", name_));
            newrec.add_field(std::make_shared<drug_lib::common::database::Field<std::string>>("manufacturer", manufacturer_));
            newrec.add_field(std::make_shared<drug_lib::common::database::Field<double>>("price", price_));
            return std::move(newrec);
        }

        // Метод для обновления данных медикамента на основе записи
        virtual void from_record(const drug_lib::common::database::Record &record)
        {
            for (const auto &field : record.fields())
            {
                if (field->name() == "name")
                {
                    name_ = dynamic_cast<drug_lib::common::database::Field<std::string> *>(field.get())->value();
                }
                else if (field->name() == "manufacturer")
                {
                    manufacturer_ = dynamic_cast<drug_lib::common::database::Field<std::string> *>(field.get())->value();
                }
                else if (field->name() == "price")
                {
                    price_ = dynamic_cast<drug_lib::common::database::Field<double> *>(field.get())->value();
                }
            }
        }

        // Методы для работы с JSON (если нужно)
        virtual Json::Value to_json() const
        {
            Json::Value json;
            json["name"] = name_;
            json["manufacturer"] = manufacturer_;
            json["price"] = price_;
            return std::move(json);
        }

        virtual void from_json(const Json::Value &json)
        {
            if (json.isMember("name"))
                name_ = json["name"].asString();
            if (json.isMember("manufacturer"))
                manufacturer_ = json["manufacturer"].asString();
            if (json.isMember("price"))
                price_ = json["price"].asDouble();
        }

    private:
        std::string name_;
        std::string manufacturer_;
        double price_ = 0.0;
    };
}
