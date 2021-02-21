#include "ExerciceModel.h"

using namespace modelMPS;
using namespace noteMPS;

///////////////////////////////////////// ExerciceEditModel
ExerciceEditModel::ExerciceEditModel(idt idRacineExo, BddNote & bdd, QObject * parent)
    : ItemNodeBddModel(bdd,parent) {
    if(idRacineExo == IdNew){
        m_data.setRoot(std::make_unique<ExerciceEditNode>(this));
        m_data.push_back(index(NodeIndex(),0), std::make_unique<ExerciceEditNode>(this));
    }
    else {

    }
}

numt ExerciceEditModel::dataCount(const NodeIndex & index) const {
    switch (index.cible()) {
    case ExerciceEditModel::SourceCible:
    case ExerciceEditModel::texte_cible:
    case ExerciceEditModel::TitreCible:
    case ExerciceEditModel::VersionCible:
        return 1;
    case SubNodeCible:
        return NbrCible;
    }
    return ItemNodeModel::dataCount(index);
}

modelMPS::Node ExerciceEditModel::nodeFactory(const modelMPS::NodeIndex & /*parent*/, numt /*pos*/, int /*type*/)
    {return std::make_unique<ExerciceEditNode>(this);}

////////////////////////////////////////////////// ExerciceNode /////////////////////////////////
QVariant ExerciceNode::data(int cible, int role, numt num) const {
    switch (cible) {
    case ExerciceEditModel::SourceCible:
        switch (role) {
        case IntRole:
            return m_idScr;
        case LabelRole:
            return "Source :";
        case OrientationRole:
            return Qt::Horizontal;
        case ListOfValues:
            QMap<QString,QVariant> map;
            auto vecScr = m_model->bdd().getList<Source>();
            for (auto iter = vecScr.cbegin(); iter != vecScr.cend(); ++iter)
                map.insert(QString(iter->nom()).append(" (").append(iter->nc()).append(")"),iter->id());
            return map;
        }
        break;
    case ExerciceEditModel::texte_cible:
        if(role == StringRole)
            return m_texte;
        break;
    case ExerciceEditModel::TitreCible:
        switch (role) {
        case OrientationRole:
            return Qt::Horizontal;
        case LabelRole:
            return "Titre :";
        case StringRole:
            return m_titre;
        }
        break;
    case ExerciceEditModel::VersionCible:
        if(role == NumRole)
            return m_exo.version();
        break;
    case modelMPS::SubNodeCible:
        if(role == SubNodeRole) {
            QList<QVariant> init;
            switch (num) {
            case ExerciceEditModel::Sourceposition:
                init.append(ExerciceEditModel::SourceCible);
                init.append(0);
                init.append(ComboBoxSubNode);
                return init;
            case ExerciceEditModel::Texteposition:
                init.append(ExerciceEditModel::texte_cible);
                init.append(0);
                init.append(TexteEditSubNode);
                return init;
            case ExerciceEditModel::Titreposition:
                init.append(ExerciceEditModel::TitreCible);
                init.append(0);
                init.append(LineEditSubNode);
                return init;
            case ExerciceEditModel::Versionposition:
                init.append(ExerciceEditModel::VersionCible);
                init.append(0);
                init.append(LineEditSubNode);
                return init;
            }
        }
        break;
    case modelMPS::NodeCible:
        if(role == OrientationRole)
            return Qt::Vertical;
    }
    return ItemBddNode::data(cible, role, num);
}

flag ExerciceNode::flags(int cible, numt num) const {
    if(m_iter.root())
        return NoFlagNode;
    return ItemBddNode::flags(cible,num);
}

void ExerciceNode::insert(bddMPS::Bdd & bdd) {
    idt idParent = 0;
    if(!m_iter.root())
        idParent = static_cast<const ExerciceNode &>(**m_iter.parent()).idExo();
    bdd.insert(m_exo,idParent,m_iter.position());
}

flag ExerciceNode::set_data(int cible, const QVariant & value, int role, numt num) {
    switch (role) {
    case ExerciceEditModel::SourceCible:
        if(role == IntRole){
            m_idScr = value.toUInt();
            return IntRole;
        }
        break;
    case ExerciceEditModel::texte_cible:
        if(role == StringRole){
            m_texte = value.to_string();
            return StringRole;
        }
        break;
    case ExerciceEditModel::TitreCible:
        if(role == modelMPS::StringRole) {
            m_titre = value.to_string();
            return StringRole;
        }
        break;
    case ExerciceEditModel::VersionCible:
        if(role == modelMPS::NumRole) {
            m_exo.setVersion(value.toInt());
            return NumRole;
        }
    }
    return ItemBddNode::set_data(cible, value, role, num);
}
