#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWidget>
#include <QTabWidget>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QString>
#include "projetediteur.h"
#include "programmationmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTabWidget *onglets;

    QMenuBar *menubar;
    QMenu *menu_fichier, *menu_projets;
    QAction *sauvegarder, *editerProjet;

    ProjetsManager& projets;
    ProgrammationManager& agenda;

    void initMenuBar();

    void chargerProjets();
public:
    MainWindow();

private slots:
    void creerProjet();
    void ouvrirProjet();
    void closeTab(int index);
};

#endif // MAINWINDOW_H
