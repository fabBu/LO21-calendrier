#ifndef PROGRMMATIONMANAGER_H
#define PROGRMMATIONMANAGER_H

#include <iostream>
#include <list>
#include "singleton.h"
#include "taches.h"
#include "programmation.h"
#include "calendar.h"

using namespace std;

/*!
 * \class ProgrammationManager
 * \brief Classe représentant le gestionnaire de programmations
 * Cette classe utilise le pattern singleton
 */
class ProgrammationManager
{
public:
    /*!
     * \brief Retourne la taille de la liste de programmations
     * \return taille
     */
    int getSize() {return programmations.size();}
    /*!
     * \brief Méthode de test pour vérifier que l'on peut programmer un événement dans un intervalle de temps donné
     * \param d : date et horaire
     * \param h : durée
     * \param pr : programmation
     * \return booleen
     */
    bool isFree(const QDateTime& d, const Duree& h, const Programmation* pr = 0) const;

    /*!
     * \brief Méthode pour ajouter une programmation à la liste
     * \param da : date et horaire
     * \param du : durée
     * \param e : événement
     */
    void addProgrammation(const QDateTime& da, const Duree& du, Evenement *e);
    /*!
     * \brief Méthode de suppression d'une programmation à partir d'une date et un horaire
     * \throw <CalendarException>["On ne peut pas programmer d'une durée supérieur à 12."];
     * \param d : jourdate et horaire
     */
    void removeProgrammation(const QDateTime& d);
    /*!
     * \brief Méthode de suppression d'une programmation à partir d'une programmation
     * \throw <CalendarException>["Il faut que la programmation existe."];
     * \param da : date et horaire
     */
    void removeProgrammation(Programmation *pr);
    /*!
     * \brief Méthode de suppression d'une programmation à partir d'une tache unaire
     * \param t : tache unaire
     */
    void removeProgrammation(const TacheUnaire* t);
    /*!
     * \brief Méthode de suppression des successeurs d'une tache
     * \param t : tache unaire
     */
    void removeProgrammationSuccesseur(const TacheUnaire* t);
    /*!
     * \brief Méthode test pour savoir si le successeur est programmé
     * \param t : tache unaire
     */
    bool isSuccesseurProgramme(const TacheUnaire *t);

    /*!
     * \brief Méthode pour récupérer toutes les programmations dans un intervalle de temps donné
     * \param debut : date de début de l'intervalle
     * \param fin : date de fin de l'intervalle
     * \return liste de pointeur de programmation
     */
    const list<Programmation*> getProgrammation(const QDate& debut, const QDate& fin)const;
    /*!
     * \brief Méthode pour récupérer toutes les programmations d'une tache (préemptive)
     * \param t : tache unaire
     * \return liste de pointeur de programmation
     */
    const list<Programmation*> getProgrammation(const TacheUnaire *t)const;
    /*!
     * \brief Méthode pour récupérer une programmation à partir d'une date et d'un horaire
     * \param d : date et horaire
     * \return programmation
     */
    Programmation& getProgrammation(const QDateTime& d);
    /*!
     * \brief Méthode pour récupérer une programmation à partir d'une date et d'un horaire avec un objet constant
     * \param d : date et horaire
     * \throw <CalendarException>["Il faut que la programmation existe."];
     * \return programmation constant
     */
    const Programmation& getProgrammation(const QDateTime& d)const;
private:
    static Handler<ProgrammationManager> handler; /*!< handler pour faire un singleton de programmation manager */
    friend Handler<ProgrammationManager>::~Handler();

    list<Programmation*> programmations; /*!< Liste de programmations */
    /*!
     * \brief Méthode pour ajouter une programmation à la liste
     * \param p : programmation
     */
    void addItem(Programmation* p);
    /*!
     * \brief Méthode pour récupérer un pointeur sur programmation à partir d'une date et un horaire
     * \param d : date et horaire
     * \throw <CalendarException>["On ne peut pas programmer : s'il y a déjà une programmation à cette date,
     * si la totalité de la tache est programmée, si la tache est préemptive et qu'elle n'est pas effecutée totalement
     * et si la durée de la programmation est supérieur à la durée restante de la tache."];
     * \return pointeur de programmation
     */
    Programmation* findProgrammation(const QDateTime& d) const;

    /*!
     * \brief Constructeur de ProgrammationManager
     */
    ProgrammationManager(){}
    /*!
     * \brief Destructeur de Programmation Manager
     */
    ~ProgrammationManager() {}

    /* On ne souhaite pas que l'utilisateur de la classe puisse utiliser l'opérateur d'affectation ainsi le constructeur par recopie d'un programmation manager */
    /*!
     * \brief Constructeur par recopie de ProgrammationManager
     * \param p : Programmation Manager
     */
    ProgrammationManager(const ProgrammationManager& p);
    /*!
     * \brief Opérateur d'affectation du programmation manager
     * \param p : Programmation Manager
     */
    ProgrammationManager& operator=(const ProgrammationManager& p);

public:
    /*!
     * \brief Méthode qui permet de récupérer l'instance de programmation manager
     * \param p : Programmation Manager
     */
    static ProgrammationManager& getInstance();
    /*!
     * \brief Méthode qui permet de libérer/supprimer l'instance de programmation manager
     */
    static void libererInstance();

    /*!
     * \brief Méthode qui permet de stocker l'objet programmation manager dans du XML
     * \param dossier
     */
    void writeXML(const QString &dossier);
    /*!
     * \brief Méthode qui permet de lire l'objet programmation manager à partir du XML
     * \param file
     */
    void readXML(QFile &file);
};

#endif // PROGRMMATIONMANAGER_H
