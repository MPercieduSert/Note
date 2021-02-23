/*Auteur: PERCIE DU SERT Maxime
 *Date: 18/01/2021
 */
#ifndef EXERCICEMODEL_H
#define EXERCICEMODEL_H

#include "BddNote.h"
#include "item_node_model.h"

namespace noteMPS {
/*! \ingroup groupe_modelNote
 * \brief Model d'édition d'exercice.
 */
class ExerciceEditModel : public model_base::item_node_bdd_model {
    Q_OBJECT
protected:
    enum {IdNew = 0};
public:
    //! Cible des données du model.
    enum data_cible {SourceCible,
                    texte_cible,
                    TitreCible,
                    VersionCible,
                    NbrCible};

    //! position des sous-noeud.
    enum position_node{Zero_Position,
                      Un_Position,
                      Deux_Position,
                      Trois_Position,
                      Titre_Position = Zero_Position,
                      Texte_Position = Un_Position,
                      Versionposition = Deux_Position,
                      Source_Position = Trois_Position};

    //! constructeur.
    ExerciceEditModel(idt idRacineExo, BddNote & bdd, QObject * parent = nullptr);

    //! Accesseur de la base de donnée.
    BddNote & bdd() const
        {return static_cast<BddNote &>(m_bdd);}

    //! Nombre de donné associé à une cible.
    numt data_count(const model_base::node_index & index) const override;

    //! Fabrique des noeuds.
    model_base::node_ptr node_factory(const model_base::node_index & parent, numt pos, int type) override;
};

/*! \ingroup groupe_modelNote
 * \brief Noeud parent des model d'exercice.
 */
class Exercicenode_ptr : public model_base::item_bdd_node {
protected:
    ExerciceEditModel * m_model;
    Exercice m_exo;     //!< Exercice associé au noeud.
    idt m_idScr;        //!< Identifiant de la source.
    QString m_texte;    //!< Texte du noeud.
    QString m_titre;    //!< Titre de d'exercice.
public:
    //! Constructeur.
    Exercicenode_ptr(ExerciceEditModel * model)
        : m_model(model){}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;

    //! Renvoie l'identifiant de l'exo.
    idt idExo() const noexcept
        {return m_exo.id();}

    //! Enregistre les données du noeud.
    void insert(b2d::Bdd & bdd) override;

    //! Enregistre les données du noeud.
    void save(b2d::Bdd & bdd) override {}

    //! Mutateur des données du noeud.
    flag set_data(int cible, const QVariant & value, int role, numt num = 0) override;
};

/*! \ingroup groupe_modelNote
 * \brief Noeud du model d'édition d'exercice.
 */
class ExerciceEditnode_ptr : public Exercicenode_ptr {
protected:

public:
    //! Constructeur.
    using Exercicenode_ptr::Exercicenode_ptr;
};
}
#endif // EXERCICEMODEL_H
