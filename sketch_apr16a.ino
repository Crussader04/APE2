const int ledPins[] = {2, 3, 4, 5, 6, 7};
const int switchPin = 8; // Cambié el nombre para que tenga más sentido

int patronActual = 0;
bool estadoEncendido = false; 

// Temporizador para cambio automático
unsigned long tiempoInicioPatron = 0;
const unsigned long DURACION_PATRON = 500; // 5 segundos por patrón

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(switchPin, INPUT);
  randomSeed(analogRead(0)); 
}

void loop() {
  // Leemos la posición física exacta del interruptor
  bool lecturaActual = digitalRead(switchPin);

  // Si el switch acaba de ser encendido (pasó de apagado a prendido)
  if (lecturaActual == HIGH && !estadoEncendido) {
    patronActual = random(0, 5);   // Elegimos un patrón al azar para empezar
    tiempoInicioPatron = millis(); // Empezamos a contar los 5 segundos
  }

  // Sincronizamos la variable interna con la posición física del switch
  estadoEncendido = lecturaActual;

  if (estadoEncendido) {
    
    // Verificamos si ya pasó el tiempo para cambiar de patrón automáticamente
    if (millis() - tiempoInicioPatron >= DURACION_PATRON) {
      int nuevoPatron = random(0, 5);
      
      // Evita que salga el mismo patrón dos veces seguidas
      while (nuevoPatron == patronActual) {
        nuevoPatron = random(0, 5);
      }
      
      patronActual = nuevoPatron;
      tiempoInicioPatron = millis();
      apagarTodos(); // Limpia los LEDs antes del nuevo patrón
    }

    // Ejecuta el patrón actual
    switch (patronActual) {
      case 0: patronSecuencia(); break;
      case 1: patronPersecucion(); break;
      case 2: patronParpadeo(); break;
      case 3: patronAleatorio(); break;
      case 4: patronOnda(); break;
    }
  } else {
    // Si el switch está en LOW (apagado), mantenemos todo apagado
    apagarTodos();
  }
}

// ---------------------------------------------------------
// FUNCIÓN CLAVE: Mucho más simple. Solo vigila si apagas el switch.
// ---------------------------------------------------------
bool esperarYRevisar(unsigned long tiempoMs) {
  unsigned long inicio = millis();
  
  while (millis() - inicio < tiempoMs) {
    // Si en cualquier momento del patrón mueves el switch a apagado (LOW)
    if (digitalRead(switchPin) == LOW) {
      apagarTodos();
      return true; // Rompe la animación y sale inmediatamente
    }
  }
  return false; 
}

void apagarTodos() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

// ==========================================
// IMPLEMENTACIÓN DE LOS PATRONES
// (Sin modificaciones, funcionan perfecto)
// ==========================================

void patronSecuencia() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPins[i], HIGH);
    if (esperarYRevisar(200)) return; 
    digitalWrite(ledPins[i], LOW);
  }
}

void patronPersecucion() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPins[i], HIGH);
    if (esperarYRevisar(150)) return;
  }
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPins[i], LOW);
    if (esperarYRevisar(100)) return;
  }
}

void patronParpadeo() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
  if (esperarYRevisar(300)) return;
  
  apagarTodos();
  if (esperarYRevisar(300)) return;
}

void patronAleatorio() {
  int pinAlAzar = random(0, 6);
  digitalWrite(ledPins[pinAlAzar], HIGH);
  if (esperarYRevisar(100)) return;
  digitalWrite(ledPins[pinAlAzar], LOW);
}

void patronOnda() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPins[i], HIGH);
    if (esperarYRevisar(100)) return;
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = 4; i > 0; i--) { 
    digitalWrite(ledPins[i], HIGH);
    if (esperarYRevisar(100)) return;
    digitalWrite(ledPins[i], LOW);
  }
}