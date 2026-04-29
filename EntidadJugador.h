#ifndef ENTIDAD_JUGADOR_H
#define ENTIDAD_JUGADOR_H

#include <Arduino.h>

enum EstadoFisico {
  CORRIENDO,
  SALTANDO,
  CAYENDO
};

class EntidadJugador {
private:
  int posX;
  int posY;
  float velocidadY;

  // Atributos de partida
  int vidas;  // Inicia en 3, tope en 4
  long puntuacion;
  EstadoFisico estadoActual;

  // Hitbox
  int anchoHitbox;
  int altoHitbox;

public:
  EntidadJugador();

  void inicializar();
  void actualizarFisicas();

  // Acciones del control (Joystick / Botones)
  void saltar();
  void levantar();

  // Estado de la partida
  void aplicarDano();
  void otorgarVidaExtra();
  void sumarPuntos(long puntos);

  // Getters para el motor y render
  int getX();
  int getY();
  int getAnchoHitbox();
  int getAltoHitbox();
  int getVidas();
  long getPuntuacion();
  EstadoFisico getEstado();
};

#endif