#ifndef PROJETEDITEUR_H
#define PROJETEDITEUR_H

#include <QWidget>
#include <QTreeWidget>
#include <QStringListModel>
#include <QStandardItem>
#include "tachemanager.h"
#include "tacheediteur.h"

class ProjetEditeur : public QWidget {
private:
    Q_OBJECT

    QPushButton *modifier, *supprimer, *ajouter_unaire, *ajouter_composite;
    QTreeWidget *taches;
    QGridLayout *main_layout;

    QWidget* parent;
    TacheManager& tm;
    QString tache_courante;
    TacheEditeur* te;

    void chargerTaches();
public:
    ProjetEditeur(TacheManager& tm1, QWidget* p=0);
public slots:
private slots:
    void getTacheCourante(QTreeWidgetItem *item, int c);
    void ajouterTache();
    void modifierTache();
    void supprimerTache();
    void refresh();
};


#endif // PROJETEDITEUR_H
