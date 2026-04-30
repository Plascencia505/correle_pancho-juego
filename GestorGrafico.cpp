#include "GestorGrafico.h"
#include "Sprites.h"

GestorGrafico::GestorGrafico() : pantalla(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, OLED_RESET) {}

bool GestorGrafico::inicializar() {
  if (!pantalla.begin(SSD1306_SWITCHCAPVCC, DIRECCION_I2C)) {
    Serial.println(F("Fallo al inicializar pantalla OLED"));
    return false;
  }
  pantalla.setTextColor(SSD1306_WHITE);
  pantalla.setTextSize(1);
  pantalla.clearDisplay();
  pantalla.display();
  return true;
}

void GestorGrafico::renderizarFrame(
  GameState estadoActual,
  EntidadJugador* jugador,
  GestorObstaculos* obstaculos,
  GestorNiveles* niveles,
  SistemaArchivos* archivos,
  const char* nombreInput,
  int indiceLetra,
  int opcionMenu,
  bool cursorVisible,
  int introFase,
  unsigned long introTiempoFase,
  int introXJugador,
  int introXPolicia,
  int introXGordo) {
  pantalla.clearDisplay();
  pantalla.setTextSize(1);
  pantalla.setTextColor(SSD1306_WHITE);

  switch (estadoActual) {
    case ESTADO_INTRO:
      dibujarIntro(introFase, introTiempoFase, introXJugador, introXPolicia, introXGordo);
      break;
    case ESTADO_MENU_INICIO:
      dibujarMenuInicio(opcionMenu, cursorVisible);
      break;
    case ESTADO_JUGANDO_NIVEL_1:
    case ESTADO_JUGANDO_NIVEL_2:
      dibujarEscenario(jugador, obstaculos, niveles);
      break;
    case ESTADO_PAUSA:
      dibujarEscenario(jugador, obstaculos, niveles);
      dibujarCapaPausa();
      break;
    case ESTADO_VICTORIA_NIVEL_1:
      dibujarVictoriaNivel1(cursorVisible);
      break;
    case ESTADO_VICTORIA_FINAL:
      dibujarVictoriaFinal(cursorVisible);
      break;
    case ESTADO_DESGLOSE_SCORE:
      dibujarDesglose(jugador->getPuntuacion(),
                      jugador->getVidas(),
                      jugador->getPuntuacion() + (jugador->getVidas() * 500),
                      cursorVisible);
      break;
    case ESTADO_REGISTRO_SCORE:
      dibujarRegistroNombre(jugador->getPuntuacion(), nombreInput, indiceLetra);
      break;
    case ESTADO_GAMEOVER:
      dibujarGameOver(jugador->getPuntuacion(), cursorVisible);
      break;
    case ESTADO_MENU_SCORES:
      dibujarTablaHighScores(archivos);
      break;
    default:
      break;
  }

  pantalla.display();
}

void GestorGrafico::dibujarIntro(int fase, unsigned long tiempoEnFase, int posJugador, int posPolicia, int posGordo) {
  switch (fase) {

    // Fase 0: Splash de estudio
    case 0:
      {
        bool visible = (tiempoEnFase < 600) ? ((tiempoEnFase / 80) % 2) : true;
        if (visible) {
          pantalla.setCursor(28, 20);
          pantalla.print(F("Un juego de:"));
          pantalla.setCursor(14, 35);
          pantalla.print(F("Fer, Dulce y Gael"));
        }
        break;
      }

    // Fase 1: Título letra por letra
    case 1:
      {
        const char* titulo = "CORRELE PANCHO!!";
        const int nLetras = 17;
        int letrasVisibles = tiempoEnFase / 80;
        if (letrasVisibles > nLetras) letrasVisibles = nLetras;

        pantalla.setTextSize(2);
        pantalla.setCursor(4, 20);
        for (int i = 0; i < letrasVisibles; i++) {
          pantalla.print(titulo[i]);
        }
        if (letrasVisibles < nLetras && (tiempoEnFase / 250) % 2) {
          pantalla.print(F("_"));
        }
        break;
      }

    // Fase 2: Sprites en movimiento
    case 2:
      {
        pantalla.setTextSize(2);
        pantalla.setCursor(4, 5);
        pantalla.print(F("CORRELE"));
        pantalla.setCursor(4, 22);
        pantalla.print(F("PANCHO!!!"));

        pantalla.drawLine(0, 56, 128, 56, SSD1306_WHITE);

        // Jugador siempre visible
        pantalla.drawBitmap(posJugador, 38, sprite_mexicano, 16, 16, SSD1306_WHITE);

        // Enemigos solo cuando ya están dentro del frame
        if (posPolicia <= 111) {
          pantalla.drawBitmap(posPolicia, 38, sprite_policia, 16, 16, SSD1306_WHITE);
        }
        if (posGordo <= 111) {
          pantalla.drawBitmap(posGordo, 38, sprite_gordo_copete, 16, 16, SSD1306_WHITE);
        }
        break;
      }

    case 3:
      {
        pantalla.setTextSize(2);
        pantalla.setCursor(4, 5);
        pantalla.print(F("CORRELE"));
        pantalla.setCursor(4, 22);
        pantalla.print(F("PANCHO!!!"));

        pantalla.drawLine(0, 56, 128, 56, SSD1306_WHITE);

        pantalla.drawBitmap(10, 38, sprite_mexicano, 16, 16, SSD1306_WHITE);
        pantalla.drawBitmap(55, 38, sprite_policia, 16, 16, SSD1306_WHITE);
        pantalla.drawBitmap(85, 38, sprite_gordo_copete, 16, 16, SSD1306_WHITE);

        if ((tiempoEnFase / 500) % 2) {
          pantalla.setTextSize(1);
          pantalla.setCursor(7, 47);
          pantalla.print(F("- PRESS A BUTTON -"));
        }
        break;
      }
  }
}

