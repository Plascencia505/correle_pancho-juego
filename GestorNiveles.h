#ifndef GESTOR_NIVELES_H
#define GESTOR_NIVELES_H

class GestorNiveles {
private:
  int nivelActual;

  // Dificultad
  float velocidadEscenario;
  float velocidadMaxima;
  float tasaIncremento;

  // Progreso
  float distanciaRecorrida;
  float distanciaMeta;

  // Enemigo
  int enemigoPosX;
  bool enemigoRetrocediendo;
  float umbralRetroceso; //Distancia donde el enemigo deja de seguirnos

public:
  GestorNiveles();

  // Control del nivel
  void inicializarNivel(int numeroNivel);
  void actualizarProgreso();

  // Lógica del enemigo
  void actualizarEnemigo();

  // Verificadores de estado
  bool metaAlcanzada();

  // Getters para el Gestor Gráfico y Gestor de Obstáculos
  float getVelocidadEscenario();
  float getPorcentajeProgreso();
  int getEnemigoPosX();
  int getNivelActual();
};

#endif