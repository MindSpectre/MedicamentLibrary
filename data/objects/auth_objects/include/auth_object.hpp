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

        struct roles_permissions
        {
            static constexpr auto free_user = {Browse};
            static constexpr auto patient = {Browse, EditPatientInfo};
            static constexpr auto nurse = {Browse, EditPatientInfo, Treat};
            static constexpr auto doctor = {Browse, EditPatientInfo, Treat, AssignDrugs};
            static constexpr auto administrator = {Browse, Administrate};
            static constexpr auto sudo = {Sudo};
        };

        struct roles_names
        {
            static constexpr auto free_user = "user";
            static constexpr auto patient = "patient";
            static constexpr auto nurse = "nurse";
            static constexpr auto doctor = "doctor";
            static constexpr auto administrator = "administrator";
            static constexpr auto sudo = "sudo";
        };
    }

    class AuthObject final
    {
    public:
        struct field_name
        {
            static constexpr auto user_id = "user_id";
            static constexpr auto role = "role";
            static constexpr auto login = "login";
            static constexpr auto password = "password";
        };

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
        [[nodiscard]] [[nodiscard]] Json::Value to_json() const
        {
            Json::Value result;
            result[field_name::user_id] = user_id_.get_id();
            result[field_name::login] = login_;
            result[field_name::password] = password_hash_;
            return result;
        }

        void from_json(const Json::Value& val)
        {
            if (val.isMember(field_name::user_id))
            {
                user_id_.set_id(val[field_name::user_id].asString()) ;
            } else
            {
                user_id_.set_id(common::database::Uuid::default_value);
            }
            login_ = val[field_name::login].asString();
            // Todo: crete hash calc because in json stores raw value
            password_hash_ = val[field_name::password].asString();

        }
    private:
        common::database::Uuid user_id_{};
        std::string role_;
        std::string login_;
        std::string password_hash_;
        std::unordered_set<auth::Permissions> permissions_;
    };
}
