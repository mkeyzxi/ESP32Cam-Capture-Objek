




// PIR diatur low sdngkan diatas HIGH
// #define CAMERA_MODEL_AI_THINKER

// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #include <UniversalTelegramBot.h>
// #include "esp_camera.h"
// #include <ArduinoJson.h>
// #include "camera_pins.h"

// // WiFi Configuration
// #define WIFI_SSID "fh_2b1c20"
// #define WIFI_PASSWORD "wland4e3df"

// // Telegram Bot
// #define BOT_TOKEN "8184393708:AAEJaIoapnhqWoZE0Ay92g2BO41eZIQlb7Y"
// #define CHAT_ID "1162574558"

// // PIN Definitions
// #define PIR_PIN 13
// #define BUZZER_PIN 12
// #define FLASH_LED_PIN 4

// // Telegram and Timing
// const unsigned long BOT_MTBS = 1000;
// unsigned long bot_lasttime = 0;

// WiFiClientSecure secured_client;
// UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// bool flashState = LOW;
// bool motionDetected = false;
// bool waitingForUserResponse = false;
// unsigned long lastDetectionTime = 0;
// const unsigned long debounceDelay = 10000;

// camera_fb_t *fb = NULL;
// bool dataAvailable = false;

// bool isMoreDataAvailable() {
//   if (dataAvailable) {
//     dataAvailable = false;
//     return true;
//   }
//   return false;
// }

// byte *getNextBuffer() {
//   return fb ? fb->buf : nullptr;
// }

// int getNextBufferLen() {
//   return fb ? fb->len : 0;
// }

// bool setupCamera() {
//   camera_config_t config;
//   config.ledc_channel = LEDC_CHANNEL_0;
//   config.ledc_timer = LEDC_TIMER_0;
//   config.pin_d0 = Y2_GPIO_NUM;
//   config.pin_d1 = Y3_GPIO_NUM;
//   config.pin_d2 = Y4_GPIO_NUM;
//   config.pin_d3 = Y5_GPIO_NUM;
//   config.pin_d4 = Y6_GPIO_NUM;
//   config.pin_d5 = Y7_GPIO_NUM;
//   config.pin_d6 = Y8_GPIO_NUM;
//   config.pin_d7 = Y9_GPIO_NUM;
//   config.pin_xclk = XCLK_GPIO_NUM;
//   config.pin_pclk = PCLK_GPIO_NUM;
//   config.pin_vsync = VSYNC_GPIO_NUM;
//   config.pin_href = HREF_GPIO_NUM;
//   config.pin_sscb_sda = SIOD_GPIO_NUM;
//   config.pin_sscb_scl = SIOC_GPIO_NUM;
//   config.pin_pwdn = PWDN_GPIO_NUM;
//   config.pin_reset = RESET_GPIO_NUM;
//   config.xclk_freq_hz = 20000000;
//   config.pixel_format = PIXFORMAT_JPEG;
//   config.frame_size = FRAMESIZE_QVGA;
//   config.jpeg_quality = 12;
//   config.fb_count = 1;

//   return esp_camera_init(&config) == ESP_OK;
// }

// void sendAlertPhoto() {
//   fb = esp_camera_fb_get();
//   if (!fb) {
//     bot.sendMessage(CHAT_ID, "Camera capture failed", "");
//     return;
//   }

//   dataAvailable = true;
//   bot.sendPhotoByBinary(CHAT_ID, "image/jpeg", fb->len,
//                         isMoreDataAvailable, nullptr,
//                         getNextBuffer, getNextBufferLen);
//   esp_camera_fb_return(fb);
//   fb = nullptr;

//   String msg = "⚠️ Ada sesuatu yang mencurigakan di rumah Anda! Berikut foto tangkapan kamera.\n\n";
//   msg += "Ketik *Ya* untuk mengaktifkan alarm, atau *Tidak* untuk mengabaikan.";
//   bot.sendMessage(CHAT_ID, msg, "Markdown");

//   waitingForUserResponse = true;
// }

// void handleUserResponse() {
//   int numMessages = bot.getUpdates(bot.last_message_received + 1);
//   for (int i = 0; i < numMessages; i++) {
//     String text = bot.messages[i].text;
//     text.toLowerCase();

//     if (text == "ambil foto") {
//       bot.sendMessage(CHAT_ID, "📸 Mengambil foto atas permintaan Anda...", "");
//       sendAlertPhoto();
//       continue;
//     }

