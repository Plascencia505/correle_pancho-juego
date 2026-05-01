#include <Wire.h>
#include "GameManager.h"
#include "EntidadJugador.h"
#include "GestorObstaculos.h"
#include "GestorNiveles.h"
#include "SistemaArchivos.h"
#include "GestorAudio.h"
#include "GestorGrafico.h"

#define PIN_BOTON_A 32
#define PIN_BOTON_B 33
#define PIN_JOYSTICK_Y 35
#define PIN_JOYSTICK_X 34
#define PIN_SD_CS 5
#define PIN_BUZ_OST1 26
#define PIN_BUZ_OST2 25
#define PIN_BUZ_SFX 27
#define PIN_TRIG 4
#define PIN_ECHO 15

#define UMBRAL_JOY_ARRIBA 1000
#define UMBRAL_JOY_ABAJO 3000
#define TIEMPO_REBOTE 200
#define VELOCIDAD_SCROLL_LETRAS 150
#define COOLDOWN_SALTO 400
#define COOLDOWN_DANO 600

// Declaración de instancias
GameManager gameManager;
EntidadJugador jugador;
GestorObstaculos obstaculos;
GestorNiveles niveles;
SistemaArchivos archivos(PIN_SD_CS);
GestorAudio audio;
GestorGrafico grafico;

// Variables globales para control
int opcionMenu = 0;
unsigned long ultimoTiempoAccion = 0;
unsigned long ultimoScrollJoy = 0;
unsigned long tiempoUltimaPresencia = 0;
unsigned long ultimoTiempoSalto = 0;
unsigned long ultimoTiempoDano = 0;

// Variables globales de intro
int faseIntro = 0;
unsigned long tiempoInicioFase = 0;
int introXJugador = -16;
int introXPolicia = 144;
int introXGordo = 168;

const unsigned long TIEMPO_LIMITE_AFK = 1500;

char nombreInput[4] = "AAA";
int indiceLetra = 0;
long puntajeTotalFinal = 0;

long leerDistanciaUltraso() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  long duracion = pulseIn(PIN_ECHO, HIGH, 10000);
  return (duracion == 0) ? 999 : duracion / 58.2;
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_BOTON_A, INPUT);
  pinMode(PIN_BOTON_B, INPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  audio.inicializar(PIN_BUZ_OST2, PIN_BUZ_OST1, PIN_BUZ_SFX);

  if (!grafico.inicializar() || !archivos.inicializar()) {
    Serial.println(F("Error de hardware detectado."));
    while (1)
      ;
  }

  archivos.cargarTopScores();
  gameManager.inicializar();
  ultimoTiempoAccion = millis();
  tiempoInicioFase = millis();
}

