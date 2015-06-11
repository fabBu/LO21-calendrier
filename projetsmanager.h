#ifndef PROJETSMANAGER_H
#define PROJETSMANAGER_H

#include <QString>
#include <QDate>
#include "xmlparsor.h"
#include "singleton.h"
#include "tachemanager.h"

class ProjetsManager
{
    std::list<TacheManager*> projets; /*!< Ensemble des projets chargés */
    /*!
     * \brief Recherche d'un projet depuis un nom
     * \param nom Nom du projet à chercher
     * \return 0 Si aucun projet n'est trouvé
     */
    TacheManager* trouverProjet(const QString& nom);

public:
    /*!
     * \brief Ajout d'un nouveau projet
     * \param nom Nom du projet
     * \param debut Date de début du projet à partir de laquelle les tâches peuvent commencer
     * \param fin Date de fin du projet à partir de laquelle les tâches doivent toutes être terminées
     * \throw <CalendarException> { Le nom est déjà pris par un projet existant }
     */
    void ajouterProjet(const QString& nom, const QDate& debut, const QDate& fin, const QColor &coul=Qt::white);
    /*!
     * \brief Suppression d'un projet
     * \param nom Nom du projet à supprimer
     * \throw <CalendarException> { Aucun projet portant ce nom n'existe }
     */
    void retirerProjet(const QString& nom);
    /*!
     * \brief Donne une référence sur un projet existant dans \projets
     * \param nom Nom du projet
     * \return Référence sur le projet
     * \throw <CalendarException> { Aucun projet portant ce nom n'existe }
     */
    TacheManager& getProjet(const QString& nom);
    /*!
     * \brief Donne une référence constante sur un projet existant dans \projets
     * \param nom Nom du projet
     * \return Référence constante sur le projet
     * \throw <CalendarException> { Aucun projet portant ce nom n'existe }
     */
    const TacheManager& getProjet(const QString& nom) const;
    const std::list<TacheManager*> getProjets() const {return projets;} /*!< Getter constant sur \a projets */
    /*!
     * \brief Modifier le nom d'un projet
     * \param nom Nom du projet à modifier
     * \param nouveau Nouveau nom à donner au projet
     * \throw <CalendarException> { Si aucun projet 'nom' n'existe ou si 'nouveau' est déjà utilisé par un autre projet }
     */
    void setNom(const QString& nom, const QString& nouveau);
    /*!
     * \brief Modifier la date de début d'un projet
     * \param nom Nom du projet à modifier
     * \param debut Nouvelle date de début à attribuer au projets
     * \throw <CalendarException> { Aucun projet portant ce nom n'existe }
     */
    void setDebut(const QString& nom, const QDate& debut);
    /*!
     * \brief Modifier la date de fin d'un projet
     * \param nom Nom du projet à modifier
     * \param debut Nouvelle date de fin à attribuer au projets
     * \throw <CalendarException> { Aucun projet portant ce nom n'existe }
     */
    void setFin(const QString& nom, const QDate& fin);
    /*!
     * \brief Modification des dates de début et fin de projet
     *
     * Utilise setDebut() et setFin()
     *
     * \param nom Nom du projet à modifier
     * \param debut Nouvelle date de début
     * \param fin Nouvelle date de find
     * \throw <CalendarException> { Incohérence des dates }
     */
    void setDates(const QString& nom, const QDate& debut, const QDate& fin);

    /*!
     * \brief Getter sur le singleton ProjetsManager
     * \return Référence sur l'unique ProjetsManager
     */
    static ProjetsManager& getInstance();
    /*!
     * \brief Détruire le singleton ProjetsManager
     */
    static void libererInstance();

    /*!
     * \brief Export en XML de l'ensemble des projets et leur tâches
     *
     * Crée un fichier pour chacun des projets chargés dans l'outil et le remplit via le XML généré par ProjetToXML() de TacheManager
     *
     * \param dossier Dossier dans lequel créer/remplacer les fichiers
     */
    void writeXML(const QString &dossier);
    /*!
     * \brief Import depuis fichiers XML de projets et leurs tâches
     *
     * Pour un fichier donné, charge l'ensemble des tâches puis les contraintes reliant ces tâches
     *
     * \param file Fichier contenant le code XML d'un projet
     */
    void readXML(QFile &file);
private:
    /*!
     * \brief Attribut permettant l'implémentation du pattern Singleton
     */
    static Handler<ProjetsManager> handler;
    friend Handler<ProjetsManager>::~Handler();

    ProjetsManager(){}
    ~ProjetsManager() {}
    ProjetsManager(const ProjetsManager& p);
    ProjetsManager& operator=(const ProjetsManager& p);
};

#endif // PROJETSMANAGER_H
