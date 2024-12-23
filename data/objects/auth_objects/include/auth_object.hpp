#pragma once
#include <string>
#include <unordered_set>
#include "db_record.hpp"

namespace drug_lib::data::objects
{
    namespace auth
    {
        enum Permissions
        {
            Browse,
            EditPatientInfo,
            Treat,
            AssignDrugs,
            Administrate,
            Sudo
        };

        namespace roles_permissions
        {
            constexpr auto free_user = {Browse};
            constexpr auto patient = {Browse, EditPatientInfo};
            constexpr auto nurse = {Browse, EditPatientInfo, Treat};
            constexpr auto doctor = {Browse, EditPatientInfo, Treat, AssignDrugs};
            constexpr auto administrator = {Browse, Administrate};
            constexpr auto sudo = {Sudo};
        };

        namespace roles_names
        {
            constexpr char free_user[] = "user";
            constexpr char patient[] = "patient";
            constexpr char nurse[] = "nurse";
            constexpr char doctor[] = "doctor";
            constexpr char administrator[] = "administrator";
            constexpr char sudo[] = "sudo";
        };
    }

    namespace auth_object::field_name
    {
        constexpr char user_id[] = "user_id";
        constexpr char role[] = "role";
        constexpr char login[] = "login";
        constexpr char password[] = "password";
        constexpr char email[] = "email";
        constexpr char salt[] = "salt";
    };
    class AuthObject final
    {
    public:

        [[nodiscard]] common::database::Record to_record() const;
        void from_record(const common::database::Record& record);
        void add_permission(auth::Permissions permission);
        void remove_permission(auth::Permissions permission);
        bool has_permission(auth::Permissions permission) const;
        void change_role(std::string role);
        AuthObject() = default;

        AuthObject( common::database::Uuid user_id, std::string role, std::string login, std::string password_hash,
                   std::unordered_set<auth::Permissions>&& permissions)
            : user_id_(std::move(user_id)),
              role_(std::move(role)),
              login_(std::move(login)),
              password_hash_(std::move(password_hash)),
              permissions_(std::move(permissions))
        {
        }

        [[nodiscard]] const common::database::Uuid & get_user_id() const
        {
            return user_id_;
        }

        void set_user_id(common::database::Uuid user_id)
        {
            user_id_ = std::move(user_id);
        }

        [[nodiscard]] const std::string & get_role() const
        {
            return role_;
        }

        void set_role(std::string role)
        {
            role_ = std::move(role);
        }

        [[nodiscard]] const std::string & get_login() const
        {
            return login_;
        }

        void set_login(std::string login)
        {
            login_ = std::move(login);
        }

        [[nodiscard]] const std::string & get_password_hash() const
        {
            return password_hash_;
        }

        void set_password_hash(std::string password_hash)
        {
            password_hash_ = std::move(password_hash);
        }

        [[nodiscard]] const std::unordered_set<auth::Permissions> & get_permissions() const
        {
            return permissions_;
        }

        void set_permissions(std::unordered_set<auth::Permissions> permissions)
        {
            permissions_ = std::move(permissions);
        }

        [[nodiscard]] const std::string & get_salt() const
        {
            return salt_;
        }

        void set_salt(std::string salt)
        {
            salt_ = std::move(salt);
        }

        [[nodiscard]] const std::string & get_email() const
        {
            return email_;
        }

        void set_email(std::string email)
        {
            email_ = std::move(email);
        }

        /**
         *
         * @return Json with stored properties i.e. salt password or others in hash format
         */
        [[nodiscard]] [[nodiscard]] Json::Value to_json() const
        {
            Json::Value result;
            result[auth_object::field_name::user_id] = user_id_.get_id();
            result[auth_object::field_name::login] = login_;
            result[auth_object::field_name::password] = password_hash_;
            result[auth_object::field_name::role] = role_;
            result[auth_object::field_name::email] = email_;
            result[auth_object::field_name::salt] = salt_;
            return result;
        }
        /**
         *
         * @param val All data should be stored in hashed format, especially password
         */
        void from_json(const Json::Value& val)
        {
            if (val.isMember(auth_object::field_name::user_id))
            {
                user_id_.set_id(val[auth_object::field_name::user_id].asString()) ;
            } else
            {
                user_id_.set_id(common::database::Uuid::default_value);
            }
            login_ = val[auth_object::field_name::login].asString();
            password_hash_ = val[auth_object::field_name::password].asString();
            role_ = val[auth_object::field_name::role].asString();
            email_ = val[auth_object::field_name::email].asString();
            salt_ = val[auth_object::field_name::salt].asString();
        }
    private:
        common::database::Uuid user_id_{};
        std::string role_;
        std::string login_;
        std::string password_hash_;
        std::string salt_;
        std::string email_;
        std::unordered_set<auth::Permissions> permissions_;
    };
}
