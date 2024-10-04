#pragma once


#include "batcher_strategy.hpp"
#include "logger_strategy.hpp"
#include "db_interface.hpp"

namespace drug_lib::common::database::behavioral::strategies
{
    using namespace drug_lib::common::database::interfaces;

    template <typename... Args>
    class StrategyControl
    {
    public:
        LoggerStrategy<Args...> Logger;
        BatcherStrategy<Args...> Batcher;

        explicit StrategyControl(DbInterface& db)
        {
        }

        StrategyControl() = default;
    };
}
