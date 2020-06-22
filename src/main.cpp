//original's first mod 
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/utils/typedefs.h"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"


using namespace il2cpp_utils;

MAKE_HOOK_OFFSETLESS(MissedNoteEffect_Init, void, Il2CppObject* self, Il2CppObject* noteData, float animationDuration, float startAnimationTime) {
    
    MissedNoteEffect_Init(self, noteData, 99999.0f, startAnimationTime);
    

}


extern "C" void load() {
    //log(INFO, "Hello from il2cpp_init!");
    //log(INFO, "Installing hooks...");

    INSTALL_HOOK_OFFSETLESS(MissedNoteEffect_Init, FindMethodUnsafe("", "MissedNoteEffect", "Init", 3));

    //log(INFO, "Installed all hooks!");
}