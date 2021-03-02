#include "exercice_model.h"

using namespace mps::model_base;
using namespace note_mps;

///////////////////////////////////////// exercice_edit_model
exercice_edit_model::exercice_edit_model(idt id_racine_exo, bdd_note & bdd, QObject *parent)
    : item_node_bdd_model(bdd,parent), m_id_root_exo(id_racine_exo) {
    if(m_id_root_exo == Id_New){
        insert_nodes(node_index(),0,1);
    }
    else {

    }
    connect(this,&item_node_model::nodes_inserted,this,[this](const node_index parent, numt pos, numt count) {
        auto ind = index(parent,pos + count,Label_Cible);
        while (ind.is_valid()) {
            emit data_changed(ind,Label_Role);
            ind = ind.next();
        }
    });
}

numt exercice_edit_model::data_count(const node_index & index) const {
    switch (index.cible()) {
    case exercice_edit_model::Label_Cible:
    case exercice_edit_model::Source_Cible:
    case exercice_edit_model::Texte_Cible:
    case exercice_edit_model::Type_Cible:
    case exercice_edit_model::Version_Cible:
        return 1;
    case exercice_edit_model::Titre_Cible:
        return index.parent().is_root() ? 1 : 0;
    case Sub_Node_Cible:
        return  index.parent().is_root() ? Nbr_Premier_Cible : Nbr_Suivant_Cible;
    }
    return item_node_model::data_count(index);
}

std::list<node_iter> exercice_edit_model::insert(const node_index &parent, numt pos, numt count, int type) {
    std::list<node_iter> nodes;
    if(parent.is_valid() && pos <= parent.child_count()) {
        begin_insert_nodes(parent,pos,count);
            nodes = m_data.insert_nodes(parent,pos,count,
                               [this,type](const node_index &parentArg, numt posArg){return node_factory(parentArg,posArg,type);});
            for (auto it = nodes.cbegin(); it != nodes.cend(); ++it)
                static_cast<exercice_node &>(***it).update_type();
        end_insert_nodes();
        auto ind = parent.index(Type_Cible);
        while(ind.is_valid()) {
            emit data_changed(ind,List_Of_Values);
            ind.to_parent();
        }
    }
    else if(pos == 0 && count == 1){
        begin_reset_model();
            m_data.set_root(std::make_unique<exercice_edit_node>(this));
            static_cast<exercice_node &>(**m_data.cbegin()).update_type();
            nodes.push_back(m_data.cbegin());
            auto it = m_data.push_back(index(node_index(),0), std::make_unique<exercice_edit_node>(this));
            static_cast<exercice_node &>(**it).update_type();
            nodes.push_back(it);
        end_reset_model();
    }
    return nodes;
}

mps::model_base::node_ptr exercice_edit_model::node_factory(const mps::model_base::node_index & /*parent*/,
                                                            numt /*pos*/, int /*type*/)
    {return std::make_unique<exercice_edit_node>(this);}

bool exercice_edit_model::set(const node_index &index, const QVariant &value, int role) {
    if(check_index(index)){
        auto changeRole = m_data.get_node(index).set_data(index.cible(),value,role,index.num());
        if(changeRole){
            emit data_changed(index,changeRole);
            if(index.cible() == Type_Cible && changeRole & Int_Role) {
                emit data_changed(index.index(Node_Cible),Flag_Role);
                if(!index.leaf()){
                    auto ind = index.first();
                    auto id_parent = index.data(Int_Role).toUInt();
                    while(ind.is_valid()) {
                        emit data_changed(ind,List_Of_Values);
                        auto arb = bdd().get_arbre<note_mps::type>(id_parent);
                        auto it = arb.cbegin().cbegin_child();
                        while (it && index_to_iterator(ind).hauteur() > it.hauteur())
                            ++it;
                        if(it)
                            set_data(ind,it->id(),Int_Role);
                        ind.to_next();
                    }
                }
            }
            return true;
        }
    }
    return false;
}

bool exercice_edit_model::remove(const node_index &index, numt count) {
    auto ind = index.parent();
    ind.set_cible(Type_Cible);
    auto bb = item_node_bdd_model::remove(index,count);
    while(ind.is_valid()) {
        emit data_changed(ind,List_Of_Values);
        ind.to_parent();
    }
    return bb;
}
////////////////////////////////////////////////// exercice_node /////////////////////////////////
QVariant exercice_node::data(int cible, int role, numt num) const {
    switch (cible) {
    case exercice_edit_model::Label_Cible:
        switch (role) {
        case Label_Role:
            if(m_iter.parent().root())
                return QString("Version %1 de l'exercice:").arg(m_iter.position());
            else
                return QString::number(m_iter.position());

        }
        break;
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
    case exercice_edit_model::Type_Cible:
        switch (role) {
        case Orientation_Role:
            return Qt::Horizontal;
        case Label_Role:
            return "Type :";
        case Int_Role:
            return m_exo.type();
        case List_Of_Values:{
            auto arb = m_model->bdd().get_arbre<note_mps::type>(
                        m_iter.root() ? m_model->bdd().ref_to_id<note_mps::type>("exo_root")
                                      : static_cast<exercice_node &>(**m_iter.parent()).m_exo.type());
            QMap<QString,QVariant> map;
            for (auto it = arb.cbegin().cbegin_child(); it; ++it)
                if(m_iter.hauteur() <= it.hauteur())
                    map.insert(it->nc(),it->id());
            return map;
        }

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
            if(num && !m_iter.root() && !m_iter.parent().root())
                num += exercice_edit_model::Nbr_Only_Premier_Cible;
            switch (num) {
            case exercice_edit_model::Label_Position:
                init.append(exercice_edit_model::Label_Cible);
                init.append(0);
                init.append(Label_Sub_Node);
                return init;
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
            case exercice_edit_model::Type_Position:
                 init.append(exercice_edit_model::Type_Cible);
                 init.append(0);
                 init.append(Combo_Box_Sub_Node);
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
    if(m_iter.root() && cible != exercice_edit_model::Type_Cible)
        return No_Flag_Node;
    auto fl = item_bdd_node::flags(cible,num);
    if(cible == Node_Cible) {
        if(m_model->bdd().is_leaf<note_mps::type>(m_exo.type()))
            fl ^= Elder_Enable_Flag_Node;
        if(m_iter.parent().root()) {
            if(!m_iter.last())
                fl ^= Brother_Enable_Flag_Node | Del_Enable_Flag_Node;
            else if(m_iter.first())
                fl ^= Del_Enable_Flag_Node;
        }
    }
    return fl;
}

void exercice_node::insert(mps::b2d::bdd & bdd) {
    idt id_parent = 0;
    if(!m_iter.root())
        id_parent = static_cast<const exercice_node &>(**m_iter.parent()).id_exo();
    bdd.insert(m_exo,id_parent,m_iter.position());
}

flag exercice_node::set_data(int cible, const QVariant & value, int role, numt num) {
    switch (cible) {
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
    case exercice_edit_model::Type_Cible:
        if(role == mps::model_base::Int_Role) {
            m_exo.set_type(value.toUInt());
            return Int_Role;
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
