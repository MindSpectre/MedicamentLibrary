#pragma once
#include "data_property.hpp"
namespace drug_lib::data::objects::patients
{
    struct properties : DataProperty::_common_properties
    {
        static constexpr auto current_diseases = "current_diseases";
    };
}