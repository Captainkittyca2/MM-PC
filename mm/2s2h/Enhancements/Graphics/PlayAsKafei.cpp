#include "PlayAsKafei.h"
#include "libultraship/libultraship.h"
#include "2s2h/Enhancements/GameInteractor/GameInteractor.h"

extern "C" {
#include "z64.h"
#include "z64save.h"
#include "functions.h"
extern PlayState* gPlayState;
extern SaveContext gSaveContext;
extern u8 gItemSlots[77];
#include "objects/object_link_child/object_link_child.h"
#include "objects/object_test3/object_test3.h"
void ResourceMgr_PatchGfxByName(const char* path, const char* patchName, int index, Gfx instruction);
void ResourceMgr_UnpatchGfxByName(const char* path, const char* patchName);
extern TexturePtr sPlayerEyesTextures[PLAYER_FORM_MAX][PLAYER_EYES_MAX];
extern TexturePtr sPlayerMouthTextures[PLAYER_FORM_MAX][PLAYER_MOUTH_MAX];
}

static SkeletonHeader gLinkHumanSkelBackup;
static SkeletonHeader gKafeiSkelBackup;

void UpdatePlayAsKafeiSkeletons() {
    if (CVarGetInteger("gModes.PlayAsKafei", 0)) {
        auto gLinkHumanSkelResource = Ship::Context::GetInstance()->GetResourceManager()->LoadResource(gLinkHumanSkel);
        SkeletonHeader* gLinkHumanSkelPtr = (SkeletonHeader*)gLinkHumanSkelResource->GetRawPointer();
        memcpy(gLinkHumanSkelPtr, &gKafeiSkelBackup, sizeof(SkeletonHeader));
    } else {
        auto gLinkHumanSkelResource = Ship::Context::GetInstance()->GetResourceManager()->LoadResource(gLinkHumanSkel);
        SkeletonHeader* gLinkHumanSkelPtr = (SkeletonHeader*)gLinkHumanSkelResource->GetRawPointer();
        memcpy(gLinkHumanSkelPtr, &gLinkHumanSkelBackup, sizeof(SkeletonHeader));
    }
}

void UpdatePlayAsKafeiOther() {
    if (CVarGetInteger("gModes.PlayAsKafei", 0)) {
        ResourceMgr_PatchGfxByName(gLinkHumanWaistDL, "gLinkHumanWaistDL0", 0,
                                   gsSPDisplayListOTRFilePath(gKafeiWaistDL));
        ResourceMgr_PatchGfxByName(gLinkHumanWaistDL, "gLinkHumanWaistDL1", 1, gsSPEndDisplayList());

        sPlayerEyesTextures[PLAYER_FORM_HUMAN][0] = (TexturePtr)gKafeiEyesOpenTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][1] = (TexturePtr)gKafeiEyesHalfTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][2] = (TexturePtr)gKafeiEyesClosedTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][3] = (TexturePtr)gKafeiEyesRollRightTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][4] = (TexturePtr)gKafeiEyesRollLeftTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][5] = (TexturePtr)gKafeiEyesRollUpTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][6] = (TexturePtr)gKafeiEyesRollDownTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][7] = (TexturePtr)object_test3_Tex_006680;

        sPlayerMouthTextures[PLAYER_FORM_HUMAN][0] = (TexturePtr)gKafeiMouthClosedTex;
        sPlayerMouthTextures[PLAYER_FORM_HUMAN][1] = (TexturePtr)gKafeiMouthTeethTex;
        sPlayerMouthTextures[PLAYER_FORM_HUMAN][2] = (TexturePtr)gKafeiMouthAngryTex;
        sPlayerMouthTextures[PLAYER_FORM_HUMAN][3] = (TexturePtr)gKafeiMouthHappyTex;
    } else {
        ResourceMgr_UnpatchGfxByName(gLinkHumanWaistDL, "gLinkHumanWaistDL0");
        ResourceMgr_UnpatchGfxByName(gLinkHumanWaistDL, "gLinkHumanWaistDL1");

        sPlayerEyesTextures[PLAYER_FORM_HUMAN][0] = (TexturePtr)gLinkHumanEyesOpenTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][1] = (TexturePtr)gLinkHumanEyesHalfTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][2] = (TexturePtr)gLinkHumanEyesClosedTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][3] = (TexturePtr)gLinkHumanEyesRollRightTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][4] = (TexturePtr)gLinkHumanEyesRollLeftTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][5] = (TexturePtr)gLinkHumanEyesRollUpTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][6] = (TexturePtr)gLinkHumanEyesRollDownTex;
        sPlayerEyesTextures[PLAYER_FORM_HUMAN][7] = (TexturePtr)object_link_child_Tex_003800;

        sPlayerMouthTextures[PLAYER_FORM_HUMAN][0] = (TexturePtr)gLinkHumanMouthClosedTex;
        sPlayerMouthTextures[PLAYER_FORM_HUMAN][1] = (TexturePtr)gLinkHumanMouthTeethTex;
        sPlayerMouthTextures[PLAYER_FORM_HUMAN][2] = (TexturePtr)gLinkHumanMouthAngryTex;
        sPlayerMouthTextures[PLAYER_FORM_HUMAN][3] = (TexturePtr)gLinkHumanMouthHappyTex;
    }
}

void RegisterPlayAsKafei() {
    auto gLinkHumanSkelResource = Ship::Context::GetInstance()->GetResourceManager()->LoadResource(gLinkHumanSkel);
    auto gKafeiSkelResource = Ship::Context::GetInstance()->GetResourceManager()->LoadResource(gKafeiSkel);

    SkeletonHeader* gLinkHumanSkelPtr = (SkeletonHeader*)gLinkHumanSkelResource->GetRawPointer();
    SkeletonHeader* gKafeiSkelPtr = (SkeletonHeader*)gKafeiSkelResource->GetRawPointer();

    memcpy(&gLinkHumanSkelBackup, gLinkHumanSkelPtr, sizeof(SkeletonHeader));
    memcpy(&gKafeiSkelBackup, gKafeiSkelPtr, sizeof(SkeletonHeader));

    UpdatePlayAsKafeiSkeletons();
    UpdatePlayAsKafeiOther();

    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnSceneInit>(
        [](s8 sceneId, s8 spawnNum) { UpdatePlayAsKafeiOther(); });
}

u8 itemUsage;
bool meterr;
u8 timerrr;
s16 timerrlimitt = 0;

void RegisterALBWMeter() {
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnPlayerUpdate>([]() {
        if (CVarGetInteger("gModes.ALBWMeter", 0)) {
            static u8 framesSinceMeter = 0;
            static int8_t temp_ammo = gSaveContext.save.saveInfo.playerData.magic;
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
