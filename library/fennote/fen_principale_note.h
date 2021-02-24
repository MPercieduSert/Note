/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef FENPRINCIPALENOTE_H
#define FENPRINCIPALENOTE_H

#include <QDockWidget>
#include "find_widget.h"
#include "new_modif_form.h"
#include "new_modif_predef.h"
#include "noyau_note.h"
#include "zone_centrale_note.h"
#include "predef_fen_principale.h"

namespace note_mps {
/*! \ingroup groupe_fenNote
 * \brief Fenêtre principale de l'application note.
 *
 * Fenêtre principale de l'application note:
 *  + Crée les différents menus.
 *
 */
class fen_principale_note : public mps::fenetre::predef_fen_principale {
    Q_OBJECT
protected:

public:
    //! Constructeur.
    fen_principale_note(noyau_note * noyau,
                      std::unique_ptr<bdd_note>  bdd,
                      zone_centrale_note * central_zone,
                      const QString &bdd_path_xml = DATA_BASE,
                      const QString & config_path = QDir::currentPath().append("/Config.xml"),
                      QWidget *parent = nullptr);

    //! Destructeur.
    ~fen_principale_note() override = default;

    //! Création du sous-menu nouvau.
    void create_menu_new_modif();

    //! Acceseur du noyau.
    noyau_note * noyau() override
        {return static_cast<noyau_note *>(m_noyau);}
};
}
#endif // FENPRINCIPALENOTE_H
