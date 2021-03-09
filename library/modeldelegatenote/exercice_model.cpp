#include "exercice_model.h"

using namespace mps::model_base;
using namespace note_mps;

///////////////////////////////////////////////////////// exercice_model /////////////////////////////////////////////////
///////////////////////////////////////////////////////// read_exercice_model ////////////////////////////////////////////
std::list<node_iter> read_exercice_model::insert(const node_index &parent, numt pos, numt count, int /*type*/) {
    if(parent.is_valid() && pos <= parent.child_count()) {
//        begin_insert_nodes(parent,pos,count);
//            auto exos = bdd().get_arbre<exercice>(parent.)
//        end_insert_nodes();
    }
    else if(pos == 0 && count == 1){
        begin_reset_model();
            m_data.set_root(std::make_unique<read_exercice_node>(this));
            auto roots = bdd().get_list_childs_id<exercice>(0);
            auto root_index = index(node_index(),0);
            for (auto it = roots.cbegin(); it != roots.cend(); ++it)
                m_data.push_back(root_index,std::make_unique<read_exercice_root>(this,*it));

        end_reset_model();
    }
    return std::list<node_iter>();
}
mps::model_base::node_ptr read_exercice_model::node_factory(const mps::model_base::node_index & parent,
                                                            numt /*pos*/, int /*type*/) {
    if(parent.is_root())
        return std::make_unique<read_exercice_root>(this);
    else
        return std::make_unique<read_exercice_node>(this);
}
///////////////////////////////////////////////////////// edit_exercice_model ////////////////////////////////////////////
edit_exercice_model::edit_exercice_model(idt id_racine_exo, bdd_note & bdd, QObject *parent)
    : exercice_model(id_racine_exo,bdd,parent) {
    insert_nodes(node_index(),0,1);
    connect(this,&item_node_model::nodes_inserted,this,[this](const node_index parent, numt pos, numt count) {
        auto ind = index(parent,pos + count,Label_Cible);
        while (ind.is_valid()) {
            emit data_changed(ind,Label_Role);
            ind = ind.next();
        }
    });
}

std::list<node_iter> edit_exercice_model::insert(const node_index &parent, numt pos, numt count, int type) {
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
            emit data_changed(ind,Map_Role);
            ind.to_parent();
        }
    }
    else if(pos == 0 && count == 1){
        begin_reset_model();
            m_data.set_root(std::make_unique<edit_exercice_node>(this));
            static_cast<exercice_node &>(**m_data.cbegin()).update_type();
            nodes.push_back(m_data.cbegin());
            auto it = m_data.push_back(index(node_index(),0), std::make_unique<edit_exercice_root>(this));
            static_cast<exercice_node &>(**it).update_type();
            nodes.push_back(it);
        end_reset_model();
    }
    return nodes;
}

mps::model_base::node_ptr edit_exercice_model::node_factory(const mps::model_base::node_index & parent,
                                                            numt /*pos*/, int /*type*/) {
    if(parent.is_root())
        return std::make_unique<edit_exercice_root>(this);
    else
        return std::make_unique<edit_exercice_node>(this);
}

bool edit_exercice_model::set(const node_index &index, const QVariant &value, int role) {
    if(check_index(index)){
        auto changeRole = m_data.get_node(index).set_data(index.cible(),value,role,index.num());
        if(changeRole){
            emit data_changed(index,changeRole);
            if(index.cible() == Type_Cible && changeRole & Variant_Role) {
                emit data_changed(index.index(Node_Cible),Flag_Role);
                if(!index.leaf()){
                    auto ind = index.first();
                    auto id_parent = index.data(Variant_Role).toUInt();
                    while(ind.is_valid()) {
                        emit data_changed(ind,Map_Role);
                        auto arb = bdd().get_arbre<note_mps::type>(id_parent);
                        auto it = arb.cbegin().cbegin_child();
                        while (it && index_to_iterator(ind).hauteur() > it.hauteur())
                            ++it;
                        if(it)
                            set_data(ind,it->id(),Variant_Role);
                        ind.to_next();
                    }
                }
            }
            return true;
        }
    }
    return false;
}

bool edit_exercice_model::remove(const node_index &index, numt count) {
    auto ind = index.parent();
    ind.set_cible(Type_Cible);
    auto bb = item_node_bdd_model::remove(index,count);
    while(ind.is_valid()) {
        emit data_changed(ind,Map_Role);
        ind.to_parent();
    }
    return bb;
}

