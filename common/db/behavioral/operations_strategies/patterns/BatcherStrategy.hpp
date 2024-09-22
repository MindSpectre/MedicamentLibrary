#pragma once
#include "StrategyBase.hpp"

namespace ControlSystemV7::Common::Database::Strategies
{
    template <typename... Args>
    class BatcherStrategy : public StrategyBase<Args...>
    {
    private:
        void InternalLogic(Args... query) override
        {
            await.push(std::make_tuple(query...));

            if (await.size() >= threshold)
            {
                auto result = Merge(*this);
                std::apply(this->action, result);
            }
        }
        size_t threshold = 1;
        std::function<std::tuple<Args...>(BatcherStrategy<Args...> &)> Merge;
        
    public:
        std::queue<std::tuple<Args...>> await;
        void SetThreshold(size_t threshold) { this->threshold = threshold; }
        int32_t GetThreshold() const { return threshold; }
        BatcherStrategy() = default;
        ~BatcherStrategy()
        {
            if (Merge && await.size()!=0)
            {
                auto result = Merge(*this);
                if (this->action)
                {
                    std::apply(this->action, result); // Call the inherited action method
                }
            }
        }
        void SetMerge(std::function<std::tuple<Args...>(BatcherStrategy<Args...> &)> externalProcess)
        {
            Merge = externalProcess;
        }
    };
}