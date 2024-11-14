#pragma once
#include "data_property.hpp"

namespace drug_lib::data::objects::organizations
{
    struct properties : DataProperty::_common_properties
    {
        static constexpr auto license = "license";
    };

}
