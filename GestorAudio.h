#ifndef GESTOR_AUDIO_H
#define GESTOR_AUDIO_H

#include <Arduino.h>

enum TipoPista {
  PISTA_NINGUNA,
  PISTA_INTRO,
  PISTA_MENU,
  PISTA_JUEGO,
  PISTA_PAUSA,
  PISTA_HIGHSCORES,
  PISTA_GAMEOVER,
  PISTA_NUEVO_RECORD
};

enum TipoSFX {
  SFX_SALTO,
  SFX_DANO,
  SFX_BTN_A,
  SFX_BTN_B
};

struct CanalAudio {
  int pin;
  const int (*notas)[2]; // Puntero al arreglo 2D en PROGMEM
  int longitudTotal;
  int indiceActual;
  unsigned long tiempoInicioNota;
  bool reproduciendo;
  bool enBucle;
  float modificadorTono;
  TipoSFX sfxActual;
  bool tieneSFXActivo;
};

class GestorAudio {
private:
  CanalAudio canalMelodia;
  CanalAudio canalSFX;

  TipoPista pistaActual;

  void actualizarCanal(CanalAudio& canal);
  void detenerCanal(CanalAudio& canal);
  void iniciarPista(CanalAudio& canal, const int (*notas)[2], int longi, bool bucle, float tono);

public:
  GestorAudio();

  void inicializar(int pinMelodia, int pinSfx);
  void actualizar();

  void reproducirMusica(TipoPista pista);
  void detenerMusica();
  void reproducirSFX(TipoSFX sfx);
};

#endif