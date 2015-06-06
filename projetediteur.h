#ifndef PROJETEDITEUR_H
#define PROJETEDITEUR_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QTreeWidget>
#include <QStringListModel>
#include <QStandardItem>
#include "projetsmanager.h"
#include "proprieteprojetediteur.h"
#include "tacheediteur.h"

class ProjetEditeur : public QWidget {
private:
    Q_OBJECT

    QGroupBox *grp_proprietes, *grp_modification, *grp_nouvelle;
    QPushButton *modifier_projet, *supprimer_projet, *modifier_tache, *supprimer_tache, *ajouter_unaire, *ajouter_composite, *programmer;
    QCheckBox *termine;
    QTreeWidget *taches;
    QGridLayout *main_layout;
    QVBoxLayout *l_existante, *l_nouvelle;
    QHBoxLayout *l_proprietes;
    QLabel *debut_label, *debut_date, *fin_label, *fin_date;

    QWidget* parent;
    TacheManager& tm;
    QString tache_courante;

    QGroupBox* initProprietes();
    void chargerTaches();
    void chargerSousTaches(QTreeWidgetItem* item, list<Tache *> *l, TacheComposite* tc );
public:
    ProjetEditeur(TacheManager& tm1, QWidget* p=0);
    const QString& getNom() const { return tm.getNom(); }

private slots:
    void modifierProjet();
    void supprimerProjet();
    void getTacheCourante(QTreeWidgetItem *item, int c);
    void ajouterTache();
    void modifierTache();
    void supprimerTache();
    void programmerTache();
    void refresh_taches();
    void refresh_projet();

signals:
    void fermeture(const QString);
};


#endif // PROJETEDITEUR_H
