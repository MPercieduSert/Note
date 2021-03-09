/*Auteur: PERCIE DU SERT Maxime
 *Date: 23/05/2020
 */
#ifndef MODEL_NOTE_H
#define MODEL_NOTE_H

#include "bdd_note.h"
#include "table_model.h"
#include "tableau_note.h"

namespace note_mps {
/*! \ingroup groupe_model_note
 * \brief Model de la liste des élèves à affecter dans un groupe.
 */
class candidat_groupe_model : public mps::model_base::table_model {
    Q_OBJECT
protected:
    abstract_colonnes_model::new_colonne_info m_groupe_info;     //! Information sur la colone de groupe
public:
    //! position des colonnes du model des éléves affectés à la classe.
    enum colonne_classe_eleve {Nom_Colonne, Prenom_Colonne, Groupe_Colonne, Nbr_Colonne};
    enum {Num_Role = candidat_groupe_tableau::Num_Role};

    //! Constructeur.
    candidat_groupe_model(bdd_note & bdd, idt id_groupe, QObject *parent = nullptr);

    //! Retire des élèves des groupes.
    void remove(std::vector<std::pair<idt,int>> && vec_id_num) {
        static_cast<candidat_groupe_tableau&>(*m_data).remove(std::move(vec_id_num));
        dataChanged(index(0,Groupe_Colonne),index(rowCount()-1,Groupe_Colonne));
    }
};

/*! \ingroup groupe_model_note
 * \brief Model des élèves d'une classe.
 */
class classe_eleve_model : public mps::model_base::table_model {
    Q_OBJECT
public:
    //! position des colonnes du model des éléves affectés à la classe.
    enum colonne_classe_eleve {Nom_Colonne,
                               Prenom_Colonne,
                               Naissance_Colonne,
                               Sexe_Colonne,
                               Entree_Colonne,
                               Sortie_Colonne,
                               Nbr_Colonne};

    //! Constructeur.
    classe_eleve_model(bdd_note & bdd, idt id_classe, QObject *parent = nullptr);

    //! Ajoute un élève à la classe à partir de son identifiant.
    void add(idt id_eleve);
public slots:
    //! Mutateur de l'identifiant de la classe.
    void set_id_classe(idt id_classe){
        beginResetModel();
            static_cast<classe_eleve_composition_tableau&>(*m_data).set_id_classe(id_classe);
            reset_row_to_ligne();
        endResetModel();
    }
};

/*! \ingroup groupe_model_note
 * \brief Model des éléves par groupe.
 */
class eleve_groupe_model : public mps::model_base::table_model {
    Q_OBJECT
public:
    //! Constructeur.
    eleve_groupe_model(bdd_note & bdd, QObject *parent = nullptr)
        : table_model(false,false,parent) {
        set_tableau(std::make_unique<eleve_groupe_tableau>(bdd));
    }

    //! Accesseur du groupe du tableau.
    const groupe & get_groupe() const
        {return static_cast<eleve_groupe_tableau&>(*m_data).get_groupe();}

    //! Mes à jour les éléves d'un groupe.
    void update_eleve(const std::list<idt> & list_el, szt num, const std::map<szt,std::forward_list<idt>> & map_del);

    //! Retire des élèves des groupes.
    std::vector<std::pair<idt, int>> remove(const QModelIndexList & selection);

public slots:
    //! Ajoute un groupe au model.
    void push_back();

    //! Mutateur de l'identifiant de la classe.
    void set_id_groupe(idt id_groupe);
};
}
#endif // MODEL_NOTE_H
