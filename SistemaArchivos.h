#ifndef SISTEMA_ARCHIVOS_H
#define SISTEMA_ARCHIVOS_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>

struct RegistroScore {
  char nombre[4];
  long puntuacion;
};

class SistemaArchivos {
private:
  int pinCS;
  const char* rutaTop = "/topScores.txt";
  const char* rutaHistorial = "/historial.txt";

  static const int MAX_SCORES = 4;
  RegistroScore topScores[MAX_SCORES];

  void ordenarScores();

public:
  SistemaArchivos(int csPin);
  bool inicializar();

  // Carga inicial del Top 4
  bool cargarTopScores();
  
  // Guardado de High Scores y de todas las puntuaciones
  bool guardarEnHistorial(const char* nombre, long puntuacion);
  bool esTopScore(long puntuacion);
  bool actualizarTopScores(const char* nombre, long nuevaPuntuacion);

  RegistroScore* getTopScores();
};

#endif