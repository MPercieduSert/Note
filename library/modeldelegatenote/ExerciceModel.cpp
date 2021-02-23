#include "ExerciceModel.h"

using namespace model_base;
using namespace noteMPS;

///////////////////////////////////////// ExerciceEditModel
ExerciceEditModel::ExerciceEditModel(idt idRacineExo, BddNote & bdd, QObject * parent)
    : item_node_bdd_model(bdd,parent) {
    if(idRacineExo == IdNew){
        m_data.set_root(std::make_unique<ExerciceEditnode_ptr>(this));
        m_data.push_back(index(node_index(),0), std::make_unique<ExerciceEditnode_ptr>(this));
    }
    else {

    }
}

numt ExerciceEditModel::data_count(const node_index & index) const {
    switch (index.cible()) {
    case ExerciceEditModel::SourceCible:
    case ExerciceEditModel::texte_cible:
    case ExerciceEditModel::TitreCible:
    case ExerciceEditModel::VersionCible:
        return 1;
    case Sub_Node_Cible:
        return NbrCible;
    }
    return item_node_model::data_count(index);
}

model_base::node_ptr ExerciceEditModel::node_factory(const model_base::node_index & /*parent*/, numt /*pos*/, int /*type*/)
    {return std::make_unique<ExerciceEditnode_ptr>(this);}

////////////////////////////////////////////////// Exercicenode_ptr /////////////////////////////////
QVariant Exercicenode_ptr::data(int cible, int role, numt num) const {
    switch (cible) {
    case ExerciceEditModel::SourceCible:
        switch (role) {
        case Int_Role:
            return m_idScr;
        case Label_Role:
            return "Source :";
        case Orientation_Role:
            return Qt::Horizontal;
        case List_Of_Values:
            QMap<QString,QVariant> map;
            auto vecScr = m_model->bdd().get_list<Source>();
            for (auto iter = vecScr.cbegin(); iter != vecScr.cend(); ++iter)
                map.insert(QString(iter->nom()).append(" (").append(iter->nc()).append(")"),iter->id());
            return map;
        }
        break;
    case ExerciceEditModel::texte_cible:
        if(role == String_Role)
            return m_texte;
        break;
    case ExerciceEditModel::TitreCible:
        switch (role) {
        case Orientation_Role:
            return Qt::Horizontal;
        case Label_Role:
            return "Titre :";
        case String_Role:
            return m_titre;
        }
        break;
    case ExerciceEditModel::VersionCible:
        if(role == Num_Role)
            return m_exo.version();
        break;
    case model_base::Sub_Node_Cible:
        if(role == Sub_Node_Role) {
            QList<QVariant> init;
            switch (num) {
            case ExerciceEditModel::Source_Position:
                init.append(ExerciceEditModel::SourceCible);
                init.append(0);
                init.append(Combo_Box_Sub_Node);
                return init;
            case ExerciceEditModel::Texte_Position:
                init.append(ExerciceEditModel::texte_cible);
                init.append(0);
                init.append(Texte_Edit_Sub_Node);
                return init;
            case ExerciceEditModel::Titre_Position:
                init.append(ExerciceEditModel::TitreCible);
                init.append(0);
                init.append(Line_Edit_Sub_Node);
                return init;
            case ExerciceEditModel::Versionposition:
                init.append(ExerciceEditModel::VersionCible);
                init.append(0);
                init.append(Line_Edit_Sub_Node);
                return init;
            }
        }
        break;
    case model_base::Node_Cible:
        if(role == Orientation_Role)
            return Qt::Vertical;
    }
    return item_bdd_node::data(cible, role, num);
}

flag Exercicenode_ptr::flags(int cible, numt num) const {
    if(m_iter.root())
        return No_Flag_Node;
    return item_bdd_node::flags(cible,num);
}

void Exercicenode_ptr::insert(b2d::Bdd & bdd) {
    idt id_parent = 0;
    if(!m_iter.root())
        id_parent = static_cast<const Exercicenode_ptr &>(**m_iter.parent()).idExo();
    bdd.insert(m_exo,id_parent,m_iter.position());
}

flag Exercicenode_ptr::set_data(int cible, const QVariant & value, int role, numt num) {
    switch (role) {
    case ExerciceEditModel::SourceCible:
        if(role == Int_Role){
            m_idScr = value.toUInt();
            return Int_Role;
        }
        break;
    case ExerciceEditModel::texte_cible:
        if(role == String_Role){
            m_texte = value.to_string();
            return String_Role;
        }
        break;
    case ExerciceEditModel::TitreCible:
        if(role == model_base::String_Role) {
            m_titre = value.to_string();
            return String_Role;
        }
        break;
    case ExerciceEditModel::VersionCible:
        if(role == model_base::Num_Role) {
            m_exo.setVersion(value.toInt());
            return Num_Role;
        }
    }
    return item_bdd_node::set_data(cible, value, role, num);
}
