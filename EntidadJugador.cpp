#include "EntidadJugador.h"

EntidadJugador::EntidadJugador() {
  inicializar();
}

void EntidadJugador::inicializar() {
  posX = 20; 
  posY = 56;
  velocidadY = 0.0;

  vidas = 3;
  puntuacion = 0;
  estadoActual = CORRIENDO;

  anchoHitbox = 16;
  altoHitbox = 16;
}

void EntidadJugador::actualizarFisicas() {
  if (estadoActual != CORRIENDO) {
    velocidadY += 0.9;
    posY += velocidadY;

    if (velocidadY > 0 && estadoActual == SALTANDO) {
      estadoActual = CAYENDO;
    }
    
    if (posY >= 56) {
      posY = 56;
      velocidadY = 0;
      estadoActual = CORRIENDO;  
    }
  }
}

void EntidadJugador::saltar() {
  //Para brincar dos muros
  if (estadoActual == CORRIENDO) {
    velocidadY = -9.0;
    estadoActual = SALTANDO;
  }
}

void EntidadJugador::aplicarDano() {
  if (vidas > 0) vidas--;
}

void EntidadJugador::otorgarVidaExtra() {
  // Un máximo de 4 vidas
  if (vidas < 4) vidas++;
}

void EntidadJugador::sumarPuntos(long puntos) {
  puntuacion += puntos;
}

//Getters
int EntidadJugador::getX() {
  return posX;
}
int EntidadJugador::getY() {
  return posY;
}
int EntidadJugador::getAnchoHitbox() {
  return anchoHitbox;
}
int EntidadJugador::getAltoHitbox() {
  return altoHitbox;
}
int EntidadJugador::getVidas() {
  return vidas;
}
long EntidadJugador::getPuntuacion() {
  return puntuacion;
}
EstadoFisico EntidadJugador::getEstado() {
  return estadoActual;
}