//     if (text == "matikan alarm") {
//       digitalWrite(BUZZER_PIN, LOW);
//       bot.sendMessage(CHAT_ID, "🔕 Alarm dimatikan.", "");
//       continue;
//     }

//     if (text == "nyalakan alarm") {
//       digitalWrite(BUZZER_PIN, HIGH);
//       bot.sendMessage(CHAT_ID, "🚨 Alarm dinyalakan secara manual!", "");
//       continue;
//     }

//     if (!waitingForUserResponse) continue;

//     if (text == "ya") {
//       digitalWrite(BUZZER_PIN, HIGH);
//       bot.sendMessage(CHAT_ID, "🚨 Alarm diaktifkan!", "");
//     } else if (text == "tidak") {
//       bot.sendMessage(CHAT_ID, "✅ Sistem kembali siaga.", "");
//     }

//     waitingForUserResponse = false;
//     motionDetected = false;
//     lastDetectionTime = millis();
//   }
// }

// bool isMotionDetected() {
//   return digitalRead(PIR_PIN) == LOW;  // Logika dibalik: LOW = terdeteksi
// }

// void setup() {
//   Serial.begin(115200);
//   pinMode(FLASH_LED_PIN, OUTPUT);
//   digitalWrite(FLASH_LED_PIN, flashState);
//   pinMode(PIR_PIN, INPUT);
//   pinMode(BUZZER_PIN, OUTPUT);
//   digitalWrite(BUZZER_PIN, LOW);

//   if (!setupCamera()) {
//     Serial.println("Camera setup failed!");
//     while (true) delay(100);
//   }

//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//   secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print(".");
//     delay(500);
//   }

//   configTime(0, 0, "pool.ntp.org");
//   time_t now = time(nullptr);
//   while (now < 24 * 3600) {
//     delay(100);
//     now = time(nullptr);
//   }

//   Serial.println("WiFi connected. IP: " + WiFi.localIP().toString());
//   bot.sendMessage(CHAT_ID, "📷 Sistem keamanan ESP32-CAM siap digunakan!", "");
// }

// void loop() {
//   if (!waitingForUserResponse) {
//     if (isMotionDetected() && !motionDetected && millis() - lastDetectionTime > debounceDelay) {
//       Serial.println("📸 Gerakan terdeteksi!");
//       motionDetected = true;
//       sendAlertPhoto();
//     }
//   } else {
//     handleUserResponse();
//   }

//   if (millis() - bot_lasttime > BOT_MTBS) {
//     handleUserResponse();
//     bot_lasttime = millis();
//   }
// }
















// #define CAMERA_MODEL_AI_THINKER

// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #include <UniversalTelegramBot.h>
// #include "esp_camera.h"
// #include "camera_pins.h"

// // --- Additional Libraries for the Gate System ---
// #include <MFRC522.h>
// #include <SPI.h>
// #include <ESP32Servo.h>
// // --- End Additional Libraries ---

// // --- Wi-Fi Configuration ---
// #define WIFI_SSID "meenum"
// #define WIFI_PASSWORD "meenum0106"

// // --- Telegram Bot Configuration ---
// #define BOT_TOKEN "8184393708:AAEJaIoapnhqWoZE0Ay92g2BO41eZIQlb7Y"
// #define CHAT_ID "1162574558"

// // --- Hardware PIN Definitions ---
// // RFID (Using HSPI pins on ESP32-CAM)
// #define SS_PIN 15
// #define RST_PIN 2 // Be cautious with GPIO 2 for boot
// MFRC522 mfrc522(SS_PIN, RST_PIN);

// // PIR Sensor (NOW ONLY ONE PIR)
// #define PIR_PIN 32 // Pin for the single PIR sensor (GPIO 32 is a good choice for input)

// // Servo Motor
// #define SERVO_PIN_PAGAR 5 // Pin for Gate Servo (GPIO 5 recommended)
// Servo servoPagar;

// // Buzzer
// #define BUZZER_PIN 16

// // Light Sensor (LDR)
// #define LDR_PIN 34           // ADC pin for LDR
// #define THRESHOLD_MALAM 1000 // Threshold for detecting night

// #define FLASH_LED_PIN 4 // Pin for controlling the camera's Flash LED

