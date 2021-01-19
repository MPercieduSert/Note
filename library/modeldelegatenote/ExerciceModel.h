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
    enum dataCible {TexteCible,
                    TitreCible,
                    VersionCible};
    //! constructeur.
    ExerciceEditModel(idt idRacineExo, BddNote & bdd, QObject * parent = nullptr);

    //! Accesseur de la base de donnée.
    BddNote & bdd() const
        {return static_cast<BddNote &>(m_bdd);}
};

/*! \ingroup groupeModelNote
 * \brief Noeud parent des model d'exercice.
 */
class ExerciceNode : public modelMPS::ItemBddNode {
protected:
    ExerciceEditModel * m_model;
    Exercice m_exo;     //!< Exercice associé au noeud.
    QString m_texte;    //!< Texte du noeud.
    QString m_titre;    //!< Titre de d'exercice.
    numt m_version;     //!< Verstion du texte.
public:
    //! Constructeur.
    ExerciceNode(ExerciceEditModel * model)
        : m_model(model){}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    //flag flags(int cible, numt num = 0) const override;

    //! Enregistre les données du noeud.
    void insert(bddMPS::Bdd & bdd) override {}

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

};
}
#endif // EXERCICEMODEL_H
