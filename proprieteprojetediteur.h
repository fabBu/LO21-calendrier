#ifndef PROPRIETEPROJETEDITEUR_H
#define PROPRIETEPROJETEDITEUR_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include "projetsmanager.h"

class ProprieteProjetEditeur : public QWidget
{
    Q_OBJECT

    QWidget *parent;
    QGridLayout *main_layout;
    QLabel *nom_label, *debut_label, *fin_label;
    QLineEdit *nom_edit;
    QDateEdit *debut_date, *fin_date;
    QPushButton *sauvegarder, *annuler;

    ProjetsManager* projets;
    TacheManager* projet;
public:
    ProprieteProjetEditeur(QWidget* pa, ProjetsManager* p, const QString& proj="");
private slots:
    void creerProjet();
    void modifierProjet();
    void refresh();
signals:
    void fermeture(const QString& proj);
};

#endif // PROPRIETEPROJETEDITEUR_H
