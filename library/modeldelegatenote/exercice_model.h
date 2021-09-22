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
 * \brief Espace de noms des modèles d'exercices.
 */
namespace model_exo {
//! Indice des type de texte.
enum type_texte {
    Main_Texte,
    Titre_Texte,
    Correction_Texte,
    Nbr_Type_Texte};

//! Type de node d'exercice.
enum type_node_exercice {
    Read,
    Read_Root,
    Find,
    Find_Root,
    Edit,
    Edit_Root};

//! Cible des données du model.
enum data_cible {
    En_Tete_Cible,
    Source_Cible,
    Texte_Cible,
    Titre_Cible,
    Type_Cible,
    Type_Titre_Cible,
    Version_Cible,
    Version_Exercice_Cible};

/*! \ingroup groupe_model_note
 * \brief Parent des models d'exercice.
 */
class exercice_model : public item_node_note_model {
    Q_OBJECT
protected:
    enum {Id_New = 0,
          All_Root = 0,
          Version_Style_Num = 0};

    idt m_id_root_exo;                                          //!< Identifiant de la racine de l'exercice.
    const std::array<idt,Nbr_Type_Texte> m_type_texte;          //!< Identifiant des type de texte.
    std::map<idt,mps::divers::num_to_string> m_num_to_string;   //!< Association entre les types d'exercice et la numérotation.
public:
    //! constructeur.
    exercice_model(idt id_racine_exo, bdd_note & bdd, QObject *parent = nullptr);

    //! Accesseur des type de texte.
    idt type_texte(enumt num) const noexcept
        {return num < Nbr_Type_Texte ? m_type_texte[num] : 0;}

    //! Convertion d'un entier en QString avec le style associé un type donnée.
    QString num_to_string(int num, idt type = Version_Style_Num) const
        {return m_num_to_string.at(type).texte(num);}
};
/*! \ingroup groupe_model_note
 * \brief Model de lecture d'exercice.
 */
class find_exercice_model : public exercice_model {
    Q_OBJECT
protected:

public:
    //! constructeur.
    find_exercice_model(idt id_racine_exo, bdd_note & bdd, QObject *parent = nullptr)
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
    exercice_model *m_model;                                        //!< Pointeur sur le model contenant le noeud.
    exercice m_exo;                                                 //!< exercice associé au noeud.
    std::array<std::set<idt>,Nbr_Type_Texte> m_source;              //!< Source associé à un texte
    std::array<texte,Nbr_Type_Texte> m_texte;                       //!< Texte du noeud.

public:
    //! Constructeur.
    exercice_node(exercice_model *model, idt exo_id = 0, int type = item_node::No_Type)
        : item_bdd_node(type), m_model(model)
    {set_id_exo(exo_id);}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;

    //! Renvoie l'identifiant de l'exo.
    idt id_exo() const noexcept
        {return m_exo.id();}

    //! Mutateur de l'identifiant de l'exercice.
    void set_id_exo(idt id);

    //! Accesseur du type d'exercice.
    idt type_exo() const noexcept
        {return m_exo.type();}

    //! Mets jour le type d'un nouveau noeud.
    void update_type();
};

/*! \ingroup groupe_model_note
 * \brief Noeud de lecture des model d'exercice.
 */
class read_exercice_node : public exercice_node {
protected:

public:
    //! Constructeur.
    read_exercice_node(exercice_model *model, idt exo_id = 0, int type = Read)
        : exercice_node(model,exo_id,type) {}

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
    read_exercice_root(exercice_model *model, idt exo_id = 0, int type = Read_Root)
        : read_exercice_node(model,exo_id,type) {}

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;
};

/*! \ingroup groupe_model_note
 * \brief Noeud du model de recherche d'exercice.
 */
class find_exercice_node : public exercice_node {
protected:

public:
    //! Constructeur.
    find_exercice_node(exercice_model *model, idt exo_id = 0, int type = Find)
        : exercice_node(model,exo_id,type) {}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;
};

/*! \ingroup groupe_model_note
 * \brief Racine du model de recherche d'exercice.
 */
class find_exercice_root : public find_exercice_node {
protected:
    exercice m_root_exo;    //! Racine de l'exercice.
public:
    //! Constructeur.
    find_exercice_root(exercice_model * model, idt exo_id = 0, int type = Find_Root)
        : find_exercice_node(model,0,type)
        {set_id_exo(exo_id);}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;

    //! Mutateur de l'identifiant de l'exercice.
    void set_id_exo(idt id);
};

/*! \ingroup groupe_model_note
 * \brief Noeud du model d'édition d'exercice.
 */
class edit_exercice_node : public exercice_node {
protected:

public:
    //! Constructeur.
    edit_exercice_node(exercice_model *model, idt exo_id = 0, int type = Edit)
        : exercice_node(model,exo_id,type) {}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;

    //! Enregistre les données du noeud.
    void insert(mps::b2d::bdd &bdd) override;

    //! Enregistre les données du noeud.
    void save(mps::b2d::bdd &bdd) override
        {save_textes(bdd);}

    //! Enregistre les textes du noeud.
    void save_textes(mps::b2d::bdd &bdd);

    //! Mutateur des données du noeud.
    flag set_data(int cible, const QVariant & value, int role, numt num = 0) override;
};

/*! \ingroup groupe_model_note
 * \brief Noeud du model d'édition d'exercice.
 */
class edit_exercice_root : public edit_exercice_node{
protected:

public:
    //! Constructeur.
    edit_exercice_root(exercice_model *model, idt exo_id = 0, int type = Edit_Root)
        : edit_exercice_node (model,exo_id,type){}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;

//    //! Enregistre les données du noeud.
//    void insert(mps::b2d::bdd & bdd) override;

    //! Enregistre les données du noeud.
    void save(mps::b2d::bdd & /*bdd*/) override {}

    //! Mutateur des données du noeud.
    flag set_data(int cible, const QVariant & value, int role, numt num = 0) override;
};
}}
#endif // EXERCICEMODEL_H
