/*
 * Soundtrack.h
 * OST 8-bit para buzzer pasivo multicanal
 * Formato: { frecuencia_Hz, duracion_ms }
 */
#pragma once
#include <Arduino.h>

// Definición de notas (Hz) y Silencio
#define REST 0
#define B2 123
#define C3 131
#define D3 147
#define E3 165
#define F3 175
#define G3 196
#define A3 220
#define B3 247
#define C4 262
#define CS4 277
#define D4 294
#define DS4 311
#define E4 330
#define F4 349
#define FS4 370
#define G4 392
#define GS4 415
#define A4 440
#define AS4 466
#define B4 494
#define C5 523
#define CS5 554
#define D5 587
#define DS5 622
#define E5 659
#define F5 698
#define FS5 740
#define G5 784
#define GS5 831
#define A5 880
#define AS5 932
#define B5 988
#define C6 1047
#define D6 1175
#define E6 1319
#define G6 1568

// Duraciones a 132 BPM (ms)
#define DUR_W 1818  // Redonda
#define DUR_H 909   // Blanca
#define DUR_Q 455   // Negra
#define DUR_E 227   // Corchea
#define DUR_S 114   // Semicorchea

// PISTA 1: INTRO
const int PROGMEM intro_melodia[][2] = {
  { E5, DUR_S },
  { E5, DUR_S },
  { REST, DUR_S },
  { E5, DUR_S },
  { REST, DUR_S },
  { C5, DUR_S },
  { E5, DUR_E },
  { G5, DUR_E },
  { REST, DUR_E },
  { G4, DUR_E },
  { REST, DUR_E },
  { C5, DUR_E },
  { REST, DUR_S },
  { G4, DUR_E },
  { REST, DUR_S },
  { E4, DUR_E },
  { REST, DUR_S },
  { A4, DUR_S },
  { REST, DUR_S },
  { B4, DUR_S },
  { REST, DUR_S },
  { AS4, DUR_S },
  { A4, DUR_E },
  { G4, DUR_S },
  { E5, DUR_S },
  { G5, DUR_S },
  { A5, DUR_E },
  { F5, DUR_S },
  { G5, DUR_S },
  { REST, DUR_S },
  { E5, DUR_E },
  { C5, DUR_S },
  { D5, DUR_S },
  { B4, DUR_E },
  { REST, DUR_E },
  { C5, DUR_S },
  { E5, DUR_S },
  { G5, DUR_S },
  { C6, DUR_E },
  { G5, DUR_S },
  { E5, DUR_S },
  { C5, DUR_H },
};
const int intro_melodia_len = sizeof(intro_melodia) / sizeof(intro_melodia[0]);

const int PROGMEM intro_armonia[][2] = { { REST, DUR_W } };
const int intro_armonia_len = sizeof(intro_armonia) / sizeof(intro_armonia[0]);

// PISTA 2: MENU PRINCIPAL
const int PROGMEM menu_melodia[][2] = {
  { G4, DUR_E }, { C5, DUR_E }, { E5, DUR_E }, { G5, DUR_E }, { A5, DUR_H },
  { G5, DUR_E }, { E5, DUR_E }, { F5, DUR_E }, { E5, DUR_E }, { D5, DUR_E },
  { C5, DUR_E }, { G4, DUR_H }, { REST, DUR_H }, { C5, DUR_E }, { E5, DUR_E },
  { G5, DUR_E }, { C6, DUR_E }, { B5, DUR_H }, { A5, DUR_E }, { G5, DUR_E },
  { F5, DUR_E }, { G5, DUR_E }, { E5, DUR_E }, { D5, DUR_E }, { C5, DUR_W },
  { E5, DUR_E }, { DS5, DUR_E }, { D5, DUR_E }, { CS5, DUR_E }, { C5, DUR_E }, 
  { B4, DUR_E }, { AS4, DUR_E }, { A4, DUR_E }, { GS4, DUR_E }, { A4, DUR_E },
  { C5, DUR_E }, { E5, DUR_E }, { G5, DUR_H }, { REST, DUR_H }, { G4, DUR_E },
  { C5, DUR_E }, { E5, DUR_E }, { G5, DUR_E }, { A5, DUR_H }, { G5, DUR_E },
  { E5, DUR_E }, { F5, DUR_E }, { E5, DUR_E }, { D5, DUR_E }, { C5, DUR_E },
  { C5, DUR_W },
};
const int menu_melodia_len = sizeof(menu_melodia) / sizeof(menu_melodia[0]);