void GestorGrafico::dibujarMenuInicio(int opcionMenu, bool cursorVisible) {
  pantalla.setCursor(25, 5);
  pantalla.print(F("HOME MENU"));

  if (opcionMenu == 0 && cursorVisible) {
    pantalla.setCursor(0, 30);
    pantalla.print(F(">"));
  }
  pantalla.setCursor(10, 30);
  pantalla.print(F("A: New game"));

  if (opcionMenu == 1 && cursorVisible) {
    pantalla.setCursor(0, 45);
    pantalla.print(F(">"));
  }
  pantalla.setCursor(10, 45);
  pantalla.print(F("B: High Scores"));
}

void GestorGrafico::dibujarEscenario(EntidadJugador* jugador, GestorObstaculos* obstaculos, GestorNiveles* niveles) {
  pantalla.drawLine(0, 56, 128, 56, SSD1306_WHITE);

  pantalla.setCursor(0, 0);
  pantalla.print(F("Score:"));
  pantalla.print(jugador->getPuntuacion());
  pantalla.setCursor(90, 0);
  pantalla.print(F("HP:"));
  pantalla.print(jugador->getVidas());

  float progreso = niveles->getPorcentajeProgreso();
  pantalla.drawRect(34, 10, 60, 4, SSD1306_WHITE);
  pantalla.fillRect(35, 11, (int)(progreso * 0.58f), 2, SSD1306_WHITE);

  int enemigoX = niveles->getEnemigoPosX();
  const unsigned char* spriteEnemigo =
    (niveles->getNivelActual() == 1) ? sprite_policia : sprite_gordo_copete;
  pantalla.drawBitmap(enemigoX, 40, spriteEnemigo, 16, 16, SSD1306_WHITE);

  pantalla.drawBitmap(15, jugador->getY() - 16, sprite_mexicano, 16, 16, SSD1306_WHITE);

  Obstaculo* pool = obstaculos->getObstaculos();
  for (int i = 0; i < obstaculos->getMaxObstaculos(); i++) {
    if (pool[i].activo) {
      if (pool[i].tipo == TIPO_MURO)
        pantalla.fillRect(pool[i].posX, pool[i].posY, pool[i].ancho, pool[i].alto, SSD1306_WHITE);
      else
        pantalla.drawRect(pool[i].posX, pool[i].posY, pool[i].ancho, pool[i].alto, SSD1306_WHITE);
    }
  }
}

void GestorGrafico::dibujarCapaPausa() {
  pantalla.fillRect(30, 20, 68, 25, SSD1306_BLACK);
  pantalla.drawRect(30, 20, 68, 25, SSD1306_WHITE);
  pantalla.setCursor(45, 28);
  pantalla.print(F("PAUSE"));
}

void GestorGrafico::dibujarVictoriaNivel1(bool cursorVisible) {
  for (int i = 0; i < 64; i += 8)
    pantalla.drawLine(0, i, 128, i, SSD1306_WHITE);
  pantalla.fillRect(0, 0, 40, 24, SSD1306_WHITE);

  pantalla.fillRect(15, 20, 100, 25, SSD1306_BLACK);
  pantalla.drawRect(15, 20, 100, 25, SSD1306_WHITE);
  pantalla.setCursor(20, 25);
  pantalla.print(F("Has escapado..."));
  pantalla.setCursor(35, 35);
  pantalla.print(F("por ahora"));

  if (cursorVisible) {
    pantalla.setCursor(10, 56);
    pantalla.print(F("Continuar (Boton A)"));
  }
}

