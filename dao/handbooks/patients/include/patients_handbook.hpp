#pragma once

#include "handbook.hpp"
#include "patients.hpp"

namespace drug_lib::dao 
{
    using namespace drug_lib::data;
    class PatientsHandbook : public HandbookBase<objects::PatientBase>  
    {
    private:
        /* data */
    public:
        PatientsHandbook(/* args */);
        ~PatientsHandbook();
    };
    
    PatientsHandbook::PatientsHandbook(/* args */)
    {
    }
    
    PatientsHandbook::~PatientsHandbook()
    {
    }
    
}