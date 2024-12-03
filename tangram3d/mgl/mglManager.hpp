#ifndef MGL_MANAGER_HPP
#define MGL_MANAGER_HPP

#include <map>

namespace mgl {

template <typename T, typename U> class Manager;
template <typename T, typename U> class Manager {
    private:
        std::map<T, U> table;
    public:
        Manager();
        ~Manager();
        void put(T key, U obj);
        void remove(T key);
        U get(T key);
        bool contains(T key);
};

template <typename T, typename U> Manager<T,U>::Manager() {
        table = std::map<T, U>();
    }

    template <typename T, typename U> Manager<T,U>::~Manager() {
        table.clear();
    }

    template <typename T, typename U> void Manager<T,U>::put(T key, U obj) {
        table[key] = obj;
    }

    template <typename T, typename U> void Manager<T,U>::remove(T key) {
        table.erase(key);
    }

    template <typename T, typename U> U Manager<T,U>::get(T key) {
        return table[key];
    }

    template <typename T, typename U> bool Manager<T,U>::contains(T key) {
        return table.find(key) != table.end();
    }

}

#endif //MGL_MANAGER_HPP