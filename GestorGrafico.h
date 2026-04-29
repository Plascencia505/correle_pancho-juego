#ifndef GESTOR_GRAFICO_H
#define GESTOR_GRAFICO_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "GameManager.h"
#include "EntidadJugador.h"
#include "GestorObstaculos.h"
#include "GestorNiveles.h"
#include "SistemaArchivos.h"

#define ANCHO_PANTALLA 128
#define ALTO_PANTALLA 64
#define OLED_RESET -1
#define DIRECCION_I2C 0x3C

class GestorGrafico {
private:
  Adafruit_SSD1306 pantalla;

  // Métodos internos actualizados para coincidir con tu .cpp final
  void dibujarIntro();
  void dibujarMenuInicio(int opcionMenu, bool cursorVisible);
  void dibujarEscenario(EntidadJugador* jugador, GestorObstaculos* obstaculos, GestorNiveles* niveles);
  void dibujarCapaPausa();
  void dibujarVictoriaNivel1(bool cursorVisible);
  void dibujarVictoriaFinal(bool cursorVisible);
  void dibujarGameOver(long score, bool cursorVisible);
  void dibujarTablaHighScores(SistemaArchivos* archivos);
  void dibujarRegistroNombre(long score, const char* nombreActual, int indiceLetra, bool cursorVisible);

public:
  GestorGrafico();

  bool inicializar();

  void renderizarFrame(
    GameState estadoActual,
    EntidadJugador* jugador,
    GestorObstaculos* obstaculos,
    GestorNiveles* niveles,
    SistemaArchivos* archivos,
    const char* nombreInput,
    int indiceLetra,
    int opcionMenu,
    bool cursorVisible
  );

  void dibujarDesglose(long puntosBase, int vidas, long total, bool cursorVisible);
};
#endif