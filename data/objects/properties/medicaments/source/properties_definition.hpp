#pragma once
#include "data_property.hpp"

namespace drug_lib::data::objects::medicaments
{
    struct properties : DataProperty::_common_properties
    {
        static constexpr std::string prescription = "prescription";
    };

}
