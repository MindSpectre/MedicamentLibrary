#pragma once
#include <string>
#include <unordered_set>
#include "db_record.hpp"

namespace drug_lib::data::objects
{
    namespace Auth
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
        void add_permission(Auth::Permissions permission);
        void remove_permission(Auth::Permissions permission);
        bool has_permission(Auth::Permissions permission) const;
        void change_role(std::string role);
        AuthObject() = default;

        AuthObject(const int32_t user_id, std::string role, std::string login, std::string password_hash,
                   std::unordered_set<Auth::Permissions>&& permissions)
            : user_id_(user_id),
              role_(std::move(role)),
              login_(std::move(login)),
              password_hash_(std::move(password_hash)),
              permissions_(std::move(permissions))
        {
        }

    private:
        int32_t user_id_{};
        std::string role_;
        std::string login_;
        std::string password_hash_;
        std::unordered_set<Auth::Permissions> permissions_;
    };
}
