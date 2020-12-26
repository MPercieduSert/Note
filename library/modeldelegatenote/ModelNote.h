/*Auteur: PERCIE DU SERT Maxime
 *Date: 23/05/2020
 */
#ifndef MODELNOTE_H
#define MODELNOTE_H

#include "BddNote.h"
#include "TableModel.h"
#include "TableauNote.h"

namespace noteMPS {
/*! \ingroup groupeModelNote
 * \brief Model de la liste des élèves à affecter dans un groupe.
 */
class CandidatGroupeModel : public modelMPS::TableModel {
    Q_OBJECT
protected:
    AbstractColonnesModel::NewColonneInfo m_groupeInfo;     //! Information sur la colone de groupe
public:
    //! Position des colonnes du model des éléves affectés à la classe.
    enum colonneClasseEleve {NomColonne, PrenomColonne, GroupeColonne, NbrColonne};
    enum {NumRole = CandidatGroupeTableau::NumRole};

    //! Constructeur.
    CandidatGroupeModel(BddNote & bdd, idt idGroupe, QObject * parent = nullptr);

    //! Retire des élèves des groupes.
    void remove(std::vector<std::pair<idt,int>> && vecIdNum) {
        static_cast<CandidatGroupeTableau&>(*m_data).remove(std::move(vecIdNum));
        dataChanged(index(0,GroupeColonne),index(rowCount()-1,GroupeColonne));
    }
};

/*! \ingroup groupeModelNote
 * \brief Model des élèves d'une classe.
 */
class ClasseEleveModel : public modelMPS::TableModel {
    Q_OBJECT
public:
    //! Position des colonnes du model des éléves affectés à la classe.
    enum colonneClasseEleve {NomColonne, PrenomColonne, NaissanceColonne, SexeColonne, EntreeColonne, SortieColonne, NbrColonne};

    //! Constructeur.
    ClasseEleveModel(BddNote & bdd, idt idClasse, QObject * parent = nullptr);

    //! Ajoute un élève à la classe à partir de son identifiant.
    void add(idt idEleve);
public slots:
    //! Mutateur de l'identifiant de la classe.
    void setIdClasse(idt idClasse){
        beginResetModel();
            static_cast<ClasseEleveCompositionTableau&>(*m_data).setIdClasse(idClasse);
            resetRowToLigne();
        endResetModel();
    }
};

/*! \ingroup groupeModelNote
 * \brief Model des éléves par groupe.
 */
class EleveGroupeModel : public modelMPS::TableModel {
    Q_OBJECT
public:
    //! Constructeur.
    EleveGroupeModel(BddNote & bdd, QObject * parent = nullptr)
        : TableModel(false,false,parent) {
        setTableau(std::make_unique<EleveGroupeTableau>(bdd));
    }

    //! Accesseur du groupe du tableau.
    const Groupe & groupe() const
        {return static_cast<EleveGroupeTableau&>(*m_data).groupe();}

    //! Mes à jour les éléves d'un groupe.
    void updateEleve(const std::list<idt> & listEl, szt num, const std::map<szt,std::forward_list<idt>> & mapDel);

    //! Retire des élèves des groupes.
    std::vector<std::pair<idt, int> > remove(const QModelIndexList & selection);

public slots:
    //! Ajoute un groupe au model.
    void push_back();

    //! Mutateur de l'identifiant de la classe.
    void setIdGroupe(idt idGroupe);
};
}
#endif // MODELNOTE_H
