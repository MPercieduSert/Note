#include "ManagersNote.h"


ManagerNote::ManagerNote()
    : ManagersPredef (InfoEntity::NbrEntityId,"InventaireVersion")
{
    enableRestrictionModification("RestModif");
    enableCommentaire("Commentaire","CbCommentaire");
    enableDonnee("Donnee","ArbDonnee","CbDonnee","CardDonnee",
                 new GestionAutorisationCible<Donnee,RestrictionModification>(0,get<RestrictionModification>()),
                 new GestionAutorisationCible<CibleDonnee,RestrictionModification>(0,get<RestrictionModification>()),
                 new GestionAutorisationCible<DonneeCard,RestrictionModification>(0,get<RestrictionModification>()));
    enableHistorique("Historique");
    enableMotCle("MotCle","ArbMotCle","CbMotCle","PermMotCle",
                 new GestionAutorisationCible<MotCle,RestrictionModification>(0,get<RestrictionModification>()),
                 new GestionAutorisationCible<MotClePermission,RestrictionModification>(0,get<RestrictionModification>()));
    enableType("Type","PermType",
               new GestionAutorisationCible<Type,RestrictionModification>(0,get<RestrictionModification>()),
               new GestionAutorisationCible<TypePermission,RestrictionModification>(0,get<RestrictionModification>()));
}