void GestorGrafico::dibujarVictoriaFinal(bool cursorVisible) {
  pantalla.fillRect(0, 0, 42, 64, SSD1306_WHITE);
  pantalla.fillRect(86, 0, 42, 64, SSD1306_WHITE);
  pantalla.drawCircle(64, 32, 5, SSD1306_WHITE);

  pantalla.fillRect(10, 10, 108, 45, SSD1306_BLACK);
  pantalla.drawRect(10, 10, 108, 45, SSD1306_WHITE);
  pantalla.setCursor(15, 15);
  pantalla.print(F("Has llegado a una"));
  pantalla.setCursor(15, 25);
  pantalla.print(F("peor mejor vida,"));
  pantalla.setCursor(15, 35);
  pantalla.print(F("creo es bueno"));

  if (cursorVisible) {
    pantalla.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    pantalla.setCursor(10, 56);
    pantalla.print(F("Continuar (Boton A)"));
    pantalla.setTextColor(SSD1306_WHITE);
  }
}

void GestorGrafico::dibujarGameOver(long score, bool cursorVisible) {
  pantalla.fillRect(15, 10, 98, 45, SSD1306_BLACK);
  pantalla.drawRect(15, 10, 98, 45, SSD1306_WHITE);
  pantalla.setCursor(35, 13);
  pantalla.print(F("GAME OVER"));
  pantalla.setCursor(30, 28);
  pantalla.print(F("FRIJOLERO!!"));
  pantalla.setCursor(25, 43);
  pantalla.print(F("Score: "));
  pantalla.print(score);

  if (cursorVisible) {
    pantalla.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    pantalla.setCursor(10, 56);
    pantalla.print(F("Continuar (Boton A)"));
    pantalla.setTextColor(SSD1306_WHITE);
  }
}

void GestorGrafico::dibujarTablaHighScores(SistemaArchivos* archivos) {
  pantalla.setCursor(25, 0);
  pantalla.print(F("TOP 4 ESCAPES"));
  pantalla.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  RegistroScore* scores = archivos->getTopScores();
  for (int i = 0; i < 4; i++) {
    pantalla.setCursor(10, 15 + (i * 12));
    pantalla.print(i + 1);
    pantalla.print(F(". "));
    pantalla.print(scores[i].nombre);
    pantalla.setCursor(80, 15 + (i * 12));
    pantalla.print(scores[i].puntuacion);
  }
}

void GestorGrafico::dibujarRegistroNombre(long score, const char* nombreActual, int indiceLetra) {
  // Parpadeo de la letra activa a 250ms
  bool letraVisible = (millis() / 250) % 2;

  pantalla.setCursor(10, 5);
  pantalla.print(F("NUEVO RECORD!"));
  pantalla.setCursor(10, 18);
  pantalla.print(F("Score: "));
  pantalla.print(score);
  pantalla.setCursor(10, 33);
  pantalla.print(F("Nombre: "));

  const int xBase = 58;
  const int slotAncho = 14;

  for (int i = 0; i < 3; i++) {
    int xLetra = xBase + (i * slotAncho);

    if (i < indiceLetra) {
      // Letra ya confirmada: estática + subrayado
      pantalla.setCursor(xLetra, 33);
      pantalla.print(nombreActual[i]);
      pantalla.drawLine(xLetra, 42, xLetra + 5, 42, SSD1306_WHITE);

    } else if (i == indiceLetra) {
      // Letra activa: parpadeo + subrayado siempre visible
      if (letraVisible) {
        pantalla.setCursor(xLetra, 33);
        pantalla.print(nombreActual[i]);
      }
      pantalla.drawLine(xLetra, 42, xLetra + 5, 42, SSD1306_WHITE);

    } else {
      // Guión a modo de placeholder
      pantalla.setCursor(xLetra, 33);
      pantalla.print(F("-"));
    }
  }

  pantalla.setCursor(4, 52);
  pantalla.print(F("A:OK  B:Borrar"));
}

void GestorGrafico::dibujarDesglose(long puntosBase, int vidas, long total, bool cursorVisible) {
  pantalla.setCursor(20, 0);
  pantalla.print(F("MISSION CLEAR"));
  pantalla.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  pantalla.setCursor(10, 20);
  pantalla.print(F("Score: "));
  pantalla.print(puntosBase);

  pantalla.setCursor(10, 32);
  pantalla.print(F("HP Bonus: "));
  pantalla.print(vidas);
  pantalla.print(F(" x 500"));

  pantalla.drawRect(5, 45, 118, 15, SSD1306_WHITE);
  pantalla.setCursor(15, 49);
  pantalla.print(F("TOTAL: "));
  pantalla.print(total);

  if (cursorVisible) {
    pantalla.fillRect(5, 45, 118, 15, SSD1306_BLACK);
    pantalla.setCursor(10, 49);
    pantalla.print(F("Continuar (Boton A)"));
  }
}