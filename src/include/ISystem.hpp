#ifndef PUGGO_I_SYSTEM_HPP
#define PUGGO_I_SYSTEM_HPP

namespace puggo {
    class ISystem {
    public:
        ISystem(const ISystem&) = delete;
        ISystem(ISystem&&) = delete;
        ~ISystem(void) {};
        ISystem& operator=(const ISystem&) = delete;
        ISystem& operator=(ISystem&&) = delete;

        virtual void init(void) = 0;
        virtual void cleanUp(void) = 0;

    protected:
        ISystem(void) {};
    };
}

#endif // !PUGGO_I_SYSTEM_HPP
