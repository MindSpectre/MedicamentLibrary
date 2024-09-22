#pragma once

#include "handbook.hpp"
#include "organizations.hpp"

namespace drug_lib::dao 
{
    using namespace drug_lib::data;
    class OrganizationsHandbook : public HandbookBase<objects::OrganizationBase>  
    {
    private:
        /* data */
    public:
        OrganizationsHandbook(/* args */);
        ~OrganizationsHandbook();
    };
    
    OrganizationsHandbook::OrganizationsHandbook(/* args */)
    {
    }
    
    OrganizationsHandbook::~OrganizationsHandbook()
    {
    }
    
}