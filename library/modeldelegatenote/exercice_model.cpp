#include "exercice_model.h"

using namespace mps::model_base;
using namespace note_mps;

///////////////////////////////////////// exercice_edit_model
exercice_edit_model::exercice_edit_model(idt id_racine_exo, bdd_note & bdd, QObject *parent)
    : item_node_bdd_model(bdd,parent) {
    if(id_racine_exo == Id_New){
        m_data.set_root(std::make_unique<exercice_edit_node>(this));
        m_data.push_back(index(node_index(),0), std::make_unique<exercice_edit_node>(this));
    }
    else {

    }
}

numt exercice_edit_model::data_count(const node_index & index) const {
    switch (index.cible()) {
    case exercice_edit_model::Source_Cible:
    case exercice_edit_model::Texte_Cible:
    case exercice_edit_model::Titre_Cible:
    case exercice_edit_model::Version_Cible:
        return 1;
    case Sub_Node_Cible:
        return Nbr_Cible;
    }
    return item_node_model::data_count(index);
}

mps::model_base::node_ptr exercice_edit_model::node_factory(const mps::model_base::node_index & /*parent*/,
                                                            numt /*pos*/, int /*type*/)
    {return std::make_unique<exercice_edit_node>(this);}

////////////////////////////////////////////////// exercice_node /////////////////////////////////
QVariant exercice_node::data(int cible, int role, numt num) const {
    switch (cible) {
    case exercice_edit_model::Source_Cible:
        switch (role) {
        case Int_Role:
            return m_idScr;
        case Label_Role:
            return "Source :";
        case Orientation_Role:
            return Qt::Horizontal;
        case List_Of_Values:
            QMap<QString,QVariant> map;
            auto vec_scr = m_model->bdd().get_list<source>();
            for (auto it = vec_scr.cbegin(); it != vec_scr.cend(); ++it)
                map.insert(QString(it->nom()).append(" (").append(it->nc()).append(")"),it->id());
            return map;
        }
        break;
    case exercice_edit_model::Texte_Cible:
        if(role == String_Role)
            return m_texte;
        break;
    case exercice_edit_model::Titre_Cible:
        switch (role) {
        case Orientation_Role:
            return Qt::Horizontal;
        case Label_Role:
            return "Titre :";
        case String_Role:
            return m_titre;
        }
        break;
    case exercice_edit_model::Version_Cible:
        switch (role) {
        case Orientation_Role:
            return Qt::Horizontal;
        case Label_Role:
            return "Version :";
        case String_Role:
            return m_exo.version();
        }
        break;
    case mps::model_base::Sub_Node_Cible:
        if(role == Sub_Node_Role) {
            QList<QVariant> init;
            switch (num) {
            case exercice_edit_model::Source_Position:
                init.append(exercice_edit_model::Source_Cible);
                init.append(0);
                init.append(Combo_Box_Sub_Node);
                return init;
            case exercice_edit_model::Texte_Position:
                init.append(exercice_edit_model::Texte_Cible);
                init.append(0);
                init.append(Texte_Edit_Sub_Node);
                return init;
            case exercice_edit_model::Titre_Position:
                init.append(exercice_edit_model::Titre_Cible);
                init.append(0);
                init.append(Line_Edit_Sub_Node);
                return init;
            case exercice_edit_model::Version_Position:
                init.append(exercice_edit_model::Version_Cible);
                init.append(0);
                init.append(Line_Edit_Sub_Node);
                return init;
            }
        }
        break;
    case mps::model_base::Node_Cible:
        if(role == Orientation_Role)
            return Qt::Vertical;
    }
    return item_bdd_node::data(cible, role, num);
}

flag exercice_node::flags(int cible, numt num) const {
    if(m_iter.root())
        return No_Flag_Node;
    return item_bdd_node::flags(cible,num);
}

void exercice_node::insert(mps::b2d::bdd & bdd) {
    idt id_parent = 0;
    if(!m_iter.root())
        id_parent = static_cast<const exercice_node &>(**m_iter.parent()).id_exo();
    bdd.insert(m_exo,id_parent,m_iter.position());
}

flag exercice_node::set_data(int cible, const QVariant & value, int role, numt num) {
    switch (role) {
    case exercice_edit_model::Source_Cible:
        if(role == Int_Role){
            m_idScr = value.toUInt();
            return Int_Role;
        }
        break;
    case exercice_edit_model::Texte_Cible:
        if(role == String_Role){
            m_texte = value.toString();
            return String_Role;
        }
        break;
    case exercice_edit_model::Titre_Cible:
        if(role == mps::model_base::String_Role) {
            m_titre = value.toString();
            return String_Role;
        }
        break;
    case exercice_edit_model::Version_Cible:
        if(role == mps::model_base::Num_Role) {
            m_exo.set_version(value.toInt());
            return Num_Role;
        }
    }
    return item_bdd_node::set_data(cible, value, role, num);
}
