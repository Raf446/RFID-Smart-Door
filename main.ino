#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// --- Konfigurasi Pin RFID ---
#define SS_PIN 10
#define RST_PIN 9

// --- Konfigurasi Pin Servo ---
#define SERVO_PIN 3

// --- Konfigurasi Pin LED RGB (Pin PWM) ---
#define LED_R 5
#define LED_G 6
#define LED_B 9

// --- Data Akses Kartu ---
String UID_VALID = "15 75 FE 28"; // Ganti dengan UID kartu Anda
byte lockStatus = 0; // 0 = Tertutup, 1 = Terbuka

// --- Inisialisasi Objek ---
MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

// --- Data Running Text ---
String barisAtas = "   Halo! Silahkan   ";
String barisBawah = "   Tempel kartu kamu!   ";
int textIndex = 0;
unsigned long previousMillis = 0;
const long textInterval = 250; // Kecepatan pergeseran teks (ms)

// Fungsi pembantu untuk mengatur warna LED RGB (Common Cathode)
void setLED(int red, int green, int blue) {
  analogWrite(LED_R, red);
  analogWrite(LED_G, green);
  analogWrite(LED_B, blue);
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(70); // Posisi awal pintu terkunci (70 derajat)

  // Status awal: Merah (Pintu Terkunci)
  setLED(255, 0, 0);
}

void loop() {
  // Running text non-blocking (agar pembacaan RFID tidak terhambat delay)
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= textInterval) {
    previousMillis = currentMillis;

    lcd.setCursor(0, 0);
    lcd.print(barisAtas.substring(textIndex, textIndex + 16));

    lcd.setCursor(0, 1);
    lcd.print(barisBawah.substring(textIndex, textIndex + 16));

    textIndex++;
    if (textIndex > 18) {
      textIndex = 0;
    }
  }

  // Cek apakah ada kartu yang ditempelkan
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Scanning kartu (LED Biru menyala)
  setLED(0, 0, 255);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scanning");

  String scannedUID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    lcd.print(".");
    scannedUID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    scannedUID.concat(String(rfid.uid.uidByte[i], HEX));
    delay(200);
  }
  scannedUID.toUpperCase();
  scannedUID = scannedUID.substring(1);

  Serial.print("Kartu terbaca: ");
  Serial.println(scannedUID);

  // Verifikasi Kartu
  if (scannedUID == UID_VALID) {
    if (lockStatus == 0) {
      // Buka Pintu
      myServo.write(160);
      setLED(0, 255, 0); // Hijau
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Akses Diterima");
      lcd.setCursor(1, 1);
      lcd.print("Pintu Terbuka!");
      delay(2000);
      lockStatus = 1;
    } else {
      // Kunci Pintu
      myServo.write(70);
      setLED(255, 0, 0); // Merah
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Akses Diterima");
      lcd.setCursor(1, 1);
      lcd.print("Pintu Tertutup");
      delay(2000);
      lockStatus = 0;
    }
  } else {
    // Kartu Salah
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Akses Ditolak");
    lcd.setCursor(2, 1);
    lcd.print("Kartu Salah!");
    
    // Kedip merah tanda peringatan
    for (int i = 0; i < 3; i++) {
      setLED(255, 0, 0);
      delay(150);
      setLED(0, 0, 0);
      delay(150);
    }
  }

  // Kembalikan tampilan LCD dan warna LED sesuai status
  lcd.clear();
  textIndex = 0;
  if (lockStatus == 0) {
    setLED(255, 0, 0);
  } else {
    setLED(0, 255, 0);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}