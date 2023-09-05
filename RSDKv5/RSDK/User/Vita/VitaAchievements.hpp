#include <vector>
#include <string>

#if RETRO_REV02
struct VitaAchievements : UserAchievements {
    VitaAchievements() {}

    void TryUnlockAchievement(AchievementID *id);

#if RETRO_VER_EGS

#endif
};

int TrophyInit(void);
#endif
