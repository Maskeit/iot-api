#include <HttpClient.h>
#include <LiquidCrystal_I2C_Spark.h>
// Configuración del LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Configuración del cliente HTTP
HttpClient http;
http_request_t request;
http_response_t response;
http_header_t headers[] = {
  { "Content-Type", "application/json" },
  { NULL, NULL } // Termina la lista de cabeceras con NULL
};

// Pines del teclado y mapeo de teclas
const int rowPins[4] = {D2, D3, D4, D5};
const int colPins[4] = {D6, D7, D8, D9};
char keyMap[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Variables para el PIN
char pinBuffer[5] = ""; // Buffer para PIN ingresado
int pinIndex = 0;

void setup() {
  Serial.begin(9600);
  
  // Inicializa el LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Ingrese PIN:");

  // Configuración de pines de filas y columnas del teclado
  for (int i = 0; i < 4; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
}

void loop() {
  char key = getKey();
  if (key) {
    // Solo aceptar números
    if (key >= '0' && key <= '9' && pinIndex < 4) {
      pinBuffer[pinIndex++] = key;
      pinBuffer[pinIndex] = '\0';

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ingrese PIN:");
      lcd.setCursor(0, 1);
      lcd.print(pinBuffer);
    }

    // Cuando el PIN tenga 4 dígitos
    if (pinIndex == 4) {
      delay(500); // Pequeña pausa para el usuario
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Verificando...");
      
      enviarPinAlServidor(pinBuffer);
      clearPinBuffer();
    }

    delay(300); // Evita múltiples lecturas del mismo botón
  }
}

// Función para leer el teclado
char getKey() {
  for (int row = 0; row < 4; row++) {
    digitalWrite(rowPins[row], LOW);
    for (int col = 0; col < 4; col++) {
      if (digitalRead(colPins[col]) == LOW) {
        digitalWrite(rowPins[row], HIGH);
        return keyMap[row][col];
      }
    }
    digitalWrite(rowPins[row], HIGH);
  }
  return 0;
}

// Función para limpiar el buffer de PIN
void clearPinBuffer() {
  memset(pinBuffer, 0, sizeof(pinBuffer));
  pinIndex = 0;
}

// Función para enviar el PIN al servidor
void enviarPinAlServidor(const char* pin) {
  char payload[64];
  snprintf(payload, sizeof(payload), "{\"pin\":\"%s\"}", pin);

  // Configuración de la solicitud HTTP
  request.hostname = "https://5a68-187-192-253-153.ngrok-free.app/"; // Cambia esto por tu URL
  request.port = 80;
  request.path = "/api/verificar_pin";
  request.body = payload;

  // Enviar la solicitud y obtener respuesta
  http.post(request, response, headers);
  Serial.println(response.status);
  Serial.println(response.body);

  // Verificar la respuesta y mostrar en el LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  if (response.status == 200) {
    if (response.body == "1") { // Autorizado
      lcd.print("Acceso Autorizado");
    } else { // No autorizado
      lcd.print("Acceso Denegado");
    }
  } else {
    lcd.print("Error Servidor");
  }

  delay(2000); // Pausa para mostrar el resultado
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ingrese PIN:");
}