#include "GestorObstaculos.h"
#include <Arduino.h>

GestorObstaculos::GestorObstaculos() {
  inicializar();
}

void GestorObstaculos::inicializar() {
  // Iniciar sin obstaculos
  for (int i = 0; i < MAX_OBSTACULOS; i++) {
    pool[i].activo = false;
  }
  umbralGeneracion = 110;  //Mínima distancia entre obstaculos
}

void GestorObstaculos::reiniciarNivel() {
  // Limpiar la pantalla para el siguiente nivel
  inicializar();
}

void GestorObstaculos::actualizar(float velocidadEscenario) {
  for (int i = 0; i < MAX_OBSTACULOS; i++) {
    if (pool[i].activo) {
      // Mover el obstáculo hacia la izquierda
      pool[i].posX -= velocidadEscenario;

      // Pool de reciclaje de obstaculos
      if (pool[i].posX < -20) {
        pool[i].activo = false;
      }
    }
  }
}

void GestorObstaculos::intentarGenerar() {
  // Verificar distancia entre obstáculos
  float maxPosX = 0;
  for (int i = 0; i < MAX_OBSTACULOS; i++) {
    if (pool[i].activo && pool[i].posX > maxPosX) {
      maxPosX = pool[i].posX;
    }
  }

  // No generar demasiados juntos
  if (128 - maxPosX < umbralGeneracion && maxPosX != 0) {
    return;
  }

  // Reciclar obstaculos
  for (int i = 0; i < MAX_OBSTACULOS; i++) {
    if (!pool[i].activo) {
      pool[i].activo = true;
      pool[i].posX = 128;

      pool[i].tipo = TIPO_MURO;
      pool[i].ancho = 12;
      pool[i].alto = 16;
      pool[i].posY = 40;

      umbralGeneracion = random(110, 145);

      break;
    }
  }
}

bool GestorObstaculos::verificarColision(EntidadJugador* jugador) {
  // Tomar los valores de la hitbox del jugador
  int jX = jugador->getX();
  int jY = jugador->getY();
  int jAncho = jugador->getAnchoHitbox();
  int jAlto = jugador->getAltoHitbox();

  for (int i = 0; i < MAX_OBSTACULOS; i++) {
    if (pool[i].activo) {
      int oX = (int)pool[i].posX;
      int oY = pool[i].posY;
      int oAncho = pool[i].ancho;
      int oAlto = pool[i].alto;

      bool colisionX = jX + jAncho > oX && jX < oX + oAncho;
      bool colisionY = (jY - jAlto) < (oY + oAlto) && jY > oY;

      if (colisionX && colisionY) {
        pool[i].activo = false;
        return true;
      }
    }
  }
  return false;
}

Obstaculo* GestorObstaculos::getObstaculos() {
  return pool;
}

int GestorObstaculos::getMaxObstaculos() {
  return MAX_OBSTACULOS;
}