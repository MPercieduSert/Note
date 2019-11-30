#include "NoyauNote.h"

using namespace noteMPS;

//!Ouvre le gestionnaire de configuration au chemin indiquer.
void NoyauNote::setConfigByPath(const QString &configPath, QWidget *modalParent)
    {setConfig(new ConfigNote(configPath), modalParent);}
