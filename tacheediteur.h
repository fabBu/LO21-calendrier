#ifndef TACHEEDITEUR_H
#define TACHEEDITEUR_H

#include<QTextEdit>
#include<QLineEdit>
#include<QApplication>
#include<QPushButton>
#include<QLabel>
#include<QCheckBox>
#include<QDateEdit>
#include<QSpinBox>
#include<QHBoxLayout>
#include <QComboBox>
#include <QMessageBox>


#include "taches.h"
#include "tachemanager.h"


class TacheEditeur : public QWidget {
private:
    Q_OBJECT
    QLabel *lid, *ltitre, *ldispo, *lduree, *lecheance, *lpred, *lsoust, *laucun, *laucune;
    QLineEdit *id;
    QTextEdit *titre;
    QCheckBox *preemp;
    QDateEdit *dispo, *echeance;
    QSpinBox *h, *m;
    QHBoxLayout *lh1, *lh2, *lh3, *lh4, *lh5, *lh6;
    QVBoxLayout *lv;
    QPushButton *cancel, *save, *ajouterpred, *retirerpred, *ajoutersoust, *retirersoust;
    QComboBox *pred, *soust, *tachespred, *tachessoust;
    TacheManager& tm;
    Tache& t;

    QHBoxLayout* ligneTitre();
public:
    TacheEditeur(TacheManager& tm, Tache& t1, QWidget* parent=0);
    TacheEditeur(TacheManager &tm, bool unaire=true);
public slots:

private slots:
    void retirerPredecesseur();
    void ajouterPredecesseur();
    void modifListePred();

    void retirerSousTache();
    void ajouterSousTache();
    void modifierListeSoust();

    void sauvegarder();
};


#endif // TACHEEDITEUR_H
