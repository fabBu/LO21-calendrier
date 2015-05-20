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
#include "tacheediteur.h"
#include "tachemanager.h"
#include "taches.h"

class TacheEditeur : public QWidget {
private:
    Q_OBJECT
    QLabel *lid, *ltitre, *ldispo, *lduree, *lecheance, *lpred, *lsoust;
    QLineEdit *id;
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
    TacheEditeur(Tache& t1, QWidget* parent=0);
public slots:
private slots:
};


#endif // TACHEEDITEUR_H
