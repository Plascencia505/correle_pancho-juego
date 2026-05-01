#include "GestorAudio.h"
#include "Soundtrack.h"

static inline int leerFrecuencia(const int (*arr)[2], int idx) {
  return (int)pgm_read_word(&arr[idx][0]);
}
static inline int leerDuracion(const int (*arr)[2], int idx) {
  return (int)pgm_read_word(&arr[idx][1]);
}

GestorAudio::GestorAudio() {
  pistaActual = PISTA_NINGUNA;
}

void GestorAudio::inicializar(int pMelodia, int pArmonia, int pSfx) {
  canalMelodia.pin = pMelodia;
  canalArmonia.pin = pArmonia;
  canalSFX.pin = pSfx;

  pinMode(canalMelodia.pin, OUTPUT);
  pinMode(canalArmonia.pin, OUTPUT);
  pinMode(canalSFX.pin, OUTPUT);
  canalSFX.tieneSFXActivo = false;

  detenerMusica();
}

void GestorAudio::actualizar() {
  actualizarCanal(canalMelodia);
  actualizarCanal(canalArmonia);
  actualizarCanal(canalSFX);
}

void GestorAudio::actualizarCanal(CanalAudio& canal) {
  if (!canal.reproduciendo) return;

  unsigned long tiempoActual = millis();
  int duracionNota = leerDuracion(canal.notas, canal.indiceActual);
  unsigned long tiempoTranscurrido = tiempoActual - canal.tiempoInicioNota;

  if (tiempoTranscurrido >= (unsigned long)(duracionNota * 0.85f)) {
    noTone(canal.pin);
  }

  // Avanzar de notas una vez cumplido el tiempo
  if (tiempoTranscurrido >= (unsigned long)duracionNota) {
    canal.indiceActual++;

    if (canal.indiceActual >= canal.longitudTotal) {
      if (canal.enBucle) {
        canal.indiceActual = 0;
      } else {
        detenerCanal(canal);
        return;
      }
    }

    // Lectura de notas
    int frecuencia = leerFrecuencia(canal.notas, canal.indiceActual);

    if (frecuencia > 0) {
      tone(canal.pin, (int)(frecuencia * canal.modificadorTono));
    }

    canal.tiempoInicioNota = tiempoActual;
  }
}

void GestorAudio::detenerCanal(CanalAudio& canal) {
  canal.reproduciendo = false;
  noTone(canal.pin);
}

void GestorAudio::detenerMusica() {
  detenerCanal(canalMelodia);
  detenerCanal(canalArmonia);
  pistaActual = PISTA_NINGUNA;
}

void GestorAudio::iniciarPista(
  CanalAudio& canal,
  const int (*notas)[2],
  int longi, bool bucle,
  float tono,
  unsigned long offsetMs) {
  canal.notas = notas;
  canal.longitudTotal = longi;
  canal.indiceActual = 0;
  canal.enBucle = bucle;
  canal.modificadorTono = tono;
  canal.reproduciendo = true;
  canal.tiempoInicioNota = millis() + offsetMs;

  int frecuencia = leerFrecuencia(notas, 0);
  if (frecuencia > 0) {
    tone(canal.pin, (int)(frecuencia * tono));
  } else {
    noTone(canal.pin);
  }
}

void GestorAudio::reproducirMusica(TipoPista pista) {
  if (pistaActual == pista) return;
  detenerMusica();
  pistaActual = pista;

  switch (pista) {
    case PISTA_INTRO:
      iniciarPista(canalMelodia, intro_melodia, intro_melodia_len, true, 1.0f);
      iniciarPista(canalArmonia, intro_melodia, intro_melodia_len, true, 0.5f, 2);
      break;
    case PISTA_MENU:
      iniciarPista(canalMelodia, menu_melodia, menu_melodia_len, true, 1.0f);
      iniciarPista(canalArmonia, menu_melodia, menu_melodia_len, true, 0.5f, 2);
      break;
    case PISTA_JUEGO:
      iniciarPista(canalMelodia, juego_melodia, juego_melodia_len, true, 1.0f);
      iniciarPista(canalArmonia, juego_melodia, juego_melodia_len, true, 0.5f, 2);
      break;
    case PISTA_PAUSA:
      iniciarPista(canalMelodia, juego_melodia, juego_melodia_len, true, 0.85f);
      iniciarPista(canalArmonia, juego_melodia, juego_melodia_len, true, 0.425f, 2);
      break;
    case PISTA_HIGHSCORES:
      iniciarPista(canalMelodia, scores_melodia, scores_melodia_len, true, 1.0f);
      iniciarPista(canalArmonia, scores_melodia, scores_melodia_len, true, 0.5f, 2);
      break;
    case PISTA_GAMEOVER:
      iniciarPista(canalMelodia, gameover_melodia, gameover_melodia_len, true, 1.0f);
      iniciarPista(canalArmonia, gameover_melodia, gameover_melodia_len, true, 0.5f, 2);
      break;
    case PISTA_NUEVO_RECORD:
      iniciarPista(canalMelodia, record_melodia, record_melodia_len, true, 1.0f);
      iniciarPista(canalArmonia, record_melodia, record_melodia_len, true, 0.5f, 2);
      break;
    default:
      break;
  }
}

void GestorAudio::reproducirSFX(TipoSFX sfx) {
  if (canalSFX.reproduciendo && canalSFX.tieneSFXActivo && canalSFX.sfxActual == sfx) {
    if (sfx != SFX_SALTO && sfx != SFX_DANO) return;
  }

  const int(*notasSolicitadas)[2] = nullptr;
  int longitudSolicitada = 0;

  switch (sfx) {
    case SFX_SALTO:
      notasSolicitadas = sfx_salto;
      longitudSolicitada = sfx_salto_len;
      break;
    case SFX_DANO:
      notasSolicitadas = sfx_dano;
      longitudSolicitada = sfx_dano_len;
      break;
    case SFX_BTN_A:
      notasSolicitadas = sfx_btna;
      longitudSolicitada = sfx_btna_len;
      break;
    case SFX_BTN_B:
      notasSolicitadas = sfx_btnb;
      longitudSolicitada = sfx_btnb_len;
      break;
  }

  canalSFX.sfxActual = sfx;
  canalSFX.tieneSFXActivo = true;
  iniciarPista(canalSFX, notasSolicitadas, longitudSolicitada, false, 1.0f);
}