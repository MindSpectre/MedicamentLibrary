#pragma once

#include "auth_data_holder.hpp"
#include "security_utils.hpp"
namespace drug_lib::services
{
	class AuthenticatorServiceInternal
	{
	public:
		[[nodiscard]] bool login(std::string_view username, std::string_view password)
		{
			data::objects::AuthObject user;
			try
			{
				user = auth_data_holder_.get_by_login(username.data());
			} catch (const common::database::exceptions::InvalidIdentifierException &e)
			{
				if (e.get_error() == common::database::errors::db_error_code::RECORD_NOT_FOUND)
				{
					// TODO: suggest signup
				}
			}
			//TODO: compare hashes
			std::string stored_hash = user.get_password_hash();
			std::string calculated_hash = password.data();
			if (common::utilities::security::constant_time_compare(stored_hash, calculated_hash))
			{
				return true;
			}
			return false;
		}
		void signup(std::string_view username, std::string_view password) const
		{
			data::objects::AuthObject new_user;
			new_user.set_login(username.data());
			//TODO: calculate hash
			new_user.set_password_hash(password.data());
			new_user.set_permissions(data::objects::auth::roles_permissions::patient);
			try
			{
				data::objects::AuthObject user = auth_data_holder_.get_by_login(username.data());
			} catch (common::database::exceptions::InvalidIdentifierException &e)
			{
				if (e.get_error() == common::database::errors::db_error_code::RECORD_NOT_FOUND)
				{
					auth_data_holder_.insert(new_user);
				} else
				{
					throw;
				}
			}
			//TODO: replace with custom exception
			throw std::invalid_argument("User is already exist");

		}
		AuthenticatorServiceInternal() = default;
		void setup_from_one(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
		{
			auth_data_holder_.set_connection(connect);
		}

		void pool_setup(common::database::creational::DbInterfacePool &pool)
		{
			auth_data_holder_.establish_from_pool(pool);
		}

		explicit AuthenticatorServiceInternal(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
		{
			setup_from_one(connect);
		}
	private:
		dao::AuthDataHolder auth_data_holder_;
	};
} // namespace drug_lib::services
