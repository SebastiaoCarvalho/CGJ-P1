#include "./mglKeyManager.hpp"

namespace mgl {
    KeyManager::KeyManager() : Manager<int, bool>() {}

    KeyManager::~KeyManager() {}

    void KeyManager::press(int key) {
        put(key, true);
    }

    void KeyManager::release(int key) {
        put(key, false);
    }

    bool KeyManager::isPressed(int key) {
        return contains(key) && get(key);
    }
}