///////////////////////////////////////////////////////// exercice_node //////////////////////////////////////////////////
QVariant exercice_node::data(int cible, int role, numt num) const {
    switch (cible) {
    case exercice_model::Label_Cible:
        switch (role) {
        case Label_Role:
            if(m_iter.parent().root())
                return QString("Version %1 de l'exercice:").arg(m_iter.position());
            else
                return QString::number(m_iter.position());
        case Type_Role:
            return Label_Sub_Node;
        }
        break;
    case exercice_model::Source_Cible:
        switch (role) {
//        case Variant_Role:
//            return m_id_scr;
        case Label_Role:
            return "Source :";
        case Orientation_Role:
            return Qt::Horizontal;
        case Type_Role:
            return Combo_Box_Sub_Node;
        case Map_Role:
            QMap<QString,QVariant> map;
            auto vec_scr = m_model->bdd().get_list<source>();
            for (auto it = vec_scr.cbegin(); it != vec_scr.cend(); ++it)
                map.insert(QString(it->nom()).append(" (").append(it->nc()).append(")"),it->id());
            return map;
        }
        break;
    case exercice_model::Texte_Cible:
        if(role == String_Role)
            return m_texte.texte();
        if(role == Type_Role)
            return Texte_Edit_Sub_Node;
        break;
    case exercice_model::Type_Cible:
        switch (role) {
        case Orientation_Role:
            return Qt::Horizontal;
        case Label_Role:
            return "Type :";
        case Variant_Role:
            return m_exo.type();
        case Type_Role:
            return Combo_Box_Sub_Node;
        case Map_Role:
            auto id_type = m_iter.root() ? 0 : static_cast<exercice_node &>(**m_iter.parent()).m_exo.type();
            auto arb = m_model->bdd().get_arbre<note_mps::type>(
                        id_type ? id_type : m_model->bdd().ref_to_id<note_mps::type>("exo_root"));
            QMap<QString,QVariant> map;
            for (auto it = arb.cbegin().cbegin_child(); it; ++it)
                if(m_iter.hauteur() <= it.hauteur())
                    map.insert(it->nc(),it->id());
            return map;
        }
        break;
    case exercice_model::Version_Cible:
        switch (role) {
        case Orientation_Role:
            return Qt::Horizontal;
        case Label_Role:
            return "Version :";
        case String_Role:
            return m_exo.version();
        case Type_Role:
            return Line_Edit_Sub_Node;
        }
        break;
    case mps::model_base::Node_Cible:
        if(num == Node_Num) {
            if(role == Orientation_Role)
                return Qt::Vertical;
            if(role == Nombre_Role)
                return exercice_model::Suivant_Nombre;
        }
        else if (role == Cible_Role) {
            switch (num) {
            case exercice_model::Label_Position:
                return exercice_model::Label_Cible;
            case exercice_model::Source_Position:
                return exercice_model::Source_Cible;
            case exercice_model::Texte_Position:
                return exercice_model::Texte_Cible;
            case exercice_model::Type_Position:
                return exercice_model::Type_Cible;
            case exercice_model::Version_Position:
                return exercice_model::Version_Cible;
        }}
        break;
    }
    return item_bdd_node::data(cible, role, num);
}

flag exercice_node::flags(int cible, numt num) const {
    if(m_iter.root() && cible != exercice_model::Type_Cible)
        return No_Flag_Node;
    return item_bdd_node::flags(cible,num);
}

