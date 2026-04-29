#ifndef GESTOR_OBSTACULOS_H
#define GESTOR_OBSTACULOS_H

#include "EntidadJugador.h"

enum TipoObstaculo {
  TIPO_MURO  // Obstáculo inferior, requiere salto
};

// Reciclar memoria
struct Obstaculo {
  TipoObstaculo tipo;
  float posX;
  int posY;
  int ancho;
  int alto;
  bool activo;
};

class GestorObstaculos {
private:
  static const int MAX_OBSTACULOS = 5;  // Límite de obstaculos en memoria
  Obstaculo pool[MAX_OBSTACULOS];

  int umbralGeneracion;  // Evitar que se amontonen de más

public:
  GestorObstaculos();

  void inicializar();
  void actualizar(float velocidadEscenario);
  void reiniciarNivel();

  // Lógica de juego
  void intentarGenerar();
  bool verificarColision(EntidadJugador* jugador);

  // Gráficos
  Obstaculo* getObstaculos();
  int getMaxObstaculos();
};

#endif