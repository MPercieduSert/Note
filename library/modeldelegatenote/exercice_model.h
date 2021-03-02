/*Auteur: PERCIE DU SERT Maxime
 *Date: 18/01/2021
 */
#ifndef EXERCICE_MODEL_H
#define EXERCICE_MODEL_H

#include "bdd_note.h"
#include "item_node_model.h"

namespace note_mps {
/*! \ingroup groupe_modelNote
 * \brief Model d'édition d'exercice.
 */
class exercice_edit_model : public mps::model_base::item_node_bdd_model {
    Q_OBJECT
protected:
    idt m_id_root_exo;      //! Identifiant de la racine de l'exercice, nul si l'exercice est nouveau.
    enum {Id_New = 0};
public:
    //! Cible des données du model.
    enum data_cible {Label_Cible,
                     Version_Cible =1,
                     Source_Cible = 2,
                     Texte_Cible = 3,
                     Titre_Cible = 4,
                     Type_Cible = 5,
                     Nbr_Premier_Cible,
                     Nbr_Only_Premier_Cible = 1,
                     Nbr_Suivant_Cible = Nbr_Premier_Cible - Nbr_Only_Premier_Cible};

    //! position des sous-noeud.
    enum position_node{Zero_Position,
                       Un_Position,
                       Deux_Position,
                       Trois_Position,
                       Quatre_Position,
                       Cinq_Position,
                       Label_Position = Zero_Position,
                       Source_Position = Cinq_Position,
                       Texte_Position = Trois_Position,
                       Titre_Position = Un_Position,
                       Type_Position = Deux_Position,
                       Version_Position = Quatre_Position,
                       };

    //! constructeur.
    exercice_edit_model(idt id_racine_exo, bdd_note & bdd, QObject *parent = nullptr);

    //! Accesseur de la base de donnée.
    bdd_note & bdd() const
        {return static_cast<bdd_note &>(m_bdd);}

    //! Nombre de donné associé à une cible.
    numt data_count(const node_index & index) const override;

    //! Insert count noeuds de nature type avant la position pos de parent.
    std::list<node_iter> insert(const node_index &parent, numt pos, numt count, int type = Default_Type) override;

    //! Fabrique des noeuds.
    mps::model_base::node_ptr node_factory(const mps::model_base::node_index & parent, numt pos, int type) override;

    //! Mutateur des données du model.
    bool set(const node_index &index, const QVariant &value, int role) override;

    //! Supprimer count noeud de la fratrie en commençant par le noeud node.
    bool remove(const node_index &index, numt count = 1) override;
};

/*! \ingroup groupe_modelNote
 * \brief Noeud parent des model d'exercice.
 */
class exercice_node : public mps::model_base::item_bdd_node {
protected:
    exercice_edit_model *m_model;
    exercice m_exo;     //!< exercice associé au noeud.
    idt m_idScr;        //!< Identifiant de la source.
    QString m_texte;    //!< Texte du noeud.
    QString m_titre;    //!< Titre de d'exercice.
public:
    //! Constructeur.
    exercice_node(exercice_edit_model *model)
        : m_model(model){}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;

    //! Renvoie l'identifiant de l'exo.
    idt id_exo() const noexcept
        {return m_exo.id();}

    //! Enregistre les données du noeud.
    void insert(mps::b2d::bdd & bdd) override;

    //! Enregistre les données du noeud.
    void save(mps::b2d::bdd & /*bdd*/) override {}

    //! Mutateur des données du noeud.
    flag set_data(int cible, const QVariant & value, int role, numt num = 0) override;

    //! Mets jour le type d'un nouveau noeud.
    void update_type() {
        if(!m_exo.id()){
            if(m_iter.root())
                m_exo.set_type(m_model->bdd().ref_to_id<note_mps::type>("question_cours"));
            else
                m_exo.set_type(m_model->bdd().get_id_child<note_mps::type>(
                               static_cast<const exercice_node &>(**m_iter.parent()).m_exo.type(),0));
        }
    }
};

/*! \ingroup groupe_modelNote
 * \brief Noeud du model d'édition d'exercice.
 */
class exercice_edit_node : public exercice_node {
protected:

public:
    //! Constructeur.
    using exercice_node::exercice_node;
};
}
#endif // EXERCICEMODEL_H
