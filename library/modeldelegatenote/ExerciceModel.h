/*Auteur: PERCIE DU SERT Maxime
 *Date: 18/01/2021
 */
#ifndef EXERCICEMODEL_H
#define EXERCICEMODEL_H

#include "BddNote.h"
#include "ItemNodeModel.h"

namespace noteMPS {
/*! \ingroup groupeModelNote
 * \brief Model d'édition d'exercice.
 */
class ExerciceEditModel : public modelMPS::ItemNodeBddModel {
    Q_OBJECT
protected:
    enum {IdNew = 0};
public:
    //! Cible des données du model.
    enum dataCible {SourceCible,
                    TexteCible,
                    TitreCible,
                    VersionCible,
                    NbrCible};

    //! Position des sous-noeud.
    enum positionNode{ZeroPosition,
                      UnPosition,
                      DeuxPosition,
                      TroisPosition,
                      TitrePosition = ZeroPosition,
                      TextePosition = UnPosition,
                      VersionPosition = DeuxPosition,
                      SourcePosition = TroisPosition};

    //! constructeur.
    ExerciceEditModel(idt idRacineExo, BddNote & bdd, QObject * parent = nullptr);

    //! Accesseur de la base de donnée.
    BddNote & bdd() const
        {return static_cast<BddNote &>(m_bdd);}

    //! Nombre de donné associé à une cible.
    numt dataCount(const modelMPS::NodeIndex & index) const override;

    //! Fabrique des noeuds.
    modelMPS::Node nodeFactory(const modelMPS::NodeIndex & parent, numt pos, int type) override;
};

/*! \ingroup groupeModelNote
 * \brief Noeud parent des model d'exercice.
 */
class ExerciceNode : public modelMPS::ItemBddNode {
protected:
    ExerciceEditModel * m_model;
    Exercice m_exo;     //!< Exercice associé au noeud.
    idt m_idScr;        //!< Identifiant de la source.
    QString m_texte;    //!< Texte du noeud.
    QString m_titre;    //!< Titre de d'exercice.
public:
    //! Constructeur.
    ExerciceNode(ExerciceEditModel * model)
        : m_model(model){}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;

    //! Renvoie l'identifiant de l'exo.
    idt idExo() const noexcept
        {return m_exo.id();}

    //! Enregistre les données du noeud.
    void insert(bddMPS::Bdd & bdd) override;

    //! Enregistre les données du noeud.
    void save(bddMPS::Bdd & bdd) override {}

    //! Mutateur des données du noeud.
    flag setData(int cible, const QVariant & value, int role, numt num = 0) override;
};

/*! \ingroup groupeModelNote
 * \brief Noeud du model d'édition d'exercice.
 */
class ExerciceEditNode : public ExerciceNode {
protected:

public:
    //! Constructeur.
    using ExerciceNode::ExerciceNode;
};
}
#endif // EXERCICEMODEL_H
