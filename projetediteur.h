#ifndef PROJETEDITEUR_H
#define PROJETEDITEUR_H

#include <QWidget>
#include <QGroupBox>
#include <QTreeWidget>
#include <QStringListModel>
#include <QStandardItem>
#include "tachemanager.h"
#include "tacheediteur.h"

class ProjetEditeur : public QWidget {
private:
    Q_OBJECT

    QGroupBox *grp_existante, *grp_nouvelle;
    QPushButton *modifier, *supprimer, *ajouter_unaire, *ajouter_composite, *programmer;
    QTreeWidget *taches;
    QGridLayout *main_layout;
    QVBoxLayout *l_existante, *l_nouvelle;

    QWidget* parent;
    TacheManager& tm;
    QString tache_courante;
    TacheEditeur* te;

    void chargerTaches();
    void chargerSousTaches(QTreeWidgetItem* item, TacheComposite* tc );
public:
    ProjetEditeur(TacheManager& tm1, QWidget* p=0);
public slots:
private slots:
    void getTacheCourante(QTreeWidgetItem *item, int c);
    void ajouterTache();
    void modifierTache();
    void supprimerTache();
    void programmerTache();
    void refresh();
};


#endif // PROJETEDITEUR_H
