#include "SistemaArchivos.h"
#include <string.h>

SistemaArchivos::SistemaArchivos(int csPin) : pinCS(csPin) {
  for (int i = 0; i < MAX_SCORES; i++) {
    strcpy(topScores[i].nombre, "---");
    topScores[i].puntuacion = 0;
  }
}

bool SistemaArchivos::inicializar() {
  if (!SD.begin(pinCS)) return false;

  // Si no existen, crearlos
  if (!SD.exists(rutaTop)) {
    actualizarTopScores("---", 0);
  }
  return true;
}

bool SistemaArchivos::cargarTopScores() {
  File archivo = SD.open(rutaTop, FILE_READ);
  if (!archivo) return false;

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, archivo);
  archivo.close();

  if (error) return false;

  JsonArray arr = doc["scores"].as<JsonArray>();
  int i = 0;
  for (JsonObject score : arr) {
    if (i >= MAX_SCORES) break;
    strncpy(topScores[i].nombre, score["nombre"], 3);
    topScores[i].nombre[3] = '\0';
    topScores[i].puntuacion = score["puntuacion"];
    i++;
  }
  return true;
}

bool SistemaArchivos::guardarEnHistorial(const char* nombre, long puntuacion) {
  File archivo = SD.open(rutaHistorial, FILE_APPEND);
  if (!archivo) return false;

  // Crear el objeto JSON
  JsonDocument doc;
  doc["nombre"] = nombre;
  doc["puntuacion"] = puntuacion;

  serializeJson(doc, archivo);
  archivo.println(); 
  archivo.close();
  return true;
}

bool SistemaArchivos::esTopScore(long puntuacion) {
  return (puntuacion > topScores[MAX_SCORES - 1].puntuacion);
}

void SistemaArchivos::ordenarScores() {
  for (int i = 0; i < MAX_SCORES - 1; i++) {
    for (int j = 0; j < MAX_SCORES - i - 1; j++) {
      if (topScores[j].puntuacion < topScores[j + 1].puntuacion) {
        RegistroScore temp = topScores[j];
        topScores[j] = topScores[j + 1];
        topScores[j + 1] = temp;
      }
    }
  }
}

bool SistemaArchivos::actualizarTopScores(const char* nombre, long nuevaPuntuacion) {
  strncpy(topScores[MAX_SCORES - 1].nombre, nombre, 3);
  topScores[MAX_SCORES - 1].nombre[3] = '\0';
  topScores[MAX_SCORES - 1].puntuacion = nuevaPuntuacion;

  ordenarScores();

  JsonDocument doc;
  JsonArray arr = doc["scores"].to<JsonArray>();

  for (int i = 0; i < MAX_SCORES; i++) {
    JsonObject obj = arr.add<JsonObject>();
    obj["nombre"] = topScores[i].nombre;
    obj["puntuacion"] = topScores[i].puntuacion;
  }

  File archivo = SD.open(rutaTop, FILE_WRITE);
  if (!archivo) return false;

  serializeJson(doc, archivo);
  archivo.close();
  return true;
}

RegistroScore* SistemaArchivos::getTopScores() {
  return topScores;
}