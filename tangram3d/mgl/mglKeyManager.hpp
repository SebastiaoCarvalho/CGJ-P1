#ifndef MGL_KEY_MANAGER_HPP
#define MGL_KEY_MANAGER_HPP

#include "./mglManager.hpp"

namespace mgl {

class KeyManager : public Manager<int, bool> {
    public:
        KeyManager();
        ~KeyManager();
        void press(int key);
        void release(int key);
        bool isPressed(int key);
};
}

#endif //MGL_KEY_MANAGER_HPP