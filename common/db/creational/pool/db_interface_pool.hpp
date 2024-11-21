#pragma once

#include <memory>
#include <iostream>
#include "db_interface.hpp"

namespace drug_lib::common::database::creational
{
    class DbInterfacePool
    {
    public:
        std::shared_ptr<interfaces::DbInterface> acquire_db_interface()
        {
            if (!pool_.empty())
            {
                auto obj = std::move(pool_.top());
                pool_.pop();
                return obj;
            }
            throw std::runtime_error("Pool is exhausted.\t");
        }

        void release_db_interface(std::shared_ptr<interfaces::DbInterface>&& obj)
        {
            pool_.push(std::move(obj));
        }

        // Fills the pool with a specified number of instances, created by the provided factory function
        template <typename FactoryFunc, typename... Args>
            requires std::invocable<FactoryFunc, Args...> &&
            std::same_as<std::invoke_result_t<FactoryFunc, Args...>, std::unique_ptr<interfaces::DbInterface>> ||
            std::same_as<std::invoke_result_t<FactoryFunc, Args...>, std::shared_ptr<interfaces::DbInterface>>
        void fill(const std::size_t size, FactoryFunc&& factory, Args&&... args)
        {
            for (std::size_t i = 0; i < size; ++i)
            {
                if (std::shared_ptr<interfaces::DbInterface> db_interface = factory(std::forward<Args>(args)...))
                {
                    pool_.push(std::move(db_interface));
                }
                else
                {
                    throw std::runtime_error("Factory function failed to create a valid DbInterface instance.\t");
                }
            }
        }

        void clear()
        {
            while (!pool_.empty())
            {
                pool_.top()->drop_connect();
                pool_.pop();
            }
            std::cout << "Destructed pool." << std::endl;
        }

        DbInterfacePool() = default;

        ~DbInterfacePool()
        {
            clear();
        }

    private:
        std::stack<std::shared_ptr<interfaces::DbInterface>> pool_;
    };
}
