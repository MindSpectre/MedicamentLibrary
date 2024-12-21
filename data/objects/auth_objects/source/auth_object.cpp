#include "auth_object.hpp"

drug_lib::common::database::Record drug_lib::data::objects::AuthObject::to_record() const
{
    common::database::Record record;
    record.push_back(std::make_unique<common::database::Field<common::database::Uuid>>(auth_object::field_name::user_id, user_id_));
    record.push_back(std::make_unique<common::database::Field<std::string>>(auth_object::field_name::login, login_));
    record.push_back(std::make_unique<common::database::Field<std::string>>(auth_object::field_name::password, password_hash_));
    record.push_back(std::make_unique<common::database::Field<std::string>>(auth_object::field_name::role, role_));
    return record;
}

void drug_lib::data::objects::AuthObject::from_record(const common::database::Record& record)
{
    for (const auto& field : record.fields())
    {
        if (const auto& field_name = field->get_name();
            field_name == auth_object::field_name::user_id)
        {
            user_id_ = field->as<common::database::Uuid>();
        }
        else if (field_name == auth_object::field_name::login)
        {
            login_ = field->as<std::string>();
        }
        else if (field_name == auth_object::field_name::password)
        {
            password_hash_ = field->as<std::string>();
        }
        else if (field_name == auth_object::field_name::role)
        {
            role_ = field->as<std::string>();
        }
        else
        {
            throw std::invalid_argument("Unknown field name: " + field_name);
        }
    }
}


void drug_lib::data::objects::AuthObject::add_permission(const auth::Permissions permission)
{
    permissions_.insert(permission);
}

void drug_lib::data::objects::AuthObject::remove_permission(const auth::Permissions permission)
{
    permissions_.erase(permission);
}

bool drug_lib::data::objects::AuthObject::has_permission(const auth::Permissions permission) const
{
    return permissions_.contains(permission);
}

void drug_lib::data::objects::AuthObject::change_role(std::string role)
{
    role_ = std::move(role);
    permissions_.clear();
    if (role_ == auth::roles_names::free_user)
    {
        permissions_ = auth::roles_permissions::free_user;
    }
    else if (role_ == auth::roles_names::patient)
    {
        permissions_ = auth::roles_permissions::patient;
    }
    else if (role_ == auth::roles_names::nurse)
    {
        permissions_ = auth::roles_permissions::nurse;
    }
    else if (role_ == auth::roles_names::doctor)
    {
        permissions_ = auth::roles_permissions::doctor;
    }
    else if (role_ == auth::roles_names::administrator)
    {
        permissions_ = auth::roles_permissions::administrator;
    }
    else if (role_ == auth::roles_names::sudo)
    {
        permissions_ = auth::roles_permissions::sudo;
    }
    else
        throw std::invalid_argument("Invalid role");
}
