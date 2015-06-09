#ifndef SINGLETON_H
#define SINGLETON_H


template <class T>
struct Handler
{
    T* instance;
    friend class T;

    Handler(): instance(0) {}
    ~Handler() {if (instance) delete instance;}
};

#endif // SINGLETON_H
