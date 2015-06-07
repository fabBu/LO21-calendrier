#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QWidget>
#include <QTabWidget>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QString>
#include "projetediteur.h"
#include "programmationmanager.h"
#include "agendaediteur.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTabWidget *onglets;

    QMenuBar *menubar;
    QMenu *menu_fichier, *menu_projets;
    QAction *sauvegarder, *editerProjet;

    ProjetsManager& projets;
    ProgrammationManager& agenda;

    std::list<ProjetEditeur*> projets_ouverts;
    AgendaEditeur* agenda_ouvert;

    void initMenuBar();
    void chargerProjets(const QString &dossier);

    ProjetEditeur* getProjetEdit(const QString nom);
    bool estOuvert(const QString nom) { return getProjetEdit(nom)!=0;  }
public:
    MainWindow();

private slots:
    void ouvrirAgenda();
    void creerProjet();
    void ouvrirProjet();
    void fermerProjet(const QString nom);
    void closeTab(int index);

protected:
     void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
