using namespace puggo;

template<typename T>
RenderManager<T>::RenderManager(void):
    IRenderManager() {
}

template<typename T>
RenderManager<T>::~RenderManager(void) {
    renderableObjects.clear();
}

template <typename T>
void RenderManager<T>::renderAll(void) {
    for_each(renderableObjects.begin(), renderableObjects.end(), [](T& object) { object.render(); });
}

template <typename T>
void RenderManager<T>::addObject(const T& renderableObject) {
    renderableObjects.push_back(renderableObject);
}

template <typename T>
void RenderManager<T>::removeObject(const T& renderableObject) {
    renderableObjects.erase(renderableObjects.find(renderableObject));
}