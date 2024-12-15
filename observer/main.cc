#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

enum class EventType : uint8_t
{
    Info = 0,
    Error
};

std::string eventTypeDescription(EventType type)
{
    switch (type)
    {
        case EventType::Error:
            return "Error";
        case EventType::Info:
            return "Info";
        default:
            std::cout << "Unknown EventType: " << (int)type << "\n";
            return "";
    }
}

class Event
{
    public:
        EventType type { EventType::Error };
        std::string data { "" };
};

class Observer
{
    public:
        virtual ~Observer() = default;
        virtual bool update(const Event&) = 0;
};

class FileLoggerObserver : public Observer
{
    public:
        FileLoggerObserver() = default;
        ~FileLoggerObserver() {}

        bool update(const Event& e) override
        {
            // write to logFileName
            std::cout << "Error [" << e.data
                      << "] has been successfully written to the log file\n";
            return true;
        }

        void set_log(const std::string& fpath) { logFileName = fpath; }

    private:
        std::string logFileName {};
};

class EmailSenderObserver : public Observer
{
    public:
        EmailSenderObserver() = default;
        ~EmailSenderObserver() {}

        bool update(const Event& e) override
        {
            // Sent to email
            std::cout << "Error [" << e.data << "] has been successfully send to email\n";
            return true;
        }

        void set_email(const std::string& email) { emailAddr = email; }

    private:
        std::string emailAddr {};
};

class EventManager
{
    public:
        void generateEvent(EventType type, const std::string& data)
        {
            Event e { type, data };
            notify(type, e);
        }

        void subscribe(EventType type, const std::shared_ptr<Observer>& obserser)
        {
            auto it = _observers.find(type);
            if (it == _observers.end())
            {
                _observers.insert({ type, { obserser } });
            }
            else
            {
                it->second.push_back(obserser);
            }
            std::cout << "New observer has been successfully added\n";
        }

        void unsubscribe(EventType type, const std::shared_ptr<Observer>& obserser)
        {
            auto it = _observers.find(type);
            if (it == _observers.end())
            {
                std::cout
                    << "There are no any observers subscribed on this type of events: "
                    << eventTypeDescription(type) << "\n";
            }
            else
            {
                auto& v = it->second;
                v.erase(find(v.begin(), v.end(), obserser));
            }
            std::cout << "Observer has been successfully removed\n";
        }

        bool notify(EventType type, const Event& e)
        {
            auto it = _observers.find(type);
            if (it == _observers.end())
            {
                std::cout
                    << "There are no any observers subscribed on this type of events: "
                    << eventTypeDescription(type) << "\n";
                return false;
            }
            else
            {
                for (auto& obs : it->second)
                {
                    obs->update(e);
                }
            }
            return true;
        }

    private:
        std::unordered_map<EventType, std::vector<std::shared_ptr<Observer>>> _observers {};
};

int main()
{
    EventManager eg;

    const auto flo = std::make_shared<FileLoggerObserver>();
    const auto eso = std::make_shared<EmailSenderObserver>();

    eg.subscribe(EventType::Error, flo);
    eg.generateEvent(EventType::Error, "Error: {Reason: BDYSH}");

    eg.subscribe(EventType::Error, eso);
    eg.generateEvent(EventType::Error, "Error: {Reason: BDYSH2}");

    eg.unsubscribe(EventType::Error, flo);
    eg.unsubscribe(EventType::Info, flo);

    eg.generateEvent(EventType::Error, "Error: {Reason: BDYSH3}");
}
