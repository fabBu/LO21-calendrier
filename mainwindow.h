#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

<<<<<<< HEAD
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
=======
    QTabWidget *onglets;

    QMenuBar *menubar;
    QMenu *menu_fichier, *menu_projets;
    QAction *sauvegarder, *editerProjet;

    ProjetsManager& projets;
    ProgrammationManager& agenda;

    std::list<ProjetEditeur*> projets_ouverts;
    //AgendaEditeur* agenda_ouvert;

    void initMenuBar();
    void chargerProjets();

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
};

#endif // MAINWINDOW_H
