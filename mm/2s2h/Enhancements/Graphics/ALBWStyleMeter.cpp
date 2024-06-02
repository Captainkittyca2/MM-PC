#include "ALBWStyleMeter.h"
#include "libultraship/libultraship.h"
#include "2s2h/Enhancements/GameInteractor/GameInteractor.h"

extern "C" {
#include "z64.h"
#include "z64save.h"
#include "functions.h"
extern PlayState* gPlayState;
extern SaveContext gSaveContext;
extern u8 gItemSlots[77];
}

s8 itemUsage;
bool meterr;
s8 timerrr;
s16 timerrlimitt = 0;

void RegisterALBWMeter() {
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnPlayerUpdate>([]() {
        if (CVarGetInteger("gModes.ALBWMeter", 0)) {
            static s8 framesSinceMeter = 0;
            static s8 temp_ammo = gSaveContext.save.saveInfo.playerData.magic;
            framesSinceMeter++;
            if (temp_ammo != gSaveContext.save.saveInfo.playerData.magic) {
                if (gSaveContext.save.saveInfo.playerData.magic < 0) {
                    timerrlimitt = gSaveContext.save.saveInfo.playerData.magic * 10;
                    gSaveContext.save.saveInfo.playerData.magic = 0;
                }
                meterr = true;
                timerrr = 0;
                temp_ammo = gSaveContext.save.saveInfo.playerData.magic;
            }
