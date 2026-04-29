#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

// Maquina de estados
enum GameState {
  ESTADO_INTRO,
  ESTADO_MENU_INICIO,
  ESTADO_JUGANDO_NIVEL_1,
  ESTADO_JUGANDO_NIVEL_2,
  ESTADO_PAUSA,
  ESTADO_VICTORIA_NIVEL_1,
  ESTADO_VICTORIA_FINAL,
  ESTADO_REGISTRO_SCORE,
  ESTADO_MENU_SCORES,
  ESTADO_DESGLOSE_SCORE,
  ESTADO_GAMEOVER
};

class GameManager {
private:
  GameState estadoActual;

public:
  GameManager();

  void inicializar();
  void actualizar();
  void cambiarEstado(GameState nuevoEstado);
  GameState getEstadoActual();
};

#endif