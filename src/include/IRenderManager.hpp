#ifndef PUGGO_I_RENDER_MANAGER_HPP
#define PUGGO_I_RENDER_MANAGER_HPP

namespace puggo {
    class IRenderManager {
    public:
        virtual ~IRenderManager(void) {};
        virtual void renderAll(void) = 0;

    protected:
        IRenderManager(void) noexcept {};
        IRenderManager(const IRenderManager& manager) noexcept {};
        IRenderManager(IRenderManager&& manager) noexcept {};

        IRenderManager& operator=(const IRenderManager& manager) noexcept {};
        IRenderManager& operator=(IRenderManager&& manager) noexcept {};
    };
}

#endif // !PUGGO_MANAGER_HPP