// // --- Global Variables for Bot Control and Timing ---
// const unsigned long BOT_MTBS = 1000;
// unsigned long bot_lasttime = 0;

// WiFiClientSecure secured_client;
// UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// // --- Global Variables for System Status ---
// bool flashState = LOW;
// bool pagarTerbuka = false;
// unsigned long waktuBukaPagar = 0;
// const long durasiBukaPagar = 10000; // Duration the gate stays open in milliseconds (10 seconds)
// unsigned long lastPIRTriggerTime = 0; // To debounce PIR triggers for camera
// const long PIR_DEBOUNCE_TIME = 5000; // 5 seconds debounce for PIR camera trigger

// // For camera image data and Telegram photo sending
// camera_fb_t *fb = NULL;
// bool dataAvailable = false;

// // --- Authorized RFID Card UIDs ---
// const String AUTHORIZED_RFID_Uids[] = {
//     "CE3F291D", // Example UID 1
//     "DE123456"  // Example UID 2
// };
// const int NUM_AUTHORIZED_RFID = sizeof(AUTHORIZED_RFID_Uids) / sizeof(AUTHORIZED_RFID_Uids[0]);

// // --- Helper Functions for Telegram Photo Sending ---
// bool isMoreDataAvailable() {
//   if (dataAvailable) {
//     dataAvailable = false;
//     return true;
//   }
//   return false;
// }

// byte *getNextBuffer() {
//   return fb ? fb->buf : nullptr;
// }

// int getNextBufferLen() {
//   return fb ? fb->len : 0;
// }

// // --- ESP32-CAM Camera Initialization Function ---
// bool setupCamera() {
//   camera_config_t config;
//   config.ledc_channel = LEDC_CHANNEL_0;
//   config.ledc_timer = LEDC_TIMER_0;
//   config.pin_d0 = Y2_GPIO_NUM;
//   config.pin_d1 = Y3_GPIO_NUM;
//   config.pin_d2 = Y4_GPIO_NUM;
//   config.pin_d3 = Y5_GPIO_NUM;
//   config.pin_d4 = Y6_GPIO_NUM;
//   config.pin_d5 = Y7_GPIO_NUM;
//   config.pin_d6 = Y8_GPIO_NUM;
//   config.pin_d7 = Y9_GPIO_NUM;
//   config.pin_xclk = XCLK_GPIO_NUM;
//   config.pin_pclk = PCLK_GPIO_NUM;
//   config.pin_vsync = VSYNC_GPIO_NUM;
//   config.pin_href = HREF_GPIO_NUM;
//   config.pin_sscb_sda = SIOD_GPIO_NUM;
//   config.pin_sscb_scl = SIOC_GPIO_NUM;
//   config.pin_pwdn = PWDN_GPIO_NUM;
//   config.pin_reset = RESET_GPIO_NUM;
//   config.xclk_freq_hz = 20000000;
//   config.pixel_format = PIXFORMAT_JPEG;
//   config.frame_size = FRAMESIZE_VGA;
//   config.jpeg_quality = 12;
//   config.fb_count = 1;

//   esp_err_t err = esp_camera_init(&config);
//   if (err != ESP_OK) {
//     Serial.printf("Camera init failed with error 0x%x\n", err);
//     return false;
//   }
//   return true;
// }

// // --- Gate and Buzzer Control Functions ---
// void bukaPagar() {
//   if (!pagarTerbuka) {
//     Serial.println("Opening Gate...");
//     servoPagar.write(90);
//     delay(1000);
//     pagarTerbuka = true;
//     waktuBukaPagar = millis();
//     bot.sendMessage(CHAT_ID, "Pagar telah dibuka.", "");
//   }
// }

// void tutupPagar() {
//   if (pagarTerbuka) {
//     Serial.println("Closing Gate...");
//     servoPagar.write(0);
//     delay(1000);
//     pagarTerbuka = false;
//     bot.sendMessage(CHAT_ID, "Pagar telah ditutup.", "");
//   }
// }

// void bunyiBuzzer() {
//   digitalWrite(BUZZER_PIN, HIGH);
//   delay(200);
//   digitalWrite(BUZZER_PIN, LOW);
//   delay(100);
// }

// // --- Function for Sending Alert Photos to Telegram ---
// void sendAlertPhoto() {
//   int ldrValue = analogRead(LDR_PIN);
//   Serial.print("LDR Value: ");
//   Serial.println(ldrValue);

