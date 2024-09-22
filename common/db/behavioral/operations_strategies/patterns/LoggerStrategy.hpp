#pragma once

#include "StrategyBase.hpp"

namespace ControlSystemV7::Common::Database::Strategies
{
    template <typename... Args>
    class LoggerStrategy : public StrategyBase<Args...> {
    private:
        void InternalLogic(Args... query)  override {
            std::cerr << "LOGGING:......\n";
            std::cerr << "Current status query: NOT EXECUTED.\n";
            Print(query...);
            try{
                this->action(query...);
                
            } catch (std::exception &exc){
                std::cerr << "CATCHED ERROR\n";
                throw;
            }
            std::cerr << "EXECUTION COMPLETED\n";
        }
        std::function<void(Args...)> Print;
    public:
        LoggerStrategy() = default;
        LoggerStrategy(std::function<void(Args...)> func, bool supported, std::function<void(Args...)> externalProcess) : StrategyBase<Args...>(func, supported), Print(externalProcess) {

        }
        ~LoggerStrategy() = default;
        void SetPrint(std::function<void(Args...)> externalProcess) 
        {
            Print = externalProcess;
        }
    };
}