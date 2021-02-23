/*Auteur: PERCIE DU SERT Maxime
 *Date: 23/05/2020
 */
#ifndef MODELNOTE_H
#define MODELNOTE_H

#include "BddNote.h"
#include "table_model.h"
#include "TableauNote.h"

namespace noteMPS {
/*! \ingroup groupe_modelNote
 * \brief Model de la liste des élèves à affecter dans un groupe.
 */
class CandidatGroupeModel : public model_base::table_model {
    Q_OBJECT
protected:
    abstract_colonnes_model::new_colonne_info m_groupeInfo;     //! Information sur la colone de groupe
public:
    //! position des colonnes du model des éléves affectés à la classe.
    enum colonneClasseEleve {NomColonne, PrenomColonne, GroupeColonne, NbrColonne};
    enum {Num_Role = CandidatGroupeTableau::Num_Role};

    //! Constructeur.
    CandidatGroupeModel(BddNote & bdd, idt id_groupe, QObject * parent = nullptr);

    //! Retire des élèves des groupes.
    void remove(std::vector<std::pair<idt,int>> && vecIdNum) {
        static_cast<CandidatGroupeTableau&>(*m_data).remove(std::move(vecIdNum));
        dataChanged(index(0,GroupeColonne),index(row_count()-1,GroupeColonne));
    }
};

/*! \ingroup groupe_modelNote
 * \brief Model des élèves d'une classe.
 */
class ClasseEleveModel : public model_base::table_model {
    Q_OBJECT
public:
    //! position des colonnes du model des éléves affectés à la classe.
    enum colonneClasseEleve {NomColonne, PrenomColonne, NaissanceColonne, SexeColonne, EntreeColonne, SortieColonne, NbrColonne};

    //! Constructeur.
    ClasseEleveModel(BddNote & bdd, idt idClasse, QObject * parent = nullptr);

    //! Ajoute un élève à la classe à partir de son identifiant.
    void add(idt idEleve);
public slots:
    //! Mutateur de l'identifiant de la classe.
    void set_idClasse(idt idClasse){
        beginResetModel();
            static_cast<ClasseElevecomposition_tableau&>(*m_data).set_idClasse(idClasse);
            reset_row_to_ligne();
        end_reset_model();
    }
};

/*! \ingroup groupe_modelNote
 * \brief Model des éléves par groupe.
 */
class Eleve_groupeModel : public model_base::table_model {
    Q_OBJECT
public:
    //! Constructeur.
    Eleve_groupeModel(BddNote & bdd, QObject * parent = nullptr)
        : table_model(false,false,parent) {
        set_tableau(std::make_unique<Eleve_groupeTableau>(bdd));
    }

    //! Accesseur du groupe du tableau.
    const Groupe & groupe() const
        {return static_cast<Eleve_groupeTableau&>(*m_data).groupe();}

    //! Mes à jour les éléves d'un groupe.
    void updateEleve(const std::list<idt> & listEl, szt num, const std::map<szt,std::forward_list<idt>> & mapDel);

    //! Retire des élèves des groupes.
    std::vector<std::pair<idt, int> > remove(const QModelIndexList & selection);

public slots:
    //! Ajoute un groupe au model.
    void push_back();

    //! Mutateur de l'identifiant de la classe.
    void set_id_groupe(idt id_groupe);
};
}
#endif // MODELNOTE_H
