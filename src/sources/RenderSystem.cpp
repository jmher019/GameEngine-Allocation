#include <RenderSystem.hpp>

using namespace puggo;

RenderSystem::RenderSystem(void):
    ISystem() {
}

RenderSystem::~RenderSystem(void) {
    // Nothing needs to be called here for now
}

void RenderSystem::initialze(void) {
    system.init();
}

void RenderSystem::cleanUpSystem(void) {
    system.cleanUp();
}

void RenderSystem::init(void) {
    // Nothing needs to be initialized for now
}

void RenderSystem::cleanUp(void) {
    managers.clear();
}

void RenderSystem::renderAllManagers(void) {
    for (size_t i = 0; i < system.managers.size(); ++i) {
        system.managers[i]->renderAll();
    }
}

template <typename T>
size_t RenderSystem::registerManager(void) {
    for (size_t i = 0; i < system.managers.size(); ++i) {
        if (dynamic_cast<RenderManager<T>*>(system.managers[i].get()) != nullptr) {
            return i;
        }
    }

    managers.push_back(make_unique<RenderManager<T>>());
    return managers.size() - 1;
}

template <typename T>
void RenderSystem::addObject(const T& renderableObject) {
    RenderManager<T>* manager = dynamic_cast<RenderManager<T>*>(system.managers[T::managerIndex]);
    manager->addObject(renderableObject);
}

template <typename T>
void RenderSystem::removeObject(const T& renderableObject) {
    RenderManager<T>* manager = dynamic_cast<RenderManager<T>*>(system.managers[T::managerIndex]);
    manager->removeObject(renderableObject);
}