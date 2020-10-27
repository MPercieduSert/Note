#include "ExerciceTab.h"

using namespace noteMPS;

ExerciceTab::ExerciceTab(BddNote & bdd, std::pair<int,int> pair, QWidget * parent)
: AbstractTabModuleWithBdd(bdd,pair,parent) {

}

void ExerciceTab::becomeCurrent()
    {emit actionPermise(fenMPS::EffacerAction|fenMPS::SauverAction);}
