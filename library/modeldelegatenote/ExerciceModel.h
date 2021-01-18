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
public:
    //! constructeur.
    ExerciceEditModel(BddNote & bdd);

    //! Accesseur de la base de donnée.
    BddNote & bdd() const
        {return static_cast<BddNote &>(m_bdd);}
};

/*! \ingroup groupeModelNote
 * \brief Noeud parent des model d'exercice.
 */
class ExerciceNode : public modelMPS::ItemBddNode {
protected:
    Exercice m_exo;     //! Exercice Associé au noeud.
public:


};

/*! \ingroup groupeModelNote
 * \brief Noeud du model d'édition d'exercice.
 */
class ExerciceEditNode : public ExerciceNode {
protected:
    ExerciceEditNode * m_model;
public:
    //! Constructeur.
    ExerciceEditNode(ExerciceEditNode * model)
        : m_model(model){}
};
}
#endif // EXERCICEMODEL_H