//   if (ldrValue < THRESHOLD_MALAM) {
//     Serial.println("It's night, turning on flash...");
//     digitalWrite(FLASH_LED_PIN, HIGH);
//     delay(500);
//   }

//   fb = esp_camera_fb_get();
//   if (!fb) {
//     bot.sendMessage(CHAT_ID, "Camera capture failed", "");
//     if (ldrValue < THRESHOLD_MALAM) {
//       digitalWrite(FLASH_LED_PIN, LOW);
//     }
//     return;
//   }

//   dataAvailable = true;
//   bot.sendPhotoByBinary(CHAT_ID, "image/jpeg", fb->len,
//                         isMoreDataAvailable, nullptr,
//                         getNextBuffer, getNextBufferLen);
//   esp_camera_fb_return(fb);
//   fb = nullptr;

//   if (ldrValue < THRESHOLD_MALAM) {
//     digitalWrite(FLASH_LED_PIN, LOW);
//   }

//   String msg = "⚠️ Gerakan terdeteksi di pagar! Berikut foto tangkapan kamera.\n\n";
//   msg += "Anda dapat membuka pagar dengan mengirim */bukapagar* atau tutup dengan */tutuppagar*.";
//   bot.sendMessage(CHAT_ID, msg, "Markdown");
// }

// // --- Function for Handling Messages from Telegram ---
// void handleTelegramMessages() {
//   int numMessages = bot.getUpdates(bot.last_message_received + 1);
//   for (int i = 0; i < numMessages; i++) {
//     String chat_id = bot.messages[i].chat_id;
//     String text = bot.messages[i].text;
//     text.toLowerCase();

//     Serial.print("Received message from ");
//     Serial.print(chat_id);
//     Serial.print(": ");
//     Serial.println(text);

//     if (chat_id != CHAT_ID) {
//       bot.sendMessage(chat_id, "Anda tidak memiliki izin untuk mengontrol bot ini.", "");
//       continue;
//     }

//     if (text == "/bukapagar") {
//       bukaPagar();
//     } else if (text == "/tutuppagar") {
//       tutupPagar();
//     } else if (text == "/status") {
//       if (pagarTerbuka) {
//         bot.sendMessage(chat_id, "Status Pagar: TERBUKA", "");
//       } else {
//         bot.sendMessage(CHAT_ID, "Status Pagar: TERTUTUP", "");
//       }
//     } else if (text == "/ambil_foto") {
//       bot.sendMessage(CHAT_ID, "📸 Mengambil foto atas permintaan Anda...", "");
//       sendAlertPhoto();
//     } else {
//       bot.sendMessage(CHAT_ID, "Perintah tidak dikenal. Gunakan /bukapagar, /tutuppagar, /status, atau /ambil_foto.", "");
//     }
//   }
// }

// // --- setup() Function - System Initialization ---
// void setup() {
//   Serial.begin(115200);

//   // Initialize Input/Output Pins
//   pinMode(PIR_PIN, INPUT); // Only one PIR pin
//   pinMode(BUZZER_PIN, OUTPUT);
//   digitalWrite(BUZZER_PIN, LOW);
//   pinMode(LDR_PIN, INPUT);
//   pinMode(FLASH_LED_PIN, OUTPUT);
//   digitalWrite(FLASH_LED_PIN, LOW);

//   // Initialize Servo
//   servoPagar.attach(SERVO_PIN_PAGAR);
//   tutupPagar();

//   // Connect to Wi-Fi
//   Serial.print("Connecting to WiFi ");
//   Serial.println(WIFI_SSID);
//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//   secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP().toString());

//   // Initialize Camera
//   if (!setupCamera()) {
//     Serial.println("Camera setup failed! Rebooting in 5 seconds...");
//     delay(5000);
//     ESP.restart();
//   }

//   // Initialize RFID Reader
//   SPI.begin(14, 12, 13, 15);
//   mfrc522.PCD_Init();
//   Serial.println("RFID Reader Ready");

//   // Time Synchronization
//   configTime(0, 0, "pool.ntp.org");
//   time_t now = time(nullptr);
//   while (now < 24 * 3600) {
//     delay(100);
//     now = time(nullptr);
//   }

//   Serial.println("System Ready!");
//   bot.sendMessage(CHAT_ID, "📷 Sistem Pagar Otomatis ESP32-CAM siap digunakan!", "");
// }

