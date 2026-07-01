#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

static bool isPlayerDeadAndFrozen = false;
static int deadTapCount = 0;
static bool showingHitboxes = false;

class $modify(PlayLayer) {
    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        PlayLayer::destroyPlayer(p0, p1);
        isPlayerDeadAndFrozen = true;
        deadTapCount = 0;
        showingHitboxes = false;
        this->pauseSchedulerAndActions();
    }

    void pushButton(PlayerButton p0, bool p1) {
        if (isPlayerDeadAndFrozen) {
            if (p0 == PlayerButton::Jump) {
                deadTapCount++;
                if (deadTapCount == 2 && !showingHitboxes) {
                    showingHitboxes = true;
                    deadTapCount = 0;
                    return;
                }
                if (deadTapCount == 2 && showingHitboxes) {
                    isPlayerDeadAndFrozen = false;
                    showingHitboxes = false;
                    this->resumeSchedulerAndActions();
                    this->resetLevel();
                    return;
                }
            }
            return;
        }
        PlayLayer::pushButton(p0, p1);
    }
};
