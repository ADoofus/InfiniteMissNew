//original's first mod 
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/utils/logging.hpp"
#include "../extern/beatsaber-hook/include/modloader.hpp"
#include "../extern/beatsaber-hook/shared/utils/typedefs.h"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"


using namespace il2cpp_utils;
static ModInfo modInfo;

const Logger& getLogger() {
  static const Logger& logger(modInfo);
  return logger;
}

MAKE_HOOK_OFFSETLESS(MissedNoteEffect_Init, void, Il2CppObject* self, Il2CppObject* noteData, float animationDuration, float startAnimationTime) {
    
    MissedNoteEffect_Init(self, noteData, 99999.0f, startAnimationTime);

}


extern "C" void load(ModInfo &info) {
    getLogger().debug("Installing InfiniteMiss!");

    INSTALL_HOOK_OFFSETLESS(MissedNoteEffect_Init, FindMethodUnsafe("", "MissedNoteEffect", "Init", 3));

    getLogger().debug("Installed InfiniteMiss!");

}