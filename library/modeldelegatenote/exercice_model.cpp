#include "exercice_model.h"

using namespace mps::model_base;
using namespace note_mps;
using namespace model_exo;

///////////////////////////////////////////////////////// exercice_model /////////////////////////////////////////////////
exercice_model::exercice_model(idt id_racine_exo, bdd_note & bd, QObject *parent)
    : item_node_note_model(bd,parent), m_id_root_exo(id_racine_exo),
      m_type_texte({bdd().ref_to_id<note_mps::type>("main_txt"),
                   bdd().ref_to_id<note_mps::type>("titre_txt"),
                   bdd().ref_to_id<note_mps::type>("correction_txt")}){
    m_num_to_string.insert({Version_Style_Num, mps::divers::num_to_string::Arabe_Suivant_Style});
    auto arb_type = bdd().get_arbre<note_mps::type>(bdd().ref_to_id<note_mps::type>("exo_root"));
    for (auto it = arb_type.cbegin().to_first().to_first(); it; ++it)
        m_num_to_string.insert({it->id(),bdd().donnee_ref("style_num",*it).toUInt()});
}
///////////////////////////////////////////////////////// find_exercice_model ////////////////////////////////////////////
std::list<node_iter> find_exercice_model::insert(const node_index &parent, numt pos, numt count, int /*type*/) {
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
            for (auto it = roots.cbegin(); it != roots.cend(); ++it){
                auto node_it = m_data.push_back(root_index,std::make_unique<find_exercice_root>(this,*it));
                exercice exo(*it);
                bdd().get(exo);
                auto arb_id_exo = bdd().get_id_arbre<exercice>(bdd().get_id_child<exercice>(exo.id(),exo.version()));
                auto id_it = arb_id_exo.cbegin();
                while (id_it) {
                    if(!id_it.leaf())
                        for (auto child_it = id_it.cbegin_child(); child_it; ++child_it)
                            m_data.push_back(node_it,std::make_unique<find_exercice_node>(this,*child_it));
                    ++id_it;
                    ++node_it;
                }
            }
        end_reset_model();
    }
    return std::list<node_iter>();
}
mps::model_base::node_ptr find_exercice_model::node_factory(const mps::model_base::node_index & parent,
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
        auto ind = index(parent,pos + count,En_Tete_Cible);
        while (ind.is_valid()) {
            emit data_changed(ind,Label_Change_Flag);
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
            emit data_changed(ind,Setting_Change_Flag);
            ind.to_parent();
        }
    }
    else if(pos == 0 && count == 1){
        begin_reset_model();
            if(m_id_root_exo){
                m_data.set_tree(bdd().get_arbre<exercice>(m_id_root_exo),
                                [this](mps::tree<exercice>::const_brother_iterator it)->node_ptr{
                    if(!it.root() && it.parent().root())
                        return std::make_unique<edit_exercice_root>(this,it->id());
                    return std::make_unique<edit_exercice_node>(this,it->id());
                });
            }
            else {
                m_data.set_root(std::make_unique<edit_exercice_node>(this));
                static_cast<exercice_node &>(**m_data.cbegin()).update_type();
                nodes.push_back(m_data.cbegin());
                auto it = m_data.push_back(index(node_index(),0), std::make_unique<edit_exercice_root>(this));
                static_cast<exercice_node &>(**it).update_type();
                nodes.push_back(it);
            }
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
            if(index.cible() == Type_Cible && changeRole & Main_Data_Change_Flag) {
                emit data_changed(index.index(Node_Cible),Flag_Change_Flag);
                emit data_changed(index.index(En_Tete_Cible),Label_Change_Flag);
                if(!index.leaf()){
                    auto ind = index.first();
                    auto id_parent = index.data(Variant_Role).toUInt();
                    while(ind.is_valid()) {
                        emit data_changed(ind,Setting_Change_Flag);
                        emit data_changed(ind.index(En_Tete_Cible),Label_Change_Flag);
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
        emit data_changed(ind,Setting_Change_Flag);
        ind.to_parent();
    }
    return bb;
}

///////////////////////////////////////////////////////// exercice_node //////////////////////////////////////////////////
QVariant exercice_node::data(int cible, int role, numt num) const {
    switch (cible) {
    case Source_Cible:
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
    case Texte_Cible:
        switch (role) {
        case Label_Role: {
            note_mps::type tp(m_exo.type());
            if(m_model->bdd().get(tp)){
                auto str = tp.nc();
                if(m_iter.profondeur() != 1)
                    str.append(" ").append(m_model->num_to_string(static_cast<int>(m_iter.position()),m_exo.type()));
                str.append(" :");
                return str;
            }
            else
                return QString();
            }
        case String_Role:
            return m_texte[Main_Texte].texte();
        case Type_Role:
            return Texte_Edit_Sub_Node;
        }
        break;
    case Type_Cible:
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
    case Version_Cible:
        switch (role) {
        case Label_Role:
            return "Version :";
        case Variant_Role:
            return m_exo.version();
        case Map_Role: {
            auto sz = m_model->bdd().fonction_agrega<texte_cible>(mps::b2d::Max, texte_cible::Num,
                                            texte_cible::Cible, m_model->bdd().cible(exercice::ID),
                                            texte_cible::Id_Cible, m_exo.id_original() ? m_exo.id_original() : m_exo.id());
            QMap<QString,QVariant> map;
            if(sz.isNull())
                return map;
            auto n = sz.toInt() + 1;
            for (auto i = 0; i != n; ++i)
                map.insert(m_model->num_to_string(i),i);
            return map;}
        case Orientation_Role:
            return Qt::Horizontal;
        case Type_Role:
            return Combo_Box_Sub_Node;
        }
        break;
    case mps::model_base::Node_Cible:
        if(num == Node_Num) {
            switch (role) {
            case Id_Role:
                return m_exo.id();
            case Cibles_Role:
                return QVariantList({Texte_Cible,Version_Cible});
            case Orientation_Role:
                return Qt::Vertical;
            }
        }
        break;
    }
    return item_bdd_node::data(cible, role, num);
}

flag exercice_node::flags(int cible, numt num) const {
    if(cible == Node_Cible && m_iter.root())
        return No_Flag_Node;
    return item_bdd_node::flags(cible,num);
}

void exercice_node::set_id_exo(idt id) {
    if(id) {
        m_exo.set_id(id);
        if(m_model->bdd().get(m_exo)){
            texte_cible text_cb;
            text_cb.set_cible(m_model->bdd().cible<exercice>());
            text_cb.set_id_cible(m_exo.id_original() ? m_exo.id_original() : m_exo.id());
            text_cb.set_num(m_exo.version());
            for (enumt tp_txt = 0; tp_txt != Nbr_Type_Texte; ++tp_txt) {
                text_cb.set_id(0);
                text_cb.set_type(m_model->type_texte(tp_txt));
                m_model->bdd().get_unique(text_cb);
                m_texte[tp_txt].set_id(text_cb.id_texte());
                if(m_model->bdd().get(m_texte[tp_txt])) {
                    auto src_txt = m_model->bdd().get_list<source_texte>(source_texte::Id_Texte,
                                                                            m_texte[tp_txt].id(),
                                                                            source_texte::Id_Source);
                    m_source[tp_txt].clear();
                    for (auto it = src_txt.cbegin(); it != src_txt.cend(); ++it)
                        m_source[tp_txt].insert(it->id_source());
                }
            }
        }
        else {
            m_exo.set_id(0);
        }
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
///////////////////////////////////////////////////////// read_exercice_node /////////////////////////////////////////////
flag read_exercice_node::flags(int cible, numt /*num*/) const {
    if(m_iter.root())
        return No_Flag_Node;
    if(cible == Node_Cible)
        return Default_Flag_Node | Expendable_FLag_Node;
    return Visible_Flag_Node;
}
///////////////////////////////////////////////////////// read_exercice_root /////////////////////////////////////////////
flag read_exercice_root::flags(int cible, numt num) const {
    return read_exercice_node::flags(cible,num);
}
///////////////////////////////////////////////////////// find_exercice_node /////////////////////////////////////////////
QVariant find_exercice_node::data(int cible, int role, numt num) const {
    return exercice_node::data(cible,role,num);
}

flag find_exercice_node::flags(int cible, numt num) const {
    auto fl = exercice_node::flags(cible,num);
    if(fl) {
        switch (cible) {
        case Node_Cible:
            fl ^= Elder_Enable_Flag_Node | Brother_Enable_Flag_Node | Del_Enable_Flag_Node;
            break;
        case Source_Cible:
        case Texte_Cible:
            fl ^= Enable_Flag_Node;
        }
    }
    return fl;
}
///////////////////////////////////////////////////////// find_exercice_root /////////////////////////////////////////////
QVariant find_exercice_root::data(int cible, int role, numt num) const {
    switch (cible) {
    case Titre_Cible:
        switch (role) {
        case Label_Role: {
            note_mps::type tp(m_root_exo.type());
            if(m_model->bdd().get(tp))
                return QString(tp.nc()).append(" : ")
                        .append(m_texte[Titre_Texte].texte());
            else
                return QString();
            }
        case Type_Role:
            return Label_Sub_Node;
        }
        break;
    case Version_Exercice_Cible:
        switch (role) {
        case Variant_Role:
            return m_root_exo.version();
        case Orientation_Role:
            return Qt::Horizontal;
        case Label_Role:
            return "Version de l'exercice : ";
        case Map_Role: {
            auto sz = m_model->bdd().size_child<exercice>(m_root_exo.id());
            QMap<QString,QVariant> map;
            for (auto i = 0; i != sz; ++i)
                map.insert(m_model->num_to_string(i),i);
            return map;}
        case Type_Role:
            return Combo_Box_Sub_Node;
        }
        break;
    case Node_Cible:
        if (num == Node_Num && role == Cibles_Role)
            return QVariantList({Version_Exercice_Cible,Titre_Cible}) << find_exercice_node::data(cible,role,num).toList();
        break;
    }
    return find_exercice_node::data(cible,role,num);
}

flag find_exercice_root::flags(int cible, numt num) const {
    return find_exercice_node::flags(cible,num);
}

void find_exercice_root::set_id_exo(idt id) {
    if(id) {
        m_root_exo.set_id(id);
        if(m_model->bdd().get(m_root_exo)){
            auto id_exo = m_model->bdd().get_id_child<exercice>(m_root_exo.id(),m_root_exo.version());
            exercice_node::set_id_exo(id_exo);
        }
    }
}
///////////////////////////////////////////////////////// edit_exercice_node /////////////////////////////////////////////
QVariant edit_exercice_node::data(int cible, int role, numt num) const {
    switch (cible) {
    case Type_Cible:
        switch (role) {
        case Variant_Role:
            return m_exo.type();
        case Orientation_Role:
            return Qt::Horizontal;
        case Label_Role:
            return "Type :";
        case Map_Role: {
            auto id_type = m_iter.root() ? 0 : static_cast<exercice_node &>(**m_iter.parent()).type_exo();
            auto arb = m_model->bdd().get_arbre<note_mps::type>(
                        id_type ? id_type : m_model->bdd().ref_to_id<note_mps::type>("exo_root"));
            QMap<QString,QVariant> map;
            for (auto it = arb.cbegin().cbegin_child(); it; ++it)
                if(m_iter.hauteur() <= it.hauteur())
                    map.insert(it->nc(),it->id());
            return map;}
        case Type_Role:
            return Combo_Box_Sub_Node;
        }
        break;
    case Version_Cible:
        if (role == Map_Role) {
             auto map = exercice_node::data(cible,role,num).toMap();
             auto n = map.count();
             map.insert(m_model->num_to_string(n).append(" (nouvelle)"),n);
             return map;
        }
        break;
    case mps::model_base::Node_Cible:
        if(role == Cibles_Role && num == Node_Num)
            return QVariantList({Type_Cible}) << exercice_node::data(cible,role,num).toList();
        break;
    }
    return exercice_node::data(cible, role, num);
}
flag edit_exercice_node::flags(int cible, numt num) const {
    if(cible == Type_Cible && !m_iter.root()
            && m_model->bdd().fonction_agrega<note_mps::type>(mps::b2d::Nombre, type::Id,
                                              note_mps::type::Parent,static_cast<exercice_node &>(**m_iter.parent()).type_exo())
            .toInt() <= 1)
        return No_Flag_Node;
    auto fl = exercice_node::flags(cible,num);
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
    save_textes(bdd);
}

void edit_exercice_node::save_textes(mps::b2d::bdd &bdd) {
    texte_cible txt_cb;
    txt_cb.set_cible(m_model->bdd().cible<exercice>());
    txt_cb.set_id_cible(m_exo.id_original() ? m_exo.id_original() : m_exo.id());
    txt_cb.set_num(m_exo.version());
    for (enumt tp_txt = 0; tp_txt != Nbr_Type_Texte; ++tp_txt){
        txt_cb.set_id(0);
        if(m_texte[tp_txt].is_new()) {
            if(!m_texte[tp_txt].texte().isEmpty()){
                if(!bdd.get_unique(m_texte[tp_txt]))
                    bdd.save(m_texte[tp_txt]);
                txt_cb.set_id_texte(m_texte[tp_txt].id());
                txt_cb.set_type(m_model->type_texte(tp_txt));
                bdd.save_unique(txt_cb);
                for (auto it = m_source[tp_txt].cbegin(); it != m_source[tp_txt].cend(); ++it) {
                    source_texte src_txt;
                    src_txt.set_id_texte(m_texte[tp_txt].id());
                    src_txt.set_id_source(*it);
                    bdd.save_unique(src_txt);
                }
            }
        }
        else {
            if(!bdd.same_in_bdd(m_texte[tp_txt])) {
                auto pair = bdd.exists_unique_id(m_texte[tp_txt]);
                if(pair.first == mps::b2d::Aucun)
                    bdd.save(m_texte[tp_txt]);
                else if (pair.first == mps::b2d::Autre) {
                    if(bdd.fonction_agrega<texte_cible>(mps::b2d::Nombre,texte_cible::Id,
                                                        texte_cible::Id_Texte,m_texte[tp_txt].id()).toInt()
                            == 1)
                        bdd.del(m_texte[tp_txt]);
                    txt_cb.set_type(m_model->type_texte(tp_txt));
                    txt_cb.set_id_texte(pair.second);
                    bdd.save_unique(txt_cb);
                }
            }
            auto sources_bdd = bdd.get_list<source_texte>(source_texte::Id_Texte,m_texte[tp_txt].id(),source_texte::Id_Source);
            auto src_bdd_it = sources_bdd.cbegin();
            auto src_it = m_source[tp_txt].cbegin();
            while (src_bdd_it != sources_bdd.cend() && src_it != m_source[tp_txt].cend()) {
                if(src_bdd_it->id_source() == *src_it){
                    ++src_it;
                    ++src_bdd_it;
                }
                else if (src_bdd_it->id_source() < *src_it) {
                    bdd.del(*src_bdd_it);
                    ++src_bdd_it;
                }
                else {
                    source_texte src_txt;
                    src_txt.set_id_texte(m_texte[tp_txt].id());
                    src_txt.set_id_source(*src_it);
                    bdd.save_unique(src_txt);
                }
            }
        }
    }
    m_model->emit_data_changed(*this,Version_Cible,Setting_Change_Flag);
}

flag edit_exercice_node::set_data(int cible, const QVariant & value, int role, numt num) {
    switch (cible) {
    case Source_Cible:
//
        break;
    case Texte_Cible:
        if(role == String_Role){
            m_texte[Main_Texte].set_texte(value.toString());
            return Main_Same_Change_Flag;
        }
        break;
    case Type_Cible:
        if(role == mps::model_base::Variant_Role) {
            m_exo.set_type(value.toUInt());
            return Main_Same_Change_Flag;
        }
        break;
    case Version_Cible:
        if(role == mps::model_base::Variant_Role) {
            m_exo.set_version(value.toInt());
            return Main_Same_Change_Flag;
        }
        break;
    }
    return exercice_node::set_data(cible, value, role, num);
}
///////////////////////////////////////////////////////// edit_exercice_root /////////////////////////////////////////////
QVariant edit_exercice_root::data(int cible, int role, numt num) const {
    switch (cible) {
    case Titre_Cible:
        switch (role) {
        case Label_Role:
            return "Titre :";
        case Orientation_Role:
            return Qt::Horizontal;
        case String_Role:
            return m_texte[Titre_Texte].texte();
        case Type_Role:
            return Line_Edit_Sub_Node;
        }
        break;
    case Version_Exercice_Cible:
        switch (role) {
        case Label_Role:
            return QString("Version %1 de l'exercice").arg(m_model->num_to_string(static_cast<int>(m_iter.position())));
        case Type_Role:
            return Label_Sub_Node;
        }
        break;
    case mps::model_base::Node_Cible:
        if(role == Cibles_Role && num == Node_Num)
            return QVariantList({Version_Exercice_Cible,Titre_Cible,Type_Cible}) << exercice_node::data(cible,role,num).toList();
        break;
    }
    return edit_exercice_node::data(cible,role,num);
}

flag edit_exercice_root::flags(int cible, numt num) const {
    if(cible == Type_Cible && !m_iter.root()
            && m_model->bdd().fonction_agrega<note_mps::type>(mps::b2d::Nombre, type::Id,
                                              note_mps::type::Parent,static_cast<exercice_node &>(**m_iter.parent()).type_exo())
            .toInt() <= 1)
        return No_Flag_Node;
    return edit_exercice_node::flags(cible,num);
}


flag edit_exercice_root::set_data(int cible, const QVariant & value, int role, numt num) {
    if (cible == Titre_Cible && role == mps::model_base::String_Role) {
        m_texte[Titre_Texte].set_texte(value.toString());
        return Main_Same_Change_Flag;
    }
    return edit_exercice_node::set_data(cible, value, role, num);
}

//case Titre_Cible:
//    if (role == Label_Role) {
//        note_mps::type tp(m_root_exo.id());
//        m_model->bdd().get(tp);
//        return QString(tp.nom()).append(" : ").append(m_texte[Titre_Texte].texte());
//    }
//    if (role == Type_Role)
//        return Label_Sub_Node;
//    return exercice_root::data(cible,role,num);
//case Type_Cible:
//    if(role == Map_Role) {
//        auto arb = m_model->bdd().get_arbre<note_mps::type>(m_root_exo.type());
//        QMap<QString,QVariant> map;
//        for (auto it = arb.cbegin().cbegin_child(); it; ++it)
//            if(m_iter.hauteur() <= it.hauteur())
//                map.insert(it->nc(),it->id());
//        return map;
//    }
//case Map_Role: {
//    auto arb = m_model->bdd().get_arbre<note_mps::type>(m_model->bdd().ref_to_id<note_mps::type>("exo_root"));
//    QMap<QString,QVariant> map;
//    for (auto it = arb.cbegin().cbegin_child(); it; ++it)
//        if(m_iter.hauteur() <= it.hauteur())
//            map.insert(it->nc(),it->id());
//    return map;}
//    break;