// // --- loop() Function - Main System Logic ---
// void loop() {
//   // ==============================
//   // Flow 1: Access Using RFID
//   // ==============================
//   if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
//     Serial.println("RFID Card Detected!");
//     String cardUID = "";
//     for (byte i = 0; i < mfrc522.uid.size; i++) {
//       if (mfrc522.uid.uidByte[i] < 0x10) {
//         cardUID += "0";
//       }
//       cardUID += String(mfrc522.uid.uidByte[i], HEX);
//     }
//     cardUID.toUpperCase();
//     Serial.print("Card UID: ");
//     Serial.println(cardUID);

//     bool authorized = false;
//     for (int i = 0; i < NUM_AUTHORIZED_RFID; i++) {
//       if (cardUID == AUTHORIZED_RFID_Uids[i]) {
//         authorized = true;
//         break;
//       }
//     }

//     if (authorized) {
//       bunyiBuzzer();
//       bukaPagar();
//       Serial.println("Gate opened via RFID.");
//     } else {
//       Serial.println("Card not recognized.");
//       bunyiBuzzer();
//       delay(300);
//       bunyiBuzzer();
//       bot.sendMessage(CHAT_ID, "‼️ Peringatan: Ada upaya akses RFID dengan kartu tidak dikenal.", "");
//     }

//     mfrc522.PICC_HaltA();
//     mfrc522.PCD_StopCrypto1();
//   }

//   // ==============================
//   // Flow 2: Object Detection by PIR & ESP32-CAM Notification (PIR is only active when gate is closed)
//   // ==============================
//   // Only trigger camera if gate is closed AND PIR detects motion AND debounce time has passed
//   if (!pagarTerbuka && digitalRead(PIR_PIN) == HIGH) {
//     if (millis() - lastPIRTriggerTime > PIR_DEBOUNCE_TIME) {
//       Serial.println("Motion detected by PIR (camera trigger)!");
//       bunyiBuzzer();
//       sendAlertPhoto();
//       lastPIRTriggerTime = millis();
//     }
//   }

//   // ==============================
//   // Automatic Gate Closing Logic (uses the same PIR)
//   // ==============================
//   // If the gate is open AND the specified time for the gate to be open has elapsed
//   if (pagarTerbuka && (millis() - waktuBukaPagar >= durasiBukaPagar)) {
//     Serial.println("Gate open time elapsed, checking PIR for object presence.");
//     delay(500); // Give a little time for PIR reading to stabilize

//     // Close the gate only if PIR detects NO object
//     if (digitalRead(PIR_PIN) == LOW) {
//       Serial.println("No object detected, closing gate.");
//       tutupPagar();
//     } else {
//       Serial.println("Object still detected, delaying gate closing.");
//       // Reset timer so the gate remains open as long as an object is detected
//       waktuBukaPagar = millis();
//     }
//   }

//   // ==============================
//   // Telegram Message Handling
//   // ==============================
//   if (millis() - bot_lasttime > BOT_MTBS) {
//     handleTelegramMessages();
//     bot_lasttime = millis();
//   }

//   delay(50);
// }






















// // alur 2
// #define CAMERA_MODEL_AI_THINKER

// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #include <UniversalTelegramBot.h>
// #include "esp_camera.h"
// #include <ArduinoJson.h>
// #include "camera_pins.h"
// #include <ESP32Servo.h>  // Ganti dari Servo.h ke ESP32Servo

// // ========================= CONFIG =========================
// #define WIFI_SSID "fh_2b1c20"
// #define WIFI_PASSWORD "wland4e3df"

// #define BOT_TOKEN "8184393708:AAEJaIoapnhqWoZE0Ay92g2BO41eZIQlb7Y"
// #define CHAT_ID "1162574558"

// // ========================= PIN ============================
// #define PIR_PIN 13
// #define BUZZER_PIN 12
// #define FLASH_LED_PIN 4
// #define LDR_PIN 34
// #define SERVO_PIN 14

// // ======================== OBJEK ===========================
// WiFiClientSecure secured_client;
// UniversalTelegramBot bot(BOT_TOKEN, secured_client);
// Servo gateServo;

// camera_fb_t *fb = NULL;

// // =================== VARIABEL STATUS ======================
// const unsigned long BOT_MTBS = 1000;
// unsigned long bot_lasttime = 0;

