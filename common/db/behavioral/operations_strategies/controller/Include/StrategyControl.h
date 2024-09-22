#pragma once

#include <iostream>
#include <exception>
#include <functional>
#include <vector>
#include <memory>
#include <queue>
#include "Strategies/BatcherStrategy.hpp"
#include "Strategies/LoggerStrategy.hpp"
#include "IStationDatabase.hpp"
namespace ControlSystemV7::Common::Database::Strategies
{
    template <typename... Args>
    class StrategyControl {
    public:

        LoggerStrategy<Args...> Logger;
        BatcherStrategy<Args...> Batcher;

        StrategyControl(ControlSystemV7::Common::Database::IStationDatabase &db)  {

        }
        StrategyControl() {}
    };
}