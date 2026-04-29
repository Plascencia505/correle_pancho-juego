#include "GestorNiveles.h"

GestorNiveles::GestorNiveles() {
  inicializarNivel(1);
}

void GestorNiveles::inicializarNivel(int numeroNivel) {
  nivelActual = numeroNivel;
  distanciaRecorrida = 0.0;

  velocidadMaxima = 4.5;   // Velocidad máximo
  tasaIncremento = 0.003;  // Aceleración por frame


  enemigoPosX = -4;  // Posición del enemigo
  enemigoRetrocediendo = false;

  // Largo del primer nivel
  float distanciaBase = 2200.0;
  umbralRetroceso = 200.0;  // Rendición del enemigo

  //Diseño de nivel 2
  if (nivelActual == 2) {
    velocidadEscenario = 3.0;
    distanciaMeta = distanciaBase * 1.5;
  } else {
    distanciaMeta = distanciaBase;
    velocidadEscenario = 2.2;
  }
}

void GestorNiveles::actualizarProgreso() {
  // Distancia recorrida
  distanciaRecorrida += velocidadEscenario;

  // Aumento gradual de velocidad
  if (velocidadEscenario < velocidadMaxima) {
    velocidadEscenario += tasaIncremento;
  }
}

void GestorNiveles::actualizarEnemigo() {
  float distanciaFaltante = distanciaMeta - distanciaRecorrida;

  // Saber si el enemigo se rinde
  if (!enemigoRetrocediendo && distanciaFaltante <= umbralRetroceso) {
    enemigoRetrocediendo = true;
  }

  // Comportamiento del enemigo
  if (enemigoRetrocediendo) {
    enemigoPosX -= (velocidadEscenario * 0.5);
  } else {
    enemigoPosX = -4 + ((int)distanciaRecorrida % 3);
  }
}

bool GestorNiveles::metaAlcanzada() {
  return distanciaRecorrida >= distanciaMeta;
}

//Getters para la Interfaz y Entorno
float GestorNiveles::getVelocidadEscenario() {
  return velocidadEscenario;
}

float GestorNiveles::getPorcentajeProgreso() {
  // Porcentaje para la barra de progreso
  float porcentaje = (distanciaRecorrida / distanciaMeta) * 100.0;

  // Limitar el 100%
  if (porcentaje > 100.0) porcentaje = 100.0;

  return porcentaje;
}

int GestorNiveles::getEnemigoPosX() {
  return enemigoPosX;
}

int GestorNiveles::getNivelActual() {
  return nivelActual;
}