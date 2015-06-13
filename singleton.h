#ifndef SINGLETON_H
#define SINGLETON_H

/*!
 * \struct Handler
 * \brief Classe permet l'implémentation du pattern singleton
 * \tparam T : la classe à laquelle on applique le singleton
 */
template <class T>
struct Handler
{
    T* instance; /*!< Instance de singleton*/

    /*!
     * \brief Constructeur de la classe Handler
     */
    Handler(): instance(0) {}
    /*!
     * \brief Destructeur de la classe Handler
     */
    ~Handler() {if (instance) delete instance;}
};

#endif // SINGLETON_H
