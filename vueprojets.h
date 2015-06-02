#ifndef VUEPROJETS_H
#define VUEPROJETS_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QComboBox>
#include <QInputDialog>

#include "proprieteprojetediteur.h"
#include "projetsmanager.h"
#include "projetediteur.h"

class VueProjets : public QWidget
{
private:
    Q_OBJECT

    QWidget* parent;

    QGroupBox *titre;
    QPushButton *modifier, *supprimer, *ajouter;
    QGridLayout *main_layout;
    QHBoxLayout *l_projet;
    QComboBox *choix_projet;

    ProjetsManager* projets;

    ProjetEditeur *pe;

    QComboBox* initListe();
public:
    VueProjets(ProjetsManager *p, QWidget* parent=0);

private slots:
    void chargerProjet(QString nom);
    void modifierProjet();
    void supprimerProjet();
    void ajouterProjet();
    void refresh(const QString& proj);
};

#endif // VUEPROJETS_H
