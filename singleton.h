#ifndef SINGLETON_H
#define SINGLETON_H


template <class T>
struct Handler
{
    T* instance;

    Handler(): instance(0) {}
    ~Handler() {if (instance) delete instance;}
};

#endif // SINGLETON_H
