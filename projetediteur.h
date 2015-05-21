#ifndef PROJETEDITEUR_H
#define PROJETEDITEUR_H

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
#include "tacheediteur.h"
#include "tachemanager.h"
#include "taches.h"

class ProjetEditeur : public QWidget {
private:
    Q_OBJECT
    QLabel *lid, *ltitre, *llieu, *ldispo, *lduree, *lecheance, *lpred, *lsoust;
    QLineEdit *id, *lieu;
    QTextEdit *titre;
    QCheckBox *preemp;
    QDateEdit *dispo, *echeance;
    QSpinBox *h, *m;
    QHBoxLayout *lh1, *lh2, *lh3, *lh4, *lh5, *lh6;
    QVBoxLayout *lv;
    QPushButton *cancel, *save;
    QComboBox *pred, *soust;
    Tache& t;
public:
    ProjetEditeur(TacheManager& tm, QWidget* parent=0);
public slots:
private slots:
};



#endif // PROJETEDITEUR_H
