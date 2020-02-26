/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef FENPRINCIPALENOTE_H
#define FENPRINCIPALENOTE_H

#include <qobject.h>
#include "NewModifForm.h"
#include "NoyauNote.h"
#include "ZoneCentraleNote.h"
#include "PredefFenPrincipale.h"

namespace noteMPS {
/*! \ingroup groupeFenNote
 * \brief Fenêtre principale de l'application note.
 *
 * Fenêtre principale de l'application note:
 *  + Crée les différents menus.
 *
 */
class FenPrincipaleNote : public fmps::PredefFenPrincipale {
    Q_OBJECT
protected:
    QMenu *m_menuNew;               //!< Menu nouveau.
    QMenu *m_menuModif;             //!< Menu modifier.

public:
    //! Constructeur.
    FenPrincipaleNote(NoyauNote * noyau,
                      std::unique_ptr<BddNote>  bdd,
                      ZoneCentraleNote * centralZone,
                      const QString &bddPathXML = DATA_BASE,
                      const QString & configPath = QDir::currentPath().append("/Config.xml"),
                      QWidget *parent = nullptr);

    //! Destructeur.
    ~FenPrincipaleNote() = default;

    //! Création du sous-menu nouvau.
    void createMenuModif();

    //! Création du sous-menu nouvau.
    void createMenuNew();

    //! Acceseur du noyau.
    NoyauNote * noyau()
        {return static_cast<NoyauNote *>(m_noyau);}
};
}
#endif // FENPRINCIPALENOTE_H
