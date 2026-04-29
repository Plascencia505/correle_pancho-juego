#include "GameManager.h"
#include <Arduino.h>

// Define la pantalla de arranque
GameManager::GameManager() {
  estadoActual = ESTADO_INTRO;
}

// Reinicio para nueva partida
void GameManager::inicializar() {
  estadoActual = ESTADO_INTRO;
}

// Ciclo de actualización global
void GameManager::actualizar() {
  static unsigned long ultimoTiempoActivo = millis();
  // Tras 30 segundos de inactividad, regresamos al inicio
  if (estadoActual == ESTADO_REGISTRO_SCORE) {
    if (millis() - ultimoTiempoActivo > 30000) {
      cambiarEstado(ESTADO_MENU_INICIO);
      ultimoTiempoActivo = millis();
    }
  } else {
    // En cualquier otro estado, mantenemos el contador fresco
    ultimoTiempoActivo = millis();
  }
}

// Transición segura de estados
void GameManager::cambiarEstado(GameState nuevoEstado) {
  estadoActual = nuevoEstado;
}

// Exposición controlada del estado
GameState GameManager::getEstadoActual() {
  return estadoActual;
}