// Modulo para la funcionalidad del teclado matricial
#include "teclado.h"
#include "config_pin.h"

Keypad getKeypad() {
  return  Keypad( makeKeymap(hexaKeys), rowPins, colPins, TECLADO_ROWS, TECLADO_COLS);
}
