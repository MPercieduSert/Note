/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef FENPRINCIPALENOTE_H
#define FENPRINCIPALENOTE_H

#include "NoyauNote.h"
#include "ZoneCentraleNote.h"
#include "FenPrincipale.h"

/*! \ingroup groupeFenNote
 * \brief Fenêtre principale de l'application note.
 *
 * Fenêtre principale de l'application note:
 *  + Crée les différents menus.
 *
 */
class FenPrincipaleNote : public FenPrincipale
{
public:
    //! Constructeur.
    FenPrincipaleNote(NoyauNote * noyau, ZoneCentraleNote * centralZone, QWidget *parent = nullptr);
};

#endif // FENPRINCIPALENOTE_H