const int PROGMEM menu_armonia[][2] = { { REST, DUR_W } };
const int menu_armonia_len = sizeof(menu_armonia) / sizeof(menu_armonia[0]);

// PISTA 3: HIGH SCORES
// RITCHIEEEE!!!!!
const int PROGMEM scores_melodia[][2] = {
  // Motivo base tipo La Bamba (C mayor)
  { C5, DUR_E },
  { E5, DUR_E },
  { G5, DUR_E },
  { E5, DUR_E },
  { C5, DUR_E },
  { E5, DUR_E },
  { G5, DUR_Q },
  { F5, DUR_E },
  { A5, DUR_E },
  { C6, DUR_E },
  { A5, DUR_E },
  { F5, DUR_E },
  { A5, DUR_E },
  { C6, DUR_Q },
  { G5, DUR_E },
  { B5, DUR_E },
  { D6, DUR_E },
  { B5, DUR_E },
  { G5, DUR_E },
  { D6, DUR_E },
  { B5, DUR_Q },
  { C6, DUR_E },
  { B5, DUR_E },
  { A5, DUR_E },
  { G5, DUR_E },
  { E5, DUR_E },
  { C5, DUR_Q },
  { C5, DUR_E },
  { E5, DUR_E },
  { G5, DUR_E },
  { E5, DUR_E },
  { C5, DUR_H },
  { REST, DUR_H },
};
const int scores_melodia_len = sizeof(scores_melodia) / sizeof(scores_melodia[0]);

const int PROGMEM scores_armonia[][2] = { { REST, DUR_W } };
const int scores_armonia_len = sizeof(scores_armonia) / sizeof(scores_armonia[0]);

// PISTA 4: JUEGO
const int PROGMEM juego_melodia[][2] = {
  { G4, DUR_S },
  { G4, DUR_S },
  { G4, DUR_E },
  { G4, DUR_S },
  { A4, DUR_S },
  { G4, DUR_E },
  { E4, DUR_E },
  { C4, DUR_S },
  { D4, DUR_S },
  { G4, DUR_H },
  { G4, DUR_S },
  { G4, DUR_S },
  { G4, DUR_E },
  { G4, DUR_S },
  { A4, DUR_S },
  { G4, DUR_E },
  { E4, DUR_E },
  { REST, DUR_H },
  { REST, DUR_E },
  { C5, DUR_E },
  { B4, DUR_S },
  { A4, DUR_S },
  { G4, DUR_E },
  { E4, DUR_E },
  { D4, DUR_E },
  { E4, DUR_S },
  { F4, DUR_S },
  { G4, DUR_H },
  { A4, DUR_E },
  { G4, DUR_S },
  { F4, DUR_S },
  { E4, DUR_E },
  { C4, DUR_E },
  { D4, DUR_H },
  { G4, DUR_H },
  { E5, DUR_E },
  { D5, DUR_S },
  { C5, DUR_S },
  { B4, DUR_E },
  { A4, DUR_E },
  { G4, DUR_E },
  { A4, DUR_S },
  { B4, DUR_S },
  { C5, DUR_H },
  { D5, DUR_E },
  { C5, DUR_S },
  { B4, DUR_S },
  { A4, DUR_E },
  { G4, DUR_E },
  { F4, DUR_E },
  { G4, DUR_S },
  { A4, DUR_S },
  { G4, DUR_H },
};
const int juego_melodia_len = sizeof(juego_melodia) / sizeof(juego_melodia[0]);

const int PROGMEM juego_armonia[][2] = { { REST, DUR_W } };
const int juego_armonia_len = sizeof(juego_armonia) / sizeof(juego_armonia[0]);


