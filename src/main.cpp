#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

bool isPlayerDeadAndFrozen = false;
int deadTapCount = 0;
bool showingHitboxes = false;

class $modify(MyPlayLayer, PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* obstacle) {
        PlayLayer::destroyPlayer(player, obstacle);
        
        isPlayerDeadAndFrozen = true;
        deadTapCount = 0;
        showingHitboxes = false;
        
        this->pauseSchedulerAndActions();
    }

    void pushButton(PlayerButton btn, bool isPlayer2) {
        if (isPlayerDeadAndFrozen) {
            if (btn == PlayerButton::Jump) {
                deadTapCount++;

                if (deadTapCount == 2 && !showingHitboxes) {
                    this->m_isDrawDebug = true; 
                    showingHitboxes = true;
                    deadTapCount = 0;
                    return;
                }

                if (deadTapCount == 2 && showingHitboxes) {
                    isPlayerDeadAndFrozen = false;
                    this->m_isDrawDebug = false;
                    this->resumeSchedulerAndActions();
                    this->resetLevel();
                    return;
                }
            }
            return;
        }
        PlayLayer::pushButton(btn, isPlayer2);
    }
};

	}
};
