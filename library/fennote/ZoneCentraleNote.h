/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef ZONECENTRALENOTE_H
#define ZONECENTRALENOTE_H

#include "TabNote.h"
#include "AbstractTabZoneCentrale.h"

/*! \ingroup groupeFenNote
 * \brief Zone centrale de l'application note.
 */
class ZoneCentraleNote : public AbstractTabZoneCentrale
{
public:
    //! Constructeur.
    explicit ZoneCentraleNote(TabNote * tab, QWidget * parent = nullptr);
};

#endif // ZONECENTRALENOTE_H
