/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/09/2018
 */
#ifndef SPINBOXANNEESCOLAIRE_H
#define SPINBOXANNEESCOLAIRE_H

#include <QMessageBox>
#include <QAbstractSpinBox>
#include <QLineEdit>
#include "entity_note.h"
#include "conteneur_ptr.h"

namespace  note_mps{
/*! \defgroup groupe_widget_note Dialogues de note
 * \brief Ensemble des widgets de l'application note.
 */

/*! \ingroup groupe_widget_note
 * \brief Widget de choix d'une Annees.
 */
class spin_box_annee_scolaire : public QAbstractSpinBox
{
    Q_OBJECT
protected:
    szt m_pos = 0;
    annee m_value;          //!< Annee courante.
    mps::vector_ptr<annee> m_vec;   //!< Tableau d'année.

public:
    //! Constructeur.
    spin_box_annee_scolaire(QWidget *parent = nullptr);

    //! Constructeur.
    spin_box_annee_scolaire(const annee & an, QWidget *parent = nullptr)
        : spin_box_annee_scolaire(parent)
        {set_value(an);}

    //! Constructeur.
    spin_box_annee_scolaire(const mps::vector_ptr<annee> & vec, QWidget *parent = nullptr)
        : spin_box_annee_scolaire(parent)
        {set_values(vec);}

    //! Constructeur.
    spin_box_annee_scolaire(mps::vector_ptr<annee> && vec, QWidget *parent = nullptr)
        : spin_box_annee_scolaire(parent)
        {set_values(std::move(vec));}

    //! Place la valeur sur l'annee actuelle si possible ou sur la dernière année de la liste sinon.
    void setNowValue();

    //! Mutateur de l'année courante (recherche par num (by_num=true) ou par id (by_num=false).
    void set_value(const annee & an, bool by_num = true);

    //! Mutateur de la liste des année disponible courante.
    void set_values(const mps::vector_ptr<annee> & vec)
        {m_vec = vec;}

    //! Mutateur de la liste des année disponible courante.
    void set_values(mps::vector_ptr<annee> && vec)
        {m_vec = std::move(vec);}

    //! Taille du widget.
    QSize sizeHint() const override
        {return QSize(fontMetrics().size(Qt::TextSingleLine,text()));}

    //! incrémente de steps fois la valeur.
    void stepBy(int steps) override;

    //! Accesseur de l'année courante.
    const annee & value() const {
        return m_vec.empty() ? m_value
                              : m_vec.at(m_pos);
    }

    //! Accesseur de la liste des valeurs.
    const mps::vector_ptr<annee> & values() const
        {return m_vec;}

signals:
    //! La valeur afficher change.
    void value_changed();

public slots:
    //! Affiche la valeur.
    void print_value();

protected:
    QAbstractSpinBox::StepEnabled stepEnabled() const override
        {return (StepUpEnabled| StepDownEnabled);}
};
}
#endif // SPINBOXANNEESCOLAIRE_H