// PISTA 5: GAME OVER
// Es una parodia a Frijolero, muy importante
const int PROGMEM gameover_melodia[][2] = {
  { E4, DUR_S },
  { REST, DUR_S },
  { E4, DUR_S },
  { G4, DUR_S },
  { E4, DUR_S },
  { REST, DUR_S },
  { G4, DUR_S },
  { REST, DUR_S },
  { A4, DUR_E },
  { A4, DUR_S },
  { REST, DUR_S },
  { A4, DUR_E },
  { E4, DUR_S },
  { REST, DUR_S },
  { E4, DUR_S },
  { G4, DUR_S },
  { E4, DUR_S },
  { REST, DUR_S },
  { F4, DUR_S },
  { FS4, DUR_S },
  { G4, DUR_E },
  { A4, DUR_E },
  { REST, DUR_E },
  { C5, DUR_E },
  { B4, DUR_S },
  { REST, DUR_S },
  { A4, DUR_E },
  { G4, DUR_S },
  { REST, DUR_S },
  { FS4, DUR_S },
  { G4, DUR_S },
  { A4, DUR_E },
  { REST, DUR_E },
  { G4, DUR_E },
  { REST, DUR_S },
  { G4, DUR_S },
  { A4, DUR_E },
  { G4, DUR_E },
  { E4, DUR_H },
  { D4, DUR_H },
  { E4, DUR_E },
  { E4, DUR_E },
  { D4, DUR_E },
  { C4, DUR_E },
  { G3, DUR_W },
};
const int gameover_melodia_len = sizeof(gameover_melodia) / sizeof(gameover_melodia[0]);

const int PROGMEM gameover_armonia[][2] = { { REST, DUR_W } };
const int gameover_armonia_len = sizeof(gameover_armonia) / sizeof(gameover_armonia[0]);

// PISTA 6: NUEVO RECORD (Fanfarria Triunfal)
const int PROGMEM record_melodia[][2] = {
  { C5, DUR_S },
  { C5, DUR_S },
  { C5, DUR_S },
  { REST, DUR_S },
  { C5, DUR_S },
  { E5, DUR_S },
  { G5, DUR_E },
  { E5, DUR_S },
  { G5, DUR_S },
  { C6, DUR_H },
  { G5, DUR_E },
  { FS5, DUR_E },
  { F5, DUR_E },
  { DS5, DUR_E },
  { E5, DUR_E },
  { REST, DUR_S },
  { GS4, DUR_S },
  { A4, DUR_S },
  { C5, DUR_S },
  { REST, DUR_S },
  { A4, DUR_S },
  { C5, DUR_S },
  { D5, DUR_E },
  { REST, DUR_E },
  { C5, DUR_S },
  { C5, DUR_S },
  { C5, DUR_S },
  { C5, DUR_S },
  { C5, DUR_S },
  { D5, DUR_S },
  { E5, DUR_E },
  { C5, DUR_S },
  { D5, DUR_S },
  { E5, DUR_E },
  { REST, DUR_S },
  { C5, DUR_S },
  { E5, DUR_E },
  { G5, DUR_W },
};
const int record_melodia_len = sizeof(record_melodia) / sizeof(record_melodia[0]);

const int PROGMEM record_armonia[][2] = { { REST, DUR_W } };
const int record_armonia_len = sizeof(record_armonia) / sizeof(record_armonia[0]);

// EFECTOS DE SONIDO
const int PROGMEM sfx_salto[][2] = {
  { C5, DUR_S },
  { D5, DUR_S },
  { E5, DUR_S },
  { G5, DUR_S },
  { A5, DUR_S },
  { C6, DUR_S },
};
const int sfx_salto_len = sizeof(sfx_salto) / sizeof(sfx_salto[0]);

const int PROGMEM sfx_dano[][2] = {
  { G5, DUR_S },
  { D5, DUR_S },
  { AS4, DUR_S },
  { FS4, DUR_S },
  { D4, DUR_S },
  { B3, DUR_S },
  { G3, DUR_E },
};
const int sfx_dano_len = sizeof(sfx_dano) / sizeof(sfx_dano[0]);

const int PROGMEM sfx_btna[][2] = {
  { C5, DUR_S },
  { E5, DUR_S },
  { G5, DUR_S },
};
const int sfx_btna_len = sizeof(sfx_btna) / sizeof(sfx_btna[0]);

const int PROGMEM sfx_btnb[][2] = {
  { G4, DUR_S },
  { C4, DUR_S },
};
const int sfx_btnb_len = sizeof(sfx_btnb) / sizeof(sfx_btnb[0]);
