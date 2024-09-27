#pragma once

#include "handbook.hpp"
#include "medicaments.hpp"

namespace drug_lib::dao 
{
    using namespace drug_lib::data;
    class MedicamentsHandbook : public HandbookBase<objects::MedicamentBase>  
    {
    private:
        /* data */
    public:
        MedicamentsHandbook(/* args */);
        ~MedicamentsHandbook();
    };
    
    MedicamentsHandbook::MedicamentsHandbook(/* args */)
    {
    }
    
    MedicamentsHandbook::~MedicamentsHandbook()
    {
    }
    
}