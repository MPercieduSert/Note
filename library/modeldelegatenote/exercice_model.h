/*Auteur: PERCIE DU SERT Maxime
 *Date: 18/01/2021
 */
#ifndef EXERCICE_MODEL_H
#define EXERCICE_MODEL_H

#include "bdd_note.h"
#include "item_node_model.h"

namespace note_mps {
/*! \ingroup groupe_model_note
 * \brief Parent des model utilisant bdd_note.
 */
class item_node_note_model : public mps::model_base::item_node_bdd_model {
    Q_OBJECT
public:
    //! Constructeur.
    using item_node_bdd_model::item_node_bdd_model;

    //! Accesseur de la base de donnée.
    bdd_note & bdd() const
        {return static_cast<bdd_note &>(m_bdd);}
};

/*! \ingroup groupe_model_note
 * \brief Parent des models d'exercice.
 */
class exercice_model : public item_node_note_model {
    Q_OBJECT
protected:
    idt m_id_root_exo;      //! Identifiant de la racine de l'exercice.
    enum {Id_New = 0,
          All_Root = 0};
public:
    //! Type de node d'exercice.
    enum type_node_exercice {Read,
                            Read_Root,
                            Edit,
                            Edit_Root};

    //! Cible des données du model.
    enum data_cible {Label_Cible,
                     Version_Cible =1,
                     Source_Cible = 2,
                     Texte_Cible = 3,
                     Titre_Cible = 4,
                     Type_Cible = 5,
                     Premier_Nombre,
                     Only_Premier_Nombre = 1,
                     Suivant_Nombre = Premier_Nombre - Only_Premier_Nombre};

    //! position des sous-noeud.
    enum position_node{Label_Position = mps::model_base::Un_Sub_Node,
                       Source_Position = mps::model_base::Cinq_Sub_Node,
                       Texte_Position = mps::model_base::Trois_Sub_Node,
                       Titre_Position = mps::model_base::Deux_Sub_Node,
                       Type_Position = mps::model_base::Deux_Sub_Node,
                       Version_Position = mps::model_base::Quatre_Sub_Node,
                       };

    //! constructeur.
    exercice_model(idt id_racine_exo, bdd_note & bdd, QObject *parent = nullptr)
        : item_node_note_model(bdd,parent), m_id_root_exo(id_racine_exo) {}
};
/*! \ingroup groupe_model_note
 * \brief Model de lecture d'exercice.
 */
class read_exercice_model : public exercice_model {
    Q_OBJECT
protected:

public:
    //! constructeur.
    read_exercice_model(idt id_racine_exo, bdd_note & bdd, QObject *parent = nullptr)
        : exercice_model(id_racine_exo,bdd,parent)
       {insert_nodes(node_index(),0,1);}

    //! Insert count noeuds de nature type avant la position pos de parent.
    std::list<node_iter> insert(const node_index &parent, numt pos, numt count, int type = Default_Type) override;
protected:
    //! Fabrique des noeuds.
    mps::model_base::node_ptr node_factory(const mps::model_base::node_index & parent, numt pos, int type) override;
};

/*! \ingroup groupe_model_note
 * \brief Model d'édition d'exercice.
 */
class edit_exercice_model : public exercice_model {
    Q_OBJECT
public:
    //! constructeur.
    edit_exercice_model(idt id_racine_exo, bdd_note & bdd, QObject *parent = nullptr);

    //! Insert count noeuds de nature type avant la position pos de parent.
    std::list<node_iter> insert(const node_index &parent, numt pos, numt count, int type = Default_Type) override;

    //! Mutateur des données du model.
    bool set(const node_index &index, const QVariant &value, int role) override;

    //! Supprimer count noeud de la fratrie en commençant par le noeud node.
    bool remove(const node_index &index, numt count = 1) override;
protected:
    //! Fabrique des noeuds.
    mps::model_base::node_ptr node_factory(const mps::model_base::node_index & parent, numt pos, int type) override;
};

/*! \ingroup groupe_model_note
 * \brief Parent des noeuds des models d'exercice.
 */
class exercice_node : public mps::model_base::item_bdd_node {
protected:
    exercice_model *m_model;            //!< Pointeur sur le model contenant le noeud.
    exercice m_exo;                     //!< exercice associé au noeud.
    mps::vector_ptr<source> m_source;   //!< Identifiant de la source.
    texte m_texte;                      //!< Texte du noeud.
public:
    //! Constructeur.
    exercice_node(exercice_model *model = nullptr, int type = item_node::No_Type)
        : item_bdd_node(type), m_model(model){}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;

    //! Renvoie l'identifiant de l'exo.
    idt id_exo() const noexcept
        {return m_exo.id();}

    //! Mutateur de l'identifiant de l'exercice.
    void set_id_exo(idt id);

    //! Mets jour le type d'un nouveau noeud.
    void update_type();
};

/*! \ingroup groupe_model_note
 * \brief Parent des racines premières d'exercices.
 */
class exercice_root : public virtual exercice_node {
protected:
    QString m_titre;    //!< Titre de d'exercice.
public:
    //! Constructeur.
    using exercice_node::exercice_node;

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;
};

/*! \ingroup groupe_model_note
 * \brief Noeud de lecture des model d'exercice.
 */
class read_exercice_node : public exercice_node {
protected:

public:
    //! Constructeur.
    read_exercice_node(exercice_model * model, int type = exercice_model::Read)
        : exercice_node(model,type) {}

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;
};

/*! \ingroup groupe_model_note
 * \brief Racine de lecture des model d'exercice.
 */
class read_exercice_root : public read_exercice_node {
protected:

public:
    //! Constructeur.
    read_exercice_root(exercice_model * model, int type = exercice_model::Read_Root)
        : read_exercice_node(model,type) {}

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;
};

/*! \ingroup groupe_model_note
 * \brief Noeud du model d'édition d'exercice.
 */
class edit_exercice_node : public virtual exercice_node {
protected:

public:
    //! Constructeur.
    edit_exercice_node(exercice_model * model = nullptr, int type = exercice_model::Edit)
        : exercice_node(model,type) {}

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;

    //! Enregistre les données du noeud.
    void insert(mps::b2d::bdd & bdd) override;

    //! Enregistre les données du noeud.
    void save(mps::b2d::bdd & /*bdd*/) override {}

    //! Mutateur des données du noeud.
    flag set_data(int cible, const QVariant & value, int role, numt num = 0) override;
};

/*! \ingroup groupe_model_note
 * \brief Noeud du model d'édition d'exercice.
 */
class edit_exercice_root : public exercice_root, public edit_exercice_node{
protected:

public:
    //! Constructeur.
    edit_exercice_root(exercice_model *model, int type = exercice_model::Edit_Root)
        : exercice_node (model,type){}

//    //! Accesseur des drapeaux associés à column.
//    flag flags(int cible, numt num = 0) const override;

//    //! Enregistre les données du noeud.
//    void insert(mps::b2d::bdd & bdd) override;

    //! Enregistre les données du noeud.
    void save(mps::b2d::bdd & /*bdd*/) override {}

    //! Mutateur des données du noeud.
    flag set_data(int cible, const QVariant & value, int role, numt num = 0) override;
};
}
#endif // EXERCICEMODEL_H
