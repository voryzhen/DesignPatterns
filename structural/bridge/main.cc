#include <cstddef>
#include <iostream>
#include <memory>
#include <string>

class ICore
{
    public:
        virtual ~ICore() { std::cout << "Core has been destroyed\n"; };
        virtual std::string coreCmd() const = 0;
};

class CoreLibA : public ICore
{
    public:
        CoreLibA() = default;
        ~CoreLibA() = default;
        std::string coreCmd() const override { return "cmd result from LibA"; }
};

class CoreLibB : public ICore
{
    public:
        CoreLibB() = default;
        ~CoreLibB() = default;
        std::string coreCmd() const override { return "cmd result from LibB"; }
};

class BaseApp
{
    public:
        BaseApp(const std::shared_ptr<ICore>& coreImpl) : _core(coreImpl) {};
        virtual ~BaseApp() { std::cout << "Client has been destroyed\n"; };
        virtual std::string cmd() const
        {
            return _core ? "Base app cmd result: " + _core->coreCmd()
                         : "Error: no core implimentation found";
        }

        void changeImplimentation(const std::shared_ptr<ICore>& coreImpl)
        {
            _core = coreImpl;
        }

    protected:
        std::shared_ptr<ICore> _core { nullptr };
};

class MainApp : public BaseApp
{
    public:
        MainApp(const std::shared_ptr<ICore>& coreImpl) : BaseApp(coreImpl) {};
        ~MainApp() {}
        std::string cmd() const override
        {
            return _core ? "Main app cmd result: " + _core->coreCmd()
                         : "Error: no core implimentation found";
        }
};

void commandFunction(const MainApp& app)
{
    std::cout << "Do the stuff: " << app.cmd() << "\n";
}

int main()
{
    commandFunction(MainApp { std::make_shared<CoreLibA>() });
    commandFunction(MainApp { std::make_shared<CoreLibB>() });

    MainApp client { std::make_shared<CoreLibB>() };
    commandFunction(client);
    client.changeImplimentation(std::make_shared<CoreLibA>());
    commandFunction(client);
}