void exercice_node::set_id_exo(idt id) {
    m_exo.set_id(id);
    if(m_model->bdd().get(m_exo)){
        texte_cible text_cb;
        text_cb.set_cible(m_model->bdd().cible<exercice>());
        text_cb.set_id_cible(m_exo.id_original() ? m_exo.id_original() : m_exo.id());
        text_cb.set_num(m_exo.version());
        text_cb.set_type(m_model->bdd().ref_to_id<note_mps::type>("main_txt"));
        m_model->bdd().get_unique(text_cb);
        m_texte.set_id(text_cb.id_texte());
        if(m_model->bdd().get(m_texte)) {
            m_source = m_model->bdd().get_list<source,source_texte>(source_texte::Id_Source,
                                                                     source_texte::Id_Texte,
                                                                     m_texte.id(),
                                                                     source::Nom);
        }
    }
    else {
        m_exo.set_id(0);
    }
}
void exercice_node::update_type() {
    if(!id_exo()){
        if(m_iter.root())
            m_exo.set_type(m_model->bdd().ref_to_id<note_mps::type>("question_cours"));
        else
            m_exo.set_type(m_model->bdd().get_id_child<note_mps::type>(
                           static_cast<const exercice_node &>(**m_iter.parent()).m_exo.type(),0));
    }
}
///////////////////////////////////////////////////////// exercice_root //////////////////////////////////////////////////
QVariant exercice_root::data(int cible, int role, numt num) const {
    if(cible == exercice_model::Titre_Cible)
        switch (role) {
        case Orientation_Role:
            return Qt::Horizontal;
        case Label_Role:
            return "Titre :";
        case String_Role:
            return m_titre.texte();
        case Type_Role:
            return Line_Edit_Sub_Node;
        }
    if(cible == Node_Cible){
        if(num == Node_Num) {
           if(role == Nombre_Role)
            return exercice_model::Premier_Nombre;
        }
        else if (num == exercice_model::Titre_Position) {
            if(role == Cible_Role)
                return exercice_model::Titre_Cible;
        }
        else if (num > exercice_model::Titre_Position)
            num -= exercice_model::Only_Premier_Nombre;
    }
    return exercice_node::data(cible,role,num);
}
///////////////////////////////////////////////////////// read_exercice_node /////////////////////////////////////////////
flag read_exercice_node::flags(int cible, numt /*num*/) const {
    if(m_iter.root())
        return No_Flag_Node;
    if(cible == Node_Cible)
        return Visible_Flag_Node | Expendable_FLag_Node;
    return Visible_Flag_Node;
}
///////////////////////////////////////////////////////// read_exercice_root /////////////////////////////////////////////
flag read_exercice_root::flags(int cible, numt num) const {
    return read_exercice_node::flags(cible,num);
}
///////////////////////////////////////////////////////// edit_exercice_node /////////////////////////////////////////////
flag edit_exercice_node::flags(int cible, numt num) const {
    if(m_iter.root() && cible != exercice_model::Type_Cible)
        return No_Flag_Node;
    auto fl = item_bdd_node::flags(cible,num);
    if(fl && cible == Node_Cible) {
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

void edit_exercice_node::insert(mps::b2d::bdd & bdd) {
    idt id_parent = 0;
    if(!m_iter.root())
        id_parent = static_cast<const exercice_node &>(**m_iter.parent()).id_exo();
    bdd.insert(m_exo,id_parent,m_iter.position());
    texte_cible text_cb;
    text_cb.set_cible(m_model->bdd().cible<exercice>());
    text_cb.set_id_cible(m_exo.id_original() ? m_exo.id_original() : m_exo.id());
    text_cb.set_num(m_exo.version());
}

flag edit_exercice_node::set_data(int cible, const QVariant & value, int role, numt num) {
    switch (cible) {
    case exercice_model::Source_Cible:
//
        break;
    case exercice_model::Texte_Cible:
        if(role == String_Role){
            m_texte.set_texte(value.toString());
            return String_Role;
        }
        break;
    case exercice_model::Type_Cible:
        if(role == mps::model_base::Variant_Role) {
            m_exo.set_type(value.toUInt());
            return Variant_Role;
        }
        break;
    case exercice_model::Version_Cible:
        if(role == mps::model_base::Variant_Role) {
            m_exo.set_version(value.toInt());
            return Variant_Role;
        }
        break;
    }
    return exercice_node::set_data(cible, value, role, num);
}
///////////////////////////////////////////////////////// edit_exercice_root /////////////////////////////////////////////
exercice_root::exercice_root(exercice_model *model, idt exo_id, int type)
    : exercice_node (model,exo_id,type) {
    if(m_exo.id()) {
        texte_cible text_cb;
        text_cb.set_cible(m_model->bdd().cible<exercice>());
        text_cb.set_id_cible(m_exo.id_original() ? m_exo.id_original() : m_exo.id());
        text_cb.set_num(m_exo.version());
        text_cb.set_type(m_model->bdd().ref_to_id<note_mps::type>("titre_txt"));
        m_model->bdd().get_unique(text_cb);
    }
}

flag edit_exercice_root::set_data(int cible, const QVariant & value, int role, numt num) {
    if (cible == exercice_model::Titre_Cible && role == mps::model_base::String_Role) {
        m_titre.set_texte(value.toString());
        return String_Role;
    }
    return edit_exercice_node::set_data(cible, value, role, num);
}
