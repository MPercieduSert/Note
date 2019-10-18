/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef NOYAUNOTE_H
#define NOYAUNOTE_H

#include "AbstractNoyau.h"

/*! \defgroup groupeFenNote Fenêtre note
 * \brief Ensemble des fenêtre et du noyau de l'application note.
 */

/*! \ingroup groupeFenInv
 * \brief Noyau de l'application note.
 *
 * Noyau de l'application note:
 *
 */
class NoyauNote : public AbstractNoyau
{
public:
    NoyauNote()
        : AbstractNoyau() {}
};

#endif // NOYAUNOTE_H
