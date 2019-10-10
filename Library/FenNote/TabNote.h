/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef TABNOTE_H
#define TABNOTE_H

#include "fen/AbstractTab.h"

/*! \ingroup groupeFenInv
 * \brief Fenêtre à onglet de l'application inventaire.
 */
class TabNote : public AbstractTab
{
public:
    TabNote() = default;

    //! Connect l'onglet aux signaux commun à tous les onglets.
    void connectTab(AbstractTabModule * tab) const override
        {}

    //! Créateur d'onglet.
    AbstractTabModule *createTab(QPair<int,int> pair) const override
        {return nullptr;}
};

#endif // TABNOTE_H
