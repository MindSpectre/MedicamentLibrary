#pragma once

#include "super_handbook.hpp"

namespace drug_lib::services
{
    class PatientEditor final
    {
    public:
        void CreatePatient(data::objects::Patient& p_object) const;

    private:
        dao::SuperHandbook handbook_;
    };
}
