#include "3DItemDrops.h"
#include "libultraship/libultraship.h"
#include "2s2h/Enhancements/GameInteractor/GameInteractor.h"

extern "C" {
#include "z64.h"
#include "functions.h"
extern PlayState* gPlayState;
}

void Register3DItemDrops() {
    GameInteractor::Instance->RegisterGameHook<GameInteractor::On3DSpinItemdrops>(
        [](EnItem00* actor, PlayState* play) {
                if (CVarGetInteger("gEnhancements.Graphics.Item3D", 0) && ((actor->actor.params >= ITEM00_ARROWS_30 && actor->actor.params < ITEM00_MASK) || actor->actor.params == ITEM00_BOMBS_A || actor->actor.params == ITEM00_ARROWS_10 || actor->actor.params == ITEM00_SMALL_KEY || actor->actor.params == ITEM00_DEKU_NUTS_10 || actor->actor.params == ITEM00_BOMBS_0))
                    actor->actor.shape.rot.y += 960;
                else actor->actor.shape.rot.y = 0;
        }
    );
