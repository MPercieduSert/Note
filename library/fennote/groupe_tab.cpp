#include "groupe_tab.h"

using namespace note_mps;

groupe_tab::groupe_tab(bdd_note & bdd, tab_index pair, QWidget *parent)
    : abstract_tab_tableau(bdd,pair,parent) {
    // Choix du groupe.
    m_groupe_select = new groupe_select_widget(bdd);

    // Model et vue.
    m_model = new eleve_groupe_model(bdd,this);
    m_view = new QTableView;
    m_view->setModel(m_model);
    m_view->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_candidat_view = new QTableView;
    m_candidat_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_candidat_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_candidat_view->setSortingEnabled(true);
    m_candidat_view->horizontalHeader()->setSectionsMovable(true);
    m_candidat_view->horizontalHeader()->setSectionsClickable(true);

    // Recheche.
    m_find_widget = new mps::widget::find_widget();
    m_find_layout = new QVBoxLayout;
    m_find_group = new QGroupBox(tr("Filtrer :"));
    m_find_layout->addWidget(m_find_widget);
    m_find_group->setLayout(m_find_layout);

    // Boutons
    m_del_eleve_bouton = new QPushButton(tr("Retirer les éléves"));
    m_add_groupe_bouton = new QPushButton(tr("Ajouter un groupe"));
    m_save_bouton = new QPushButton(tr("Sauvegarder"));
    m_prop_gr_label = new QLabel(tr("Propriétés du groupe :"));
    m_exclusif_check = new QCheckBox(tr("Exclusif"));
    m_exclusif_check->setDisabled(true);
    m_total_check = new QCheckBox(tr("Total"));
    m_total_check->setDisabled(true);

    // Peuple et connect
    update_eleve();
    connect(m_groupe_select,&groupe_select_widget::id_changed,this,&groupe_tab::update_eleve);
    connect(m_add_groupe_bouton,&QPushButton::clicked,this,&groupe_tab::add_groupe);
    connect(m_del_eleve_bouton,&QPushButton::clicked,this,&groupe_tab::remove);
    connect(m_save_bouton,&QPushButton::clicked,this,&groupe_tab::sauver);
    connect(m_view->horizontalHeader(),&QHeaderView::sectionClicked,this,&groupe_tab::affecte);


    // Calque
    m_eleve_layout = new QHBoxLayout;
    m_eleve_layout->addWidget(m_candidat_view);
    m_eleve_layout->addWidget(m_find_group);
    m_bouton_layout = new QHBoxLayout;
    m_bouton_layout->addWidget(m_add_groupe_bouton);
    m_bouton_layout->addWidget(m_del_eleve_bouton);
    m_bouton_layout->addWidget(m_save_bouton);
    m_bouton_layout->addWidget(m_prop_gr_label);
    m_bouton_layout->addWidget(m_exclusif_check);
    m_bouton_layout->addWidget(m_total_check);
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_groupe_select);
    m_main_layout->addLayout(m_eleve_layout);
    m_main_layout->addLayout(m_bouton_layout);
    m_main_layout->addWidget(m_view);
}

void groupe_tab::affecte(int column) {
    if(column >= 0) {
        auto col = static_cast<numt>(column);
        auto selection = m_candidat_view->selectionModel()->selectedRows();
        std::list<idt> list_Id;
        std::map<szt,std::forward_list<idt>> del_grId;
        if(static_cast<eleve_groupe_model&>(*m_model).get_groupe().code().test(groupe::Exclusif)){
            m_candidat_view->selectionModel()->clear();
            for (auto iter = selection.cbegin(); iter != selection.cend(); ++iter) {
                auto num = m_candidat_model->data(iter->siblingAtColumn(candidat_groupe_model::Groupe_Colonne),
                                                 candidat_groupe_model::Num_Role).toInt();
                if(num != column) {
                    auto id = m_candidat_model->data(*iter,mps::model_base::abstract_colonnes_model::Id_Role).toUInt();
                    list_Id.push_front(id);
                    m_candidat_model->setData(iter->siblingAtColumn(candidat_groupe_model::Groupe_Colonne),col);
                    if(num >= 0)
                        del_grId[static_cast<szt>(num)].push_front(id);
                }
            }
        }
        else {
            for (auto iter = selection.cbegin(); iter != selection.cend(); ++iter) {
                auto listNum = m_candidat_model->data(iter->siblingAtColumn(candidat_groupe_model::Groupe_Colonne),
                                                     candidat_groupe_model::Num_Role).toList();
                auto iterNum = listNum.cbegin();
                while(iterNum != listNum.cend() && iterNum->toUInt() < col)
                    ++iterNum;
                if(iterNum == listNum.cend() || iterNum->toUInt() != col) {
                    list_Id.push_front(m_candidat_model->data(*iter,mps::model_base::abstract_colonnes_model::Id_Role).toUInt());
                    m_candidat_model->setData(iter->siblingAtColumn(candidat_groupe_model::Groupe_Colonne),col);
                }
            }
        }
        static_cast<eleve_groupe_model&>(*m_model).update_eleve(list_Id,col,del_grId);
    }
}

void groupe_tab::become_current()
    {emit action_permise(mps::fenetre::Effacer_Action | mps::fenetre::Sauver_Action);}

void groupe_tab::remove(){
    auto selection = m_view->selectionModel()->selectedIndexes();
    m_candidat_model->remove(static_cast<eleve_groupe_model&>(*m_model).remove(selection));
}

void groupe_tab::update_eleve() {
    auto model = m_candidat_model;
    m_candidat_model = new candidat_groupe_model(bdd(),m_groupe_select->id(),this);
    m_candidat_model->sort(candidat_groupe_model::Nom_Colonne);
    m_find_widget->set_model(m_candidat_model);
    m_candidat_view->setModel(m_candidat_model);
    if(model)
        delete model;
    static_cast<eleve_groupe_model&>(*m_model).set_id_groupe(m_groupe_select->id());
    m_exclusif_check->setChecked(static_cast<eleve_groupe_model&>(*m_model).get_groupe().code().test(groupe::Exclusif));
    m_total_check->setChecked(static_cast<eleve_groupe_model&>(*m_model).get_groupe().code().test(groupe::Total));
}
