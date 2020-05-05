/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/04/2020
 */
#ifndef ELEVEMODEL_H
#define ELEVEMODEL_H

//#include <QRegularExpression>
//#include "AbstractEntityTableau.h"
//#include "BddNote.h"
//#include "EntityNote.h"

//namespace  noteMPS{
///*! \defgroup groupeModelNote Dialogues de note
// * \brief Ensemble des models de l'application note.
// */

///*! \ingroup groupeModelNote
// * \brief Model abstrait de donnée principale les éléves.
// */
//class AbstractEleveModel : public modelMPS::AbstractTableEntityModel {
//    Q_OBJECT
//protected:
//    using Column = modelMPS::OnColumn<Eleve>;
//public:
//    enum ColumnId {Nom,
//                Prenom,
//                Naissance,
//                 Sexe};
//    //! Constructeur.
//    AbstractEleveModel(BddNote & bdd, QObject * parent = nullptr)
//        : AbstractTableEntityModel(bdd,parent) {}

////    //! Ajoute les colonnes nom et prénom.
////    void addNomPrenom(Tableau::iterator iter);

////    //! Ajoute les colonnes date de naissance et sexe.
////    void addNaissanceSexe(Tableau::iterator iter);

//protected:
//    //! Lien avec la base de données.
//    BddNote & bdd() const override
//        {return static_cast<BddNote &>(m_bdd);}
//};

///*! \ingroup groupeModelNote
// * \brief Model réduit aux données de l'entité éléve en écriture et lecture.
// */
//class EleveModel : public AbstractEleveModel {
//    Q_OBJECT
//public:
//    struct FindEleves {
//        QRegularExpression nom;
//        QRegularExpression prenom;
//        QDate apres;
//        QDate avant;
//        bool femme;
//        bool homme;
//    };
//protected:
//    FindEleves m_findEleves;        //!< Précédente recherche.
//public:
////    enum ColumnId {Nom,
////                 Prenom,
////                 Naissance,
////                 Sexe};
//    //! Constructeur.
//    EleveModel(BddNote & bdd, QObject * parent = nullptr)
//        : AbstractEleveModel(/*bdd.getList<Eleve>(),*/bdd,parent) {
//        auto iterEl = push_backData(bdd.getList<Eleve>());
//        addNomPrenom(iterEl);
//        addNaissanceSexe(iterEl);
//        for (auto iter =m_columns.begin();iter != std::prev(m_columns.end()); ++iter)
//            static_cast<Column &>(**iter).setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable);
//        static_cast<Column &>(*m_columns[Sexe])
//                .setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable);
//    }


//    //! Cherche les élèves vérifiant findEleves.
//    void find(const FindEleves & fe);

//    //! Teste si la ligne est nouvelle ou modifiée.
//    bool newOrModifRow(szt id) const override
//        {return m_data.front()[id].isNew() || !bdd().sameInBdd(m_data.front()[id]);}

//    //! Réinitialise l'affichage des lignes du model (pas les donnée).
//    void resetRow(bool init = false) override {
//        AbstractEleveModel::resetRow(init);
//        if(!init)
//            sort(EleveModel::Nom);
//    }

//    //! Enregistre les données de la ligne id du model dans la base de donnée.
//    void saveRow(szt id) override
//        {bdd().save(m_data.front()[id]);}

//    //! Teste si la ligne de donnée d'identifiant id est valide.
//    bool valideRow(szt id) const override
//        {return m_data.front()[id].isValid();}

//protected:
//    //! Renvoie un pointeur sur un éléve.
//    entityMPS::Entity * entityFactory(const Indice & /*ind*/) override
//        {return new Eleve;}

//    //! Supprime l'éléve correspondant à la ligne id dans la base de donnée.
//    void removeInternalData(szt id) override
//        {bdd().del(m_data.front()[id]);}
//};
/*
class ClasseEleveModel : public AbstractEleveModel
{
    Q_OBJECT
public:
    enum Column {Nom,
                 Prenom,
                 Naissance,
                 Sexe};
    //! Constructeur.
    ClasseEleveModel(conteneurMPS::VectorPtr<Eleve> && vecEleve, BddNote & bdd, QObject * parent = nullptr)
        : AbstractEleveModel(std::move(vecEleve),bdd,parent)
        {addNaissanceSexe(m_data.begin());}
};*/
//}
#endif // ELEVEMODEL_H
