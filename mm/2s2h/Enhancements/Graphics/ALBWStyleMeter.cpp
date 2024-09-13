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
            if (meterr == true) {
                timerrr++;
                if (timerrr == 30 + abs(timerrlimitt)) {
                    timerrr = 0;
                    timerrlimitt = 0;
                    meterr = false;
                }
            }
            if (framesSinceMeter > 10 && meterr == false) {
                framesSinceMeter = 0;
                if (gSaveContext.save.saveInfo.playerData.magic < gSaveContext.magicCapacity) {
                    gSaveContext.save.saveInfo.playerData.magic++;
                    temp_ammo++;
                } else if (gSaveContext.save.saveInfo.playerData.magic > gSaveContext.magicCapacity)
                    gSaveContext.save.saveInfo.playerData.magic = temp_ammo = gSaveContext.magicCapacity;
            }
            float newAmmoAmount = (10 * gSaveContext.magicCapacity / 48) *
                            (static_cast<float>(gSaveContext.save.saveInfo.playerData.magic) / gSaveContext.magicCapacity);
            if (timerrlimitt == 0 || gSaveContext.save.saveInfo.playerData.magic > 0) {
                if (floor(newAmmoAmount / 5) == 0)
                    AMMO(ITEM_DEKU_STICK) = 1;
                else
                    AMMO(ITEM_DEKU_STICK) = newAmmoAmount / 5 + 1;
                if (floor(newAmmoAmount / 5) == 0)
                    AMMO(ITEM_DEKU_NUT) = 1;
                else
                    AMMO(ITEM_DEKU_NUT) = newAmmoAmount / 5 + 1;
                if (floor(newAmmoAmount / 5) == 0)
                    AMMO(ITEM_BOMB) = 1;
                else
                    AMMO(ITEM_BOMB) = newAmmoAmount / 5 + 1;
                if (floor(newAmmoAmount / 2.5) == 0)
                    AMMO(ITEM_BOW) = 1;
                else
                    AMMO(ITEM_BOW) = newAmmoAmount / 2.5 + 1;
                if (floor(newAmmoAmount / 5) == 0)
                    AMMO(ITEM_BOMBCHU) = 1;
                else
                    AMMO(ITEM_BOMBCHU) = newAmmoAmount / 5 + 1;
            } else
                AMMO(ITEM_DEKU_STICK) = AMMO(ITEM_DEKU_NUT) = AMMO(ITEM_BOMB) = AMMO(ITEM_BOW) = AMMO(ITEM_BOMBCHU) = 0;
        }
    });
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnChangeAmmo>([](s16 item, s16 ammoChange) {
        if (CVarGetInteger("gModes.ALBWMeter", 0)) {
            if (item == ITEM_DEKU_STICK || item == ITEM_DEKU_NUT || item == ITEM_BOW || item == ITEM_BOMB ||
                item == ITEM_BOMBCHU) {
                if (item == ITEM_DEKU_NUT || item == ITEM_DEKU_STICK || item == ITEM_BOMB ||item == ITEM_BOMBCHU)
                    itemUsage = 24;
                else if (item == ITEM_BOW)
                    itemUsage = 12;
                gSaveContext.save.saveInfo.playerData.magic = gSaveContext.save.saveInfo.playerData.magic - itemUsage;
            }
        }
    });
}
