#include <iostream>

class Singleton
{
    public:
        static Singleton& getRef()
        {
            static Singleton inst {};
            return inst;
        }

        ~Singleton() { std::cout << "~Singleton\n"; }

        void cmd() const { std::cout << "I am singleton with data: " << _data << "\n"; }

        void setData(int data) { _data = data; }

    private:
        Singleton() = default;
        Singleton(const Singleton&) = default;            // Delete maybe
        Singleton& operator=(const Singleton&) = default; // Delete maybe

        int _data { 42 };
};

class SingletonPtr
{
    public:
        // Can return shared ptr
        static SingletonPtr* getPtr()
        {
            if (!inst)
            {
                inst = new SingletonPtr();
            }
            return inst;
        }

        static void destroy() { delete inst; }

        ~SingletonPtr() { std::cout << "~SingletonPtr\n"; }

        void cmd() const { std::cout << "I am singleton with data: " << _data << "\n"; }

        void setData(int data) { _data = data; }

    private:
        static SingletonPtr* inst;
        SingletonPtr() = default;
        SingletonPtr(const SingletonPtr&) = default;            // Delete maybe
        SingletonPtr& operator=(const SingletonPtr&) = default; // Delete maybe

        int _data { 54 };
};
SingletonPtr* SingletonPtr::inst { nullptr };

int main()
{
    Singleton& s = Singleton::getRef();
    s.cmd();
    Singleton& s1 = Singleton::getRef();
    s1.cmd();

    s.setData(55);
    s.cmd();
    s1.cmd();

    const auto sPtr = SingletonPtr::getPtr();
    sPtr->cmd();
    SingletonPtr::destroy();
}
