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
    QLabel *titre_label, *desc_label, *dispo_label, *duree_label, *echeance_label, *pred_label, *soust_label;
    QLineEdit *titre;
    QTextEdit *desc;
    QCheckBox *preemp;
    QDateEdit *dispo, *echeance;
    QSpinBox *duree_h, *duree_m;
    QHBoxLayout *l_titre, *l_desc, *l_dates, *l_pred, *l_soust, *l_cancelsave;
    QVBoxLayout *main_layout;
    QPushButton *btn_cancel, *btn_save, *btn_ajouterpred, *btn_retirerpred, *btn_ajoutersoust, *btn_retirersoust;
    QComboBox *pred_list, *soust_list, *nonpred_list, *nonsoust_list;
    TacheManager& tm;
    Tache* t;

    void initTitre(bool unaire=true);
    void initDesc();
    void initDates(bool unaire=true);
    void initCancelSave();
    void initPrecedence();
    void initSousTaches();
public:
    TacheEditeur(TacheManager& tm, Tache* t1, QWidget* parent=0);
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