// bool flashState = LOW;
// bool motionDetected = false;
// bool waitingForUserResponse = false;
// bool gateOpen = false;
// unsigned long lastDetectionTime = 0;
// const unsigned long debounceDelay = 10000;

// bool dataAvailable = false;

// // ===================== KAMERA =============================
// bool setupCamera() {
//   camera_config_t config;
//   config.ledc_channel = LEDC_CHANNEL_0;
//   config.ledc_timer = LEDC_TIMER_0;
//   config.pin_d0 = Y2_GPIO_NUM;
//   config.pin_d1 = Y3_GPIO_NUM;
//   config.pin_d2 = Y4_GPIO_NUM;
//   config.pin_d3 = Y5_GPIO_NUM;
//   config.pin_d4 = Y6_GPIO_NUM;
//   config.pin_d5 = Y7_GPIO_NUM;
//   config.pin_d6 = Y8_GPIO_NUM;
//   config.pin_d7 = Y9_GPIO_NUM;
//   config.pin_xclk = XCLK_GPIO_NUM;
//   config.pin_pclk = PCLK_GPIO_NUM;
//   config.pin_vsync = VSYNC_GPIO_NUM;
//   config.pin_href = HREF_GPIO_NUM;
//   config.pin_sscb_sda = SIOD_GPIO_NUM;
//   config.pin_sscb_scl = SIOC_GPIO_NUM;
//   config.pin_pwdn = PWDN_GPIO_NUM;
//   config.pin_reset = RESET_GPIO_NUM;
//   config.xclk_freq_hz = 20000000;
//   config.pixel_format = PIXFORMAT_JPEG;
//   config.frame_size = FRAMESIZE_QVGA;
//   config.jpeg_quality = 12;
//   config.fb_count = 1;

//   return esp_camera_init(&config) == ESP_OK;
// }

// // ========== FUNGSI BUFFER KAMERA UNTUK TELEGRAM ===========
// bool isMoreDataAvailable() {
//   if (dataAvailable) {
//     dataAvailable = false;
//     return true;
//   }
//   return false;
// }
// byte *getNextBuffer() {
//   return fb ? fb->buf : nullptr;
// }
// int getNextBufferLen() {
//   return fb ? fb->len : 0;
// }

// // ============= FUNGSI UTAMA UNTUK KIRIM FOTO ==============
// void sendAlertPhoto() {
//   int lightValue = analogRead(LDR_PIN);
//   if (lightValue < 1000) {
//     digitalWrite(FLASH_LED_PIN, HIGH);
//     delay(300);
//   }

//   fb = esp_camera_fb_get();
//   if (!fb) {
//     bot.sendMessage(CHAT_ID, "Camera capture failed", "");
//     return;
//   }

//   dataAvailable = true;
//   bot.sendPhotoByBinary(CHAT_ID, "image/jpeg", fb->len,
//                         isMoreDataAvailable, nullptr,
//                         getNextBuffer, getNextBufferLen);
//   esp_camera_fb_return(fb);
//   fb = nullptr;

//   digitalWrite(FLASH_LED_PIN, LOW);

//   String msg = "🚨 *Gerakan terdeteksi!*";
//   msg += "\n\nBalas:\n*ya* → Aktifkan alarm\n*tidak* → Abaikan\n*buka pagar* → Buka pagar";
//   bot.sendMessage(CHAT_ID, msg, "Markdown");

//   waitingForUserResponse = true;
// }

// // =================== RESPON DARI TELEGRAM ==================
// void handleUserResponse() {
//   int numMessages = bot.getUpdates(bot.last_message_received + 1);
//   for (int i = 0; i < numMessages; i++) {
//     String text = bot.messages[i].text;
//     text.toLowerCase();

//     if (text == "ambil foto") {
//       sendAlertPhoto();
//       continue;
//     }

//     if (text == "nyalakan alarm") {
//       digitalWrite(BUZZER_PIN, HIGH);
//       bot.sendMessage(CHAT_ID, "🔔 Alarm dinyalakan!", "");
//       continue;
//     }

//     if (text == "matikan alarm") {
//       digitalWrite(BUZZER_PIN, LOW);
//       bot.sendMessage(CHAT_ID, "🔕 Alarm dimatikan.", "");
//       continue;
//     }

