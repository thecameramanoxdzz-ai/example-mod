#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variables globales para controlar el estado del juego
bool isPlayerDeadAndFrozen = false;
int deadTapCount = 0;
bool showingHitboxes = false;

class $modify(MyPlayLayer, PlayLayer) {
    // Estructura obligatoria para que Geode no falle con macros de m_fields
    struct Fields {
        bool m_dummy = false;
    };

    // 1. Detectamos cuando el icono muere
    void destroyPlayer(PlayerObject* player, GameObject* obstacle) {
        PlayLayer::destroyPlayer(player, obstacle);

        isPlayerDeadAndFrozen = true;
        deadTapCount = 0;
        showingHitboxes = false;

        // Congela la pantalla en el frame exacto de la muerte
        this->pauseSchedulerAndActions();
    }

    // 2. Forzamos el dibujado de hitboxes si la variable está activa
    void draw() {
        if (showingHitboxes) {
            // Activa temporalmente las hitboxes nativas durante el renderizado
            this->m_debugDraw = true;
        }
        PlayLayer::draw();
    }

    // 3. Detectamos los toques en la pantalla estando congelado
    void pushButton(PlayerButton btn, bool isPlayer2) {
        if (isPlayerDeadAndFrozen) {
            if (btn == PlayerButton::Jump) { 
                deadTapCount++;

                // Primer doble toque: Muestra hitboxes
                if (deadTapCount == 2 && !showingHitboxes) {
                    showingHitboxes = true;
                    deadTapCount = 0; 
                    return;
                }

                // Segundo doble toque: Descongela y reinicia el nivel
                if (deadTapCount == 2 && showingHitboxes) {
                    isPlayerDeadAndFrozen = false;
                    showingHitboxes = false;
                    this->m_debugDraw = false;
                    this->resumeSchedulerAndActions(); 
                    this->resetLevel(); 
                    return;
                }
            }
            return; // Bloquea el toque para que el icono muerto no intente saltar
        }
        PlayLayer::pushButton(btn, isPlayer2);
    }
};
