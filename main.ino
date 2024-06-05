#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Pino de reset do leitor RFID
#define SS_PIN          10         // Pino SS do leitor RFID

#define LED_VERDE       7          // Pino digital para a LED verde
#define LED_VERMELHA    8          // Pino digital para a LED vermelha

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Criação do objeto MFRC522

String uidFound = "";
String lastUidFound = "";

bool state = false;

int linhaAtual, linhaAnterior;

unsigned long tempoInicial = 0; // Variável para armazenar o tempo inicial

String uids[][2] = {{"D3DAE4FC", "43A20FFD"},
                    {"033DEEFD", "B35321FD"},
                    {"11EF1247", "A3D4E1FC"},
                    {"F365BEFC", "53E493FD"},
                    {"F3AB6CFC", "C040531D"}};

void setup() {
    Serial.begin(9600);            // Inicializa a comunicação serial
    SPI.begin();                    // Inicializa a interface SPI
    mfrc522.PCD_Init();             // Inicializa o leitor RFID

    pinMode(LED_VERDE, OUTPUT);     // Configura o pino da LED verde como saída
    pinMode(LED_VERMELHA, OUTPUT);  // Configura o pino da LED vermelha como saída

    // Desliga todos os LEDs no início
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHA, LOW);

    Serial.println("Aproxime o primeiro cartão RFID...");
}

void loop() {
    // Verifica se o tempo decorrido é maior ou igual a 5 segundos
    // if (millis() - tempoInicial >= 5000) {
    //     // Reinicia o código
    //     setup();
    //     tempoInicial = millis(); // Atualiza o tempo inicial
    //     return; // Sai da função loop
    // }

    // Verifica se há cartões presentes
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        String uid = "";  // String para armazenar o UID do cartão

        // Lê o UID do cartão
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
            uid += String(mfrc522.uid.uidByte[i], HEX);
        }

        uid.toUpperCase();  // Converte o UID para letras maiúsculas

        uidFound = ""; // Flag para indicar se o UID foi encontrado no mapeamento
        // Verifica se o UID está presente no mapeamento
        //int linha;

        for (int i = 0; i < sizeof(uids) / sizeof(uids[0]); i++) {
            if (uids[i][0] == uid || uids[i][1] == uid) {
                linhaAtual = i;
                uidFound = uid;
                break;
            }
        }

        if(uidFound != "") {
          if(state == false) {
            Serial.println("Primeiro cartao detectado!");
            Serial.println("Aproxime o segundo cartao!");
            linhaAnterior = linhaAtual;
            lastUidFound = uidFound;
            state = true;
            digitalWrite(LED_VERDE, HIGH);    // Acende a LED verde
            digitalWrite(LED_VERMELHA, HIGH);    // Acende a LED verd
            delay(200);
            digitalWrite(LED_VERDE, LOW);  // Apaga a LED vermelha
            digitalWrite(LED_VERMELHA, LOW);
            delay(200);
            digitalWrite(LED_VERDE, HIGH);  // Apaga a LED vermelha
            digitalWrite(LED_VERMELHA, HIGH);
            delay(200);
            digitalWrite(LED_VERDE, LOW);  // Apaga a LED vermelha
            digitalWrite(LED_VERMELHA, LOW);
            delay(200);
          } else if(state == true && linhaAtual == linhaAnterior && lastUidFound != uidFound) {
            Serial.println("Acertou");
            linhaAnterior = -1;
            lastUidFound = "";
            state = false;
            digitalWrite(LED_VERDE, HIGH);    // Acende a LED verde
            delay(1000);
            digitalWrite(LED_VERDE, LOW);  // Apaga a LED vermelha
            delay(200);
            digitalWrite(LED_VERDE, HIGH);    // Acende a LED verde
            delay(1000);
            digitalWrite(LED_VERDE, LOW);  // Apaga a LED vermelha
          } else if(state == true && (linhaAtual != linhaAnterior || lastUidFound == uidFound)) {
            Serial.println("Errou");
            linhaAnterior = -1;
            lastUidFound = "";
            state = false;
            digitalWrite(LED_VERMELHA, HIGH);     // Apaga a LED verde
            delay(1000);
            digitalWrite(LED_VERMELHA, LOW); // Acende a LED vermelha
            delay(200);
            digitalWrite(LED_VERMELHA, HIGH); // Acende a LED vermelha
            delay(1000);
            digitalWrite(LED_VERMELHA, LOW); // Acende a LED vermelha
          }
        }

        // if (found) {
        //     Serial.println("Cartão válido.");
        // } else {
        //     Serial.println("Cartão inválido.");
        //     // Reinicia o código após 5 segundos
        //     tempoInicial = millis();
        // }
        
        delay(1000);  // Aguarda 1 segundo antes de verificar novamente
    }
}