void loop() {
  bool btnAPulsado = (digitalRead(PIN_BOTON_A) == LOW);
  bool btnBPulsado = (digitalRead(PIN_BOTON_B) == LOW);

  int joyY = analogRead(PIN_JOYSTICK_Y);
  long distanciaMano = leerDistanciaUltraso();
  bool cursorVisible = (millis() / 300) % 2;

  GameState estado = gameManager.getEstadoActual();
  gameManager.actualizar();
  audio.actualizar();

  switch (estado) {

    case ESTADO_INTRO:
      audio.reproducirMusica(PISTA_INTRO);
      {
        unsigned long ahora = millis();
        unsigned long tiempoEnFase = ahora - tiempoInicioFase;

        if (faseIntro == 0 && tiempoEnFase >= 2000) {
          faseIntro = 1;
          tiempoInicioFase = ahora;
        } else if (faseIntro == 1 && tiempoEnFase >= 1800) {
          faseIntro = 2;
          tiempoInicioFase = ahora;
          introXJugador = -16;
          introXPolicia = 144;
          introXGordo = 168;
        }

        else if (faseIntro == 2) {
          introXJugador += 3;
          if (introXJugador > 144) {
            faseIntro = 3;
            tiempoInicioFase = ahora;
          }
        }

        else if (faseIntro == 3 && tiempoEnFase >= 500) {
          faseIntro = 4;
          tiempoInicioFase = ahora;
          introXPolicia = -16;
          introXGordo = -40;
        }

        else if (faseIntro == 4) {
          introXPolicia += 3;
          introXGordo += 3;
          if (introXPolicia > 144) {
            faseIntro = 5;
            tiempoInicioFase = ahora;
            introXJugador = 128;
          }
        }

        else if (faseIntro == 5) {
          introXJugador -= 3;
          if (introXJugador <= 56) {
            faseIntro = 6;
            tiempoInicioFase = ahora;
            introXJugador = 56;
          }
        }

        if (faseIntro == 6 && (btnAPulsado || btnBPulsado) && (millis() - ultimoTiempoAccion > TIEMPO_REBOTE)) {
          audio.reproducirSFX(btnAPulsado ? SFX_BTN_A : SFX_BTN_B);
          ultimoTiempoAccion = millis();
          faseIntro = 0;
          tiempoInicioFase = millis();
          gameManager.cambiarEstado(ESTADO_MENU_INICIO);
        }
      }
      break;

    case ESTADO_MENU_INICIO:
      audio.reproducirMusica(PISTA_MENU);
      if (joyY < UMBRAL_JOY_ARRIBA) opcionMenu = 0;
      if (joyY > UMBRAL_JOY_ABAJO) opcionMenu = 1;

      if (btnAPulsado && (millis() - ultimoTiempoAccion > TIEMPO_REBOTE)) {
        audio.reproducirSFX(SFX_BTN_A);
        ultimoTiempoAccion = millis();
        if (opcionMenu == 0) {
          jugador.inicializar();
          niveles.inicializarNivel(1);
          obstaculos.reiniciarNivel();
          puntajeTotalFinal = 0;
          tiempoUltimaPresencia = millis();
          gameManager.cambiarEstado(ESTADO_JUGANDO_NIVEL_1);
        } else {
          gameManager.cambiarEstado(ESTADO_MENU_SCORES);
        }
      }
      break;

    case ESTADO_JUGANDO_NIVEL_1:
    case ESTADO_JUGANDO_NIVEL_2:
      audio.reproducirMusica(PISTA_JUEGO);

      // Detección de presencia
      if (distanciaMano > 0 && distanciaMano <= 45) {
        tiempoUltimaPresencia = millis();
      }
      if (millis() - tiempoUltimaPresencia > TIEMPO_LIMITE_AFK) {
        ultimoTiempoAccion = millis();
        tiempoUltimaPresencia = millis();
        gameManager.cambiarEstado(ESTADO_PAUSA);
        break;
      }

      // Pausa manual
      if (btnAPulsado && (millis() - ultimoTiempoAccion > TIEMPO_REBOTE)) {
        audio.reproducirSFX(SFX_BTN_A);
        ultimoTiempoAccion = millis();
        tiempoUltimaPresencia = millis();
        gameManager.cambiarEstado(ESTADO_PAUSA);
        break;
      }

      if (joyY < UMBRAL_JOY_ARRIBA && (millis() - ultimoTiempoSalto > COOLDOWN_SALTO)) {
        jugador.saltar();
        audio.reproducirSFX(SFX_SALTO);
        ultimoTiempoSalto = millis();
      }

      // Lógica de juego
      jugador.actualizarFisicas();
      niveles.actualizarProgreso();
      niveles.actualizarEnemigo();
      obstaculos.actualizar(niveles.getVelocidadEscenario());
      obstaculos.intentarGenerar();
      jugador.sumarPuntos(1);

      if (obstaculos.verificarColision(&jugador)) {
        if (millis() - ultimoTiempoDano > COOLDOWN_DANO) {
          jugador.aplicarDano();
          audio.reproducirSFX(SFX_DANO);
          ultimoTiempoDano = millis();

          if (jugador.getVidas() <= 0) {
            puntajeTotalFinal = jugador.getPuntuacion();
            ultimoTiempoAccion = millis();
            if (archivos.esTopScore(puntajeTotalFinal)) {
              gameManager.cambiarEstado(ESTADO_REGISTRO_SCORE);
            } else {
              archivos.guardarEnHistorial("---", puntajeTotalFinal);
              gameManager.cambiarEstado(ESTADO_GAMEOVER);
            }
          }
        }
      }

      if (niveles.metaAlcanzada()) {
        ultimoTiempoAccion = millis();
        gameManager.cambiarEstado(
          estado == ESTADO_JUGANDO_NIVEL_1 ? ESTADO_VICTORIA_NIVEL_1 : ESTADO_VICTORIA_FINAL);
      }
      break;

    case ESTADO_PAUSA:
      audio.reproducirMusica(PISTA_PAUSA);
      if (btnBPulsado && (millis() - ultimoTiempoAccion > TIEMPO_REBOTE)) {
        audio.reproducirSFX(SFX_BTN_B);
        ultimoTiempoAccion = millis();
        tiempoUltimaPresencia = millis();
        gameManager.cambiarEstado(
          niveles.getNivelActual() == 1 ? ESTADO_JUGANDO_NIVEL_1 : ESTADO_JUGANDO_NIVEL_2);
      }
      break;

    case ESTADO_VICTORIA_NIVEL_1:
      audio.detenerMusica();
      if (btnAPulsado && (millis() - ultimoTiempoAccion > TIEMPO_REBOTE)) {
        audio.reproducirSFX(SFX_BTN_A);
        ultimoTiempoAccion = millis();
        tiempoUltimaPresencia = millis();
        jugador.otorgarVidaExtra();
        niveles.inicializarNivel(2);
        obstaculos.reiniciarNivel();
        gameManager.cambiarEstado(ESTADO_JUGANDO_NIVEL_2);
      }
      break;

    case ESTADO_VICTORIA_FINAL:
      audio.detenerMusica();
      if (btnAPulsado && (millis() - ultimoTiempoAccion > TIEMPO_REBOTE)) {
        audio.reproducirSFX(SFX_BTN_A);
        ultimoTiempoAccion = millis();
        gameManager.cambiarEstado(ESTADO_DESGLOSE_SCORE);
      }
      break;

    case ESTADO_DESGLOSE_SCORE:
      audio.reproducirMusica(PISTA_HIGHSCORES);
      if (btnAPulsado && (millis() - ultimoTiempoAccion > TIEMPO_REBOTE)) {
        audio.reproducirSFX(SFX_BTN_A);
        ultimoTiempoAccion = millis();
        jugador.sumarPuntos(jugador.getVidas() * 500);
        puntajeTotalFinal = jugador.getPuntuacion();
        if (archivos.esTopScore(puntajeTotalFinal)) {
          gameManager.cambiarEstado(ESTADO_REGISTRO_SCORE);
        } else {
          archivos.guardarEnHistorial("---", puntajeTotalFinal);
          gameManager.cambiarEstado(ESTADO_MENU_SCORES);
        }
      }
      break;

    case ESTADO_REGISTRO_SCORE:
      audio.reproducirMusica(PISTA_NUEVO_RECORD);
      if (millis() - ultimoScrollJoy > VELOCIDAD_SCROLL_LETRAS) {
        if (joyY < UMBRAL_JOY_ARRIBA) {
          nombreInput[indiceLetra]++;
          ultimoScrollJoy = millis();
        } else if (joyY > UMBRAL_JOY_ABAJO) {
          nombreInput[indiceLetra]--;
          ultimoScrollJoy = millis();
        }
        if (nombreInput[indiceLetra] > 'Z') nombreInput[indiceLetra] = 'A';
        if (nombreInput[indiceLetra] < 'A') nombreInput[indiceLetra] = 'Z';
      }
      if (btnAPulsado && (millis() - ultimoTiempoAccion > TIEMPO_REBOTE)) {
        audio.reproducirSFX(SFX_BTN_A);
        indiceLetra++;
        ultimoTiempoAccion = millis();
        if (indiceLetra > 2) {
          archivos.guardarEnHistorial(nombreInput, puntajeTotalFinal);
          archivos.actualizarTopScores(nombreInput, puntajeTotalFinal);
          Serial.println(F("Registro de usuario completo"));
          gameManager.cambiarEstado(ESTADO_MENU_SCORES);
          indiceLetra = 0;
          strcpy(nombreInput, "AAA");
        }
      }
      if (btnBPulsado && (millis() - ultimoTiempoAccion > TIEMPO_REBOTE)) {
        if (indiceLetra > 0) {
          audio.reproducirSFX(SFX_BTN_B);
          indiceLetra--;
          ultimoTiempoAccion = millis();
        }
      }
      break;

    case ESTADO_MENU_SCORES:
      audio.reproducirMusica(PISTA_HIGHSCORES);
      if ((btnBPulsado || btnAPulsado) && (millis() - ultimoTiempoAccion > TIEMPO_REBOTE)) {
        audio.reproducirSFX(SFX_BTN_B);
        ultimoTiempoAccion = millis();
        gameManager.cambiarEstado(ESTADO_MENU_INICIO);
      }
      break;

    case ESTADO_GAMEOVER:
      audio.reproducirMusica(PISTA_GAMEOVER);
      if (btnAPulsado && (millis() - ultimoTiempoAccion > TIEMPO_REBOTE)) {
        audio.reproducirSFX(SFX_BTN_A);
        ultimoTiempoAccion = millis();
        gameManager.cambiarEstado(ESTADO_MENU_INICIO);
      }
      break;

    default:
      break;
  }

  grafico.renderizarFrame(
    estado,
    &jugador, &obstaculos, &niveles, &archivos,
    nombreInput, indiceLetra,
    opcionMenu, cursorVisible,
    // Parámetros de intro
    faseIntro, millis() - tiempoInicioFase,
    introXJugador, introXPolicia, introXGordo);
  delay(16);
}