//     if (text == "buka pagar") {
//       gateServo.write(90);
//       bot.sendMessage(CHAT_ID, "🔓 Pagar dibuka.", "");
//       gateOpen = true;
//       continue;
//     }

//     if (text == "tutup pagar") {
//       gateServo.write(0);
//       bot.sendMessage(CHAT_ID, "🔒 Pagar ditutup.", "");
//       gateOpen = false;
//       continue;
//     }

//     if (!waitingForUserResponse) continue;

//     if (text == "ya") {
//       digitalWrite(BUZZER_PIN, HIGH);
//       bot.sendMessage(CHAT_ID, "🚨 Alarm aktif!", "");
//     } else if (text == "tidak") {
//       bot.sendMessage(CHAT_ID, "✅ Peringatan diabaikan.", "");
//     }

//     waitingForUserResponse = false;
//     motionDetected = false;
//     lastDetectionTime = millis();
//   }
// }

// // ======================== SETUP ============================
// void setup() {
//   Serial.begin(115200);
//   pinMode(FLASH_LED_PIN, OUTPUT);
//   pinMode(PIR_PIN, INPUT);
//   pinMode(BUZZER_PIN, OUTPUT);
//   digitalWrite(BUZZER_PIN, LOW);
//   pinMode(LDR_PIN, INPUT);

//   gateServo.setPeriodHertz(50);  // 50Hz = standar servo
//   gateServo.attach(SERVO_PIN, 500, 2400);  // PWM min-max mikrodetik
//   gateServo.write(0);  // posisi tutup

//   if (!setupCamera()) {
//     Serial.println("Camera setup failed!");
//     while (true) delay(100);
//   }

//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//   secured_client.setInsecure();  // jika sertifikat gagal
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   configTime(0, 0, "pool.ntp.org");
//   time_t now = time(nullptr);
//   while (now < 24 * 3600) {
//     delay(100);
//     now = time(nullptr);
//   }

//   Serial.println("WiFi connected. IP: " + WiFi.localIP().toString());
//   bot.sendMessage(CHAT_ID, "✅ Sistem ESP32-CAM siap!", "");
// }

// // ========================= LOOP ============================
// void loop() {
//   if (!waitingForUserResponse) {
//     if (digitalRead(PIR_PIN) == LOW && !motionDetected && millis() - lastDetectionTime > debounceDelay) {
//       Serial.println("📸 Gerakan Terdeteksi!");
//       motionDetected = true;
//       sendAlertPhoto();
//     }
//   } else {
//     handleUserResponse();
//   }

//   if (gateOpen && digitalRead(PIR_PIN) == HIGH) {
//     delay(5000);
//     gateServo.write(0);
//     bot.sendMessage(CHAT_ID, "🔒 Pagar ditutup otomatis karena objek telah lewat.", "");
//     gateOpen = false;
//   }

//   if (millis() - bot_lasttime > BOT_MTBS) {
//     handleUserResponse();
//     bot_lasttime = millis();
//   }
// }


























#define CAMERA_MODEL_AI_THINKER

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "esp_camera.h"
#include <ArduinoJson.h>
#include "camera_pins.h"
#include <ESP32Servo.h>

// ========================= CONFIG =========================
#define WIFI_SSID "fh_2b1c20"
#define WIFI_PASSWORD "wland4e3df"

#define BOT_TOKEN "8184393708:AAEJaIoapnhqWoZE0Ay92g2BO41eZIQlb7Y"
#define CHAT_ID "1162574558"

// ========================= PIN ============================
#define PIR_PIN 13
#define BUZZER_PIN 12
#define FLASH_LED_PIN 4
#define LDR_PIN 34
#define SERVO_PIN 14

// ======================== OBJEK ===========================
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
Servo gateServo;

camera_fb_t *fb = NULL;

// =================== VARIABEL STATUS ======================
const unsigned long BOT_MTBS = 1000;
unsigned long bot_lasttime = 0;

bool motionDetected = false;
bool waitingForUserResponse = false;
bool gateOpen = false;
unsigned long lastDetectionTime = 0;
const unsigned long debounceDelay = 10000;

bool dataAvailable = false;

// ===================== KAMERA =============================
bool setupCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  return esp_camera_init(&config) == ESP_OK;
}

