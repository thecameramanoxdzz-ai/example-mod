#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variables para controlar el estado del juego
bool isPlayerDeadAndFrozen = false;
int deadTapCount = 0;
bool showingHitboxes = false;

class $modify(MyPlayLayer, PlayLayer) {
    // 1. Detectamos cuando el icono muere
    void destroyPlayer(PlayerObject* player, GameObject* obstacle) {
        PlayLayer::destroyPlayer(player, obstacle);
        
        isPlayerDeadAndFrozen = true;
        deadTapCount = 0;
        showingHitboxes = false;
        
        // Congela la pantalla en el frame exacto de la muerte
        this->pauseSchedulerAndActions();
    }

    // 2. Detectamos los toques en la pantalla estando congelado
    void pushButton(PlayerButton btn, bool isPlayer2) {
        if (isPlayerDeadAndFrozen) {
            if (btn == PlayerButton::Jump) { // Si el jugador toca la pantalla
                deadTapCount++;

                // Primer doble toque: Muestra hitboxes
                if (deadTapCount == 2 && !showingHitboxes) {
                    this->m_debugDraw = true;
                    
                    showingHitboxes = true;
                    deadTapCount = 0; // Reinicia contador para los próximos toques
                    return;
                }

                // Segundo doble toque: Descongela y reinicia el nivel
                if (deadTapCount == 2 && showingHitboxes) {
                    isPlayerDeadAndFrozen = false;
                    this->m_debugDraw = false;
                    
                    this->resumeSchedulerAndActions(); // Descongela
                    this->resetLevel(); // Revive/Reinicia
                    return;
                }
            }
            return; // Bloquea el toque para que el icono muerto no intente saltar
        }
        PlayLayer::pushButton(btn, isPlayer2);
    }
};
