#include "proprieteprojetediteur.h"

ProprieteProjetEditeur::ProprieteProjetEditeur(QWidget *pa, ProjetsManager *p, const QString& proj):parent(pa), projets(p)
{
    projet=0;
    if( proj != "") projet=&p->getProjet(proj);

    setWindowTitle("Propriétés du projet");

    main_layout = new QGridLayout(this);

    /// ---- NOM DU PROJET ---- ///
    nom_label = new QLabel("Nom: ");
    nom_edit = new QLineEdit(this);
    if( projet )
        nom_edit->setText(projet->getNom());
    main_layout->addWidget(nom_label,0,0);
    main_layout->addWidget(nom_edit, 0,1);

    /// ---- DEBUT DU PROJET ---- ///
    debut_label= new QLabel("Début: ");
    debut_date = new QDateEdit(this);
    if(projet)
        debut_date->setDate(projet->getDebut());
    else
        debut_date->setDate(QDate::currentDate());
    main_layout->addWidget(debut_label, 1,0);
    main_layout->addWidget(debut_date, 1,1);

    /// ---- FIN DU PROJET ---- ///
    fin_label= new QLabel("Fin: ");
    fin_date = new QDateEdit(this);
    if(projet)
        fin_date->setDate(projet->getFin());
    else
        fin_date->setDate(QDate::currentDate());
    main_layout->addWidget(fin_label, 2,0);
    main_layout->addWidget(fin_date, 2,1);

    /// ---- BOUTONS ---- ///
    sauvegarder = new QPushButton("OK");
    annuler = new QPushButton("Annuler");
    main_layout->addWidget(sauvegarder, 3,0);
    main_layout->addWidget(annuler, 3,1);

    if(projet) connect(sauvegarder, SIGNAL(clicked(bool)), this, SLOT(modifierProjet()));
    else        connect(sauvegarder, SIGNAL(clicked(bool)), this, SLOT(creerProjet()));
    connect(annuler, SIGNAL(clicked(bool)), this, SLOT(refresh()));

}

void ProprieteProjetEditeur::creerProjet()
{
    try
    {
        projets->ajouterProjet(nom_edit->text(), debut_date->date(), fin_date->date());
        emit fermeture(nom_edit->text());
        close();
    }
    catch(CalendarException e)
    { QMessageBox::warning(this, "Création de projet", e.getInfo()); }
}

void ProprieteProjetEditeur::modifierProjet()
{
    QString nom=projet->getNom();
    try
    {
        if( projet->getNom().toStdString() != nom_edit->text().toStdString() )
        {
            projets->setNom(projet->getNom(), nom_edit->text());
            nom=nom_edit->text();
        }
        if( projet->getDebut() != debut_date->date())
            projets->setDebut(projet->getNom(), debut_date->date());
        if( projet->getFin() != fin_date->date() )
            projets->setFin(projet->getNom(), fin_date->date());

        emit fermeture(nom);
        close();
    }
    catch(CalendarException e)
    { QMessageBox::warning(this, "Création de projet", e.getInfo()); }
}

void ProprieteProjetEditeur::refresh()
{
    if(projet)
    {
        nom_edit->setText(projet->getNom());
        debut_date->setDate(projet->getDebut());
        fin_date->setDate(projet->getFin());
    }
    else
    {
        nom_edit->setText("");
        debut_date->setDate(QDate::currentDate());
        fin_date->setDate(QDate::currentDate());
    }

}