// ========== FUNGSI BUFFER KAMERA UNTUK TELEGRAM ===========
bool isMoreDataAvailable() {
  if (dataAvailable) {
    dataAvailable = false;
    return true;
  }
  return false;
}
byte *getNextBuffer() {
  return fb ? fb->buf : nullptr;
}
int getNextBufferLen() {
  return fb ? fb->len : 0;
}

// ============= FUNGSI UTAMA UNTUK KIRIM FOTO ==============
void sendAlertPhoto() {
  Serial.println("📸 Mengambil foto...");

  digitalWrite(FLASH_LED_PIN, HIGH);
  delay(300);

  fb = esp_camera_fb_get();
  if (!fb) {
    bot.sendMessage(CHAT_ID, "❌ Gagal ambil foto!", "");
    return;
  }

  dataAvailable = true;
  bot.sendPhotoByBinary(CHAT_ID, "image/jpeg", fb->len,
                        isMoreDataAvailable, nullptr,
                        getNextBuffer, getNextBufferLen);
  esp_camera_fb_return(fb);
  fb = nullptr;

  digitalWrite(FLASH_LED_PIN, LOW);

  String msg = "🚨 *Gerakan terdeteksi!*";
  msg += "\nBalas:\n*ya* → Aktifkan alarm\n*tidak* → Abaikan\n*buka pagar* → Buka pagar";
  bot.sendMessage(CHAT_ID, msg, "Markdown");

  waitingForUserResponse = true;
}

// =================== RESPON DARI TELEGRAM ==================
void handleUserResponse() {
  int numMessages = bot.getUpdates(bot.last_message_received + 1);
  for (int i = 0; i < numMessages; i++) {
    String text = bot.messages[i].text;
    text.toLowerCase();

    if (text == "ambil foto") {
      sendAlertPhoto();
      continue;
    }

    if (text == "nyalakan alarm") {
      digitalWrite(BUZZER_PIN, HIGH);
      bot.sendMessage(CHAT_ID, "🔔 Alarm dinyalakan!", "");
      continue;
    }

    if (text == "matikan alarm") {
      digitalWrite(BUZZER_PIN, LOW);
      bot.sendMessage(CHAT_ID, "🔕 Alarm dimatikan.", "");
      continue;
    }

    if (text == "buka pagar") {
      gateServo.write(180);
      bot.sendMessage(CHAT_ID, "🔓 Pagar dibuka.", "");
      gateOpen = true;
      continue;
    }

    if (text == "tutup pagar") {
      gateServo.write(0);
      bot.sendMessage(CHAT_ID, "🔒 Pagar ditutup.", "");
      gateOpen = false;
      continue;
    }

    if (!waitingForUserResponse) continue;

    if (text == "ya") {
      digitalWrite(BUZZER_PIN, HIGH);
      bot.sendMessage(CHAT_ID, "🚨 Alarm aktif!", "");
    } else if (text == "tidak") {
      bot.sendMessage(CHAT_ID, "✅ Peringatan diabaikan.", "");
    }

    waitingForUserResponse = false;
    motionDetected = false;
    lastDetectionTime = millis();
  }
}

// ======================== SETUP ============================
void setup() {
  Serial.begin(115200);
  pinMode(FLASH_LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  gateServo.setPeriodHertz(50);
  gateServo.attach(SERVO_PIN, 500, 2400);
  gateServo.write(0);

  if (!setupCamera()) {
    Serial.println("Camera setup failed!");
    while (true) delay(100);
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setInsecure();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  configTime(0, 0, "pool.ntp.org");
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    delay(100);
    now = time(nullptr);
  }

  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());
  bot.sendMessage(CHAT_ID, "✅ Sistem siap digunakan!", "");
}

// ========================= LOOP ============================
void loop() {
  int pirState = digitalRead(PIR_PIN);
  if (!waitingForUserResponse && pirState == HIGH && !motionDetected && millis() - lastDetectionTime > debounceDelay) {
    Serial.println("👀 Gerakan terdeteksi!");
    motionDetected = true;
    lastDetectionTime = millis();
    sendAlertPhoto();
  }

  if (gateOpen && pirState == LOW && millis() - lastDetectionTime > 10000) {
    gateServo.write(0);
    bot.sendMessage(CHAT_ID, "🔒 Pagar ditutup otomatis.", "");
    gateOpen = false;
  }

  if (millis() - bot_lasttime > BOT_MTBS) {
    handleUserResponse();
    bot_lasttime = millis();
  }
}
