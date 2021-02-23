/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef FENPRINCIPALENOTE_H
#define FENPRINCIPALENOTE_H

#include <QDockWidget>
#include "find_widget.h"
#include "NewModifForm.h"
#include "NewModifPredef.h"
#include "NoyauNote.h"
#include "ZoneCentraleNote.h"
#include "predef_fen_principale.h"

namespace noteMPS {
/*! \ingroup groupe_fenNote
 * \brief Fenêtre principale de l'application note.
 *
 * Fenêtre principale de l'application note:
 *  + Crée les différents menus.
 *
 */
class fen_principaleNote : public fmps::predef_fen_principale {
    Q_OBJECT
protected:

public:
    //! Constructeur.
    fen_principaleNote(NoyauNote * noyau,
                      std::unique_ptr<BddNote>  bdd,
                      ZoneCentraleNote * central_zone,
                      const QString &bdd_path_xml = DATA_BASE,
                      const QString & config_path = QDir::currentPath().append("/Config.xml"),
                      QWidget *parent = nullptr);

    //! Destructeur.
    ~fen_principaleNote() override = default;

    //! Création du sous-menu nouvau.
    void create_menuNewModif();

    //! Acceseur du noyau.
    NoyauNote * noyau() override
        {return static_cast<NoyauNote *>(m_noyau);}
};
}
#endif // FENPRINCIPALENOTE_H
