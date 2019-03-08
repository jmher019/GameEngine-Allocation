#ifndef PUGGO_RENDER_SYSTEM_HPP
#define PUGGO_RENDER_SYSTEM_HPP

#include <vector>
#include <memory>
#include <algorithm>

#include <ISystem.hpp>
#include <RenderManager.hpp>

using namespace std;

namespace puggo {
    class RenderSystem : public ISystem {
    public:
        RenderSystem(const RenderSystem&) = delete;
        RenderSystem(RenderSystem&&) = delete;
        ~RenderSystem(void);

        RenderSystem& operator=(const RenderSystem&) = delete;
        RenderSystem& operator=(RenderSystem&&) = delete;

        static void initialze(void);
        static void cleanUpSystem(void);
        static void renderAllManagers(void);
        
        template <typename T>
        static size_t registerManager(void);
        template <typename T>
        static void addObject(const T& renderableObject);
        template <typename T>
        static void removeObject(const T& renderableObject);

        void init(void) override;
        void cleanUp(void) override;

    private:
        RenderSystem(void);

        vector<unique_ptr<IRenderManager>> managers;

        static RenderSystem system;
    };

#define REGISTER_RENDERABLE_TYPE(type) RenderSystem::registerManager<type>();
}

#endif // !PUGGO_RENDERER_SYSTEM_HPP
