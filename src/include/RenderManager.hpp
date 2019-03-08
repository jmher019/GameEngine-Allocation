#ifndef PUGGO_RENDER_MANAGER_HPP
#define PUGGO_RENDER_MANAGER_HPP

#include <algorithm>
#include <vector>

#include <IRenderManager.hpp>

using namespace std;

namespace puggo {
    template<typename T>
    class RenderManager : public IRenderManager{
    public:
        RenderManager(void);
        RenderManager(const RenderManager<T>& manager) = delete;
        RenderManager(RenderManager<T>&& manager) = delete;
        ~RenderManager(void);

        RenderManager<T>& operator=(const RenderManager<T>& manager) = delete;
        RenderManager<T>& operator=(RenderManager<T>&& manager) = delete;

        void renderAll(void) override;
        void addObject(const T& renderableObject);
        void removeObject(const T& renderableObject);

    private:
        vector<T> renderableObjects;
    };
}

#include <RenderManager.inl>

#endif // !PUGGO_RENDER_MANAGER_HPP
