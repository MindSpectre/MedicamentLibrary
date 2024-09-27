#pragma once

#include "db_field.hpp"
#include "db_conditions.hpp"
#include "db_record.hpp"

namespace drug_lib::data::objects
{
    class OrganizationBase 
    {
    public:
        OrganizationBase() = default;

        // Конструктор с параметрами
        OrganizationBase(std::string name, std::string country)
            : name_(std::move(name)), country_(std::move(country)) {}

        virtual ~OrganizationBase() = default;
    private:
        std::string name_;
        std::string country_;
    };
}