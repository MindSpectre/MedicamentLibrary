#pragma once
namespace ControlSystemV7::Common::Database::Strategies
{
    template<typename... Args>
    class StrategyBase {
    protected:
        bool enabled = false;
        bool supported = false;
        std::function<void(Args...)> action;
        virtual void InternalLogic(Args... v) {
            std::cerr << "DOESNT DECLARED IN THIS SCOPE" << std::endl;
        };
    public:
        StrategyBase(std::function<void(Args...)> func, bool support = false) : action(func), supported(support) {}
        StrategyBase() = default;
        void SetAction(std::function<void(Args...)> action){
            this->action = action;
        }
        void SetSupport(bool dbStrategySupport) { supported = dbStrategySupport;}
        bool IsSupported() const {return supported;} 
        void Enable() { enabled = true; }
        void Disable() { enabled = false; }
        bool IsEnabled() const { return enabled; }

        void execute(Args... args) {
            if (enabled && !supported ){
                throw std::runtime_error("Unavailable strategy");
            }
            if (enabled && action) {
                InternalLogic(args...);
            }
        }
    };
}

