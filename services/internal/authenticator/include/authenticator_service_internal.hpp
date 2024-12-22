#pragma once
#include "salt_generator.hpp"
#include "hash_creator_factory.hpp"
#include "auth_data_holder.hpp"
#include "security_utils.hpp"

namespace drug_lib::services
{
	class AuthenticatorServiceInternal
	{
	public:
		[[nodiscard]] bool login(const std::string_view username, const std::string_view password) const
		{
			data::objects::AuthObject user;
			try
			{
				user = auth_data_holder_.get_by_login(username.data());
			}
			catch (const common::database::exceptions::InvalidIdentifierException &e)
			{
				if (e.get_error() == common::database::errors::db_error_code::RECORD_NOT_FOUND)
				{
					// TODO: suggest signup
				}
			}
			if (common::utilities::security::constant_time_compare(user.get_password_hash(),
			                                                       hasher->hash_function(password, user.get_salt())))
			{
				return true;
			}
			return false;
		}

		void signup(std::string_view login, std::string_view password, const std::optional<std::string>& email) const
		{
			data::objects::AuthObject new_user;
			new_user.set_login(login.data());
			new_user.set_salt(common::crypto::SaltGenerator::generate_hex(31));
			std::string computed_password_hash = hasher->hash_function(password.data(), new_user.get_salt());
			new_user.set_password_hash(std::move(computed_password_hash));
			new_user.set_permissions(data::objects::auth::roles_permissions::sudo);
			new_user.set_role(data::objects::auth::roles_names::sudo);
			new_user.set_email(email.value_or("example@example.com"));
			new_user.set_user_id(common::database::Uuid().set_null());
			try
			{
				data::objects::AuthObject user = auth_data_holder_.get_by_login(login.data());
			}
			catch (common::database::exceptions::InvalidIdentifierException &e)
			{
				if (e.get_error() == common::database::errors::db_error_code::RECORD_NOT_FOUND)
				{
					auth_data_holder_.insert(new_user);
					return;
				}
			}
			//TODO: replace with custom exception
			throw std::invalid_argument("User is already exist");
		}

		AuthenticatorServiceInternal() : hasher(common::crypto::HashCreatorFactory::CreatePBKDF2Coder())
		{
		}

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
			hasher = common::crypto::HashCreatorFactory::CreatePBKDF2Coder();
		}

	private:
		std::unique_ptr<common::crypto::HashCreator> hasher;
		dao::AuthDataHolder auth_data_holder_;
	};
} // namespace drug_lib::services
