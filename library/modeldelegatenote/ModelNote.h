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
    CandidatGroupeModel(BddNote & bdd, szt idGroupe, QObject * parent = nullptr)
        : TableModel(false,true,parent),
          m_groupeInfo({Qt::ItemIsEnabled|Qt::ItemIsSelectable,0,tr("Groupe"),0,CandidatGroupeTableau::EleveGroupeTableau}){
        setTableau(std::make_unique<CandidatGroupeTableau>(bdd,idGroupe));
        insertColonne(NomColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                    EleveVecTableau::Nom,tr("Nom"),0,CandidatGroupeTableau::EleveTableau});
        insertColonne(PrenomColonne,{Qt::ItemIsEnabled|Qt::ItemIsSelectable,
                                       EleveVecTableau::Prenom,tr("Prenom"),0,CandidatGroupeTableau::EleveTableau});
        insertColonne(GroupeColonne,m_groupeInfo);
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
    ClasseEleveModel(BddNote & bdd, szt idClasse, QObject * parent = nullptr);

    //! Ajoute un élève à la classe à partir de son identifiant.
    void add(szt idEleve);
public slots:
    //! Mutateur de l'identifiant de la classe.
    void setIdClasse(szt idClasse){
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

    //! Ajoute un éléve à un groupe.
    void updateEleve(const std::list<szt> & listEl, szt num, const std::map<szt,std::forward_list<szt>> & mapDel) {
        auto & tableau = static_cast<EleveGroupeTableau&>(*m_data);
        if(num < tableau.sizeColumn()){
            if(mapDel.empty()) {
                if(tableau.sizeOfColumn(num) + listEl.size() < nbrLignes()){
                    tableau.addEleve(listEl,num);
                    dataChanged(index(0,static_cast<int>(num)),index(
                                    tableau.sizeOfColumn(num) < nbrRow() ? static_cast<int>(tableau.sizeOfColumn(num))
                                                                         : rowCount(),
                                    static_cast<int>(num)));
                }
                else {
                    beginInsertRows(QModelIndex(), rowCount(), static_cast<int>(tableau.sizeOfColumn(num) + listEl.size() - 1));
                        tableau.addEleve(listEl,num);
                        resetRowToLigne();
                    endInsertRows();
                    dataChanged(index(0,static_cast<int>(num)),index(rowCount() - 1,static_cast<int>(num)));
                }
            }
            else {
                beginResetModel();
                    tableau.addEleve(listEl,num);
                    tableau.delEleve(mapDel);
                    resetRowToLigne();
                endResetModel();
            }
        }
    }

    //! Accesseur du groupe du tableau.
    const Groupe & groupe() const
        {return static_cast<EleveGroupeTableau&>(*m_data).groupe();}

public slots:
    //! Ajoute un groupe au model.
    void push_back() {
        static_cast<EleveGroupeTableau&>(*m_data).push_back();
        NewColonneInfo info;
        info.flags = Qt::ItemIsEnabled|Qt::ItemIsSelectable;
        info.name = static_cast<EleveGroupeTableau&>(*m_data).groupe().alphaTexte(columnCount());
        info.tableau = static_cast<szt>(columnCount());
        push_backColonne(info);
    }

    //! Mutateur de l'identifiant de la classe.
    void setIdGroupe(szt idGroupe){
        beginResetModel();
            static_cast<EleveGroupeTableau&>(*m_data).setIdGroupe(idGroupe);
            m_colonnes.clear();
            NewColonneInfo info;
            info.flags = Qt::ItemIsEnabled|Qt::ItemIsSelectable;
            for (szt num = 0; num != static_cast<EleveGroupeTableau&>(*m_data).sizeColumn(); ++num){
                info.name = static_cast<EleveGroupeTableau&>(*m_data).groupe().alphaTexte(static_cast<int>(num));
                info.tableau = num;
                push_backColonne(info);
            }
            resetRowToLigne();
        endResetModel();
    }
};
}
#endif // MODELNOTE_H
