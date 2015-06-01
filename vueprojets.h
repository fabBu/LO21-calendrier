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

#include "projetediteur.h"

class VueProjets : public QWidget
{
private:
    Q_OBJECT

    QWidget* parent;

    QGroupBox *titre;
    QPushButton *supprimer, *ajouter;
    QGridLayout *main_layout;
    QHBoxLayout *l_projet;
    QComboBox *choix_projet;

    std::list<TacheManager*> *projets;

    ProjetEditeur *pe;

    void initListe();
public:
    VueProjets(std::list<TacheManager*> *p, QWidget* parent=0);

    void supprimerProjet()
    {
          QMessageBox::StandardButton reply;
          reply = QMessageBox::question(this, "Test", "Quit?",
                                        QMessageBox::Yes|QMessageBox::No);
          if (reply == QMessageBox::Yes) {
            qDebug() << "Yes was clicked";
            QApplication::quit();
          } else {
            qDebug() << "Yes was *not* clicked";
          }
    }

private slots:
    void chargerProjet(QString nom);
    void ajouterProjet();
};

#endif // VUEPROJETS_H
