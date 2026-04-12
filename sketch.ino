#include <DHT.h>               // DHT sensör kütüphanesi
#include <Wire.h>              // I2C iletişim kütüphanesi
#include <LiquidCrystal_I2C.h> // I2C LCD kütüphanesi

#define RED_PIN 25    // RGB LED kırmızı pin
#define GREEN_PIN 26  // RGB LED yeşil pin
#define BLUE_PIN 27   // RGB LED mavi pin
#define DHTPIN 15     // DHT22 data pini
#define DHTTYPE DHT22 // Sensör tipi DHT22
#define LDR_PIN 34    // LDR analog giriş pini
#define BUTTON_PIN 18 // Buton pini

DHT dht(DHTPIN, DHTTYPE);           // DHT sensör nesnesi
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD nesnesi

int lastButtonState = HIGH; // Önceki buton durumu
int currentButtonState;     // Anlık buton durumu

unsigned long previousMillis = 0;    // Son sensör okuma zamanı
const long interval = 3000;          // Sensör okuma aralığı (3 sn)
unsigned long previousLCDMillis = 0; // Son LCD güncelleme zamanı
int lcdInterval = 2000;              // LCD gösterim süresi (2 sn)
int ekranIndex = 0;                  // 0=NEM, 1=SICAKLIK, 2=ISIK

float humidity = 0;    // Nem değeri
float temperature = 0; // Sıcaklık değeri
int lux = 500;         // Işık değeri

unsigned long lastDebounceTime = 0; // Son debounce zamanı
const long debounceDelay = 200;     // Debounce gecikmesi (ms)

// RGB LED'e renk yaz (pinMode OUTPUT + analogWrite ile)
void setRGB(int r, int g, int b)
{
  analogWrite(RED_PIN, r);   // Kırmızı pine PWM yaz
  analogWrite(GREEN_PIN, g); // Yeşil pine PWM yaz
  analogWrite(BLUE_PIN, b);  // Mavi pine PWM yaz
}

// Nem durumu: %60 ve altı LOW, üstü HIGH
String nemDurumu()
{
  if (humidity <= 60)
    return "LOW";
  else
    return "HIGH";
}

// Sıcaklık durumu
String sicaklikDurumu()
{
  if (temperature < 0)
    return "VERY LOW";
  else if (temperature <= 30)
    return "NORMAL";
  else
    return "HIGH";
}

// Işık durumu
String isikDurumu()
{
  if (lux < 10)
    return "KARANLIK";
  else
    return "AYDINLIK";
}

// LED'i sensör değerlerine göre güncelle
void updateLED()
{
  if (lux < 200)
  {
    setRGB(0, 0, 255); // KARANLIK → Mavi
  }
  else
  {
    if (temperature < 0)
    {
      setRGB(0, 150, 255); // VERY LOW  → Açık mavi
    }
    else if (temperature <= 30)
    {
      setRGB(0, 255, 0); // NORMAL    → Yeşil
    }
    else if (temperature <= 40)
    {
      setRGB(255, 165, 0); // HIGH      → Turuncu
    }
    else
    {
      setRGB(255, 0, 0); // VERY HIGH → Kırmızı
    }
  }
}

// LCD ekranı güncelle
void updateLCD()
{
  lcd.clear();                 // Ekranı temizle
  int sn = lcdInterval / 1000; // Süreyi saniyeye çevir

  if (ekranIndex == 0)
  { // NEM ekranı
    lcd.setCursor(4, 0);
    lcd.print("NEM"); // Başlık
    lcd.setCursor(15, 0);
    lcd.print(sn); // Sağ üst köşe: aktif süre
    lcd.setCursor(2, 1);
    lcd.print(humidity, 2); // Nem değeri
    lcd.print("-");
    lcd.print(nemDurumu()); // LOW veya HIGH
  }
  else if (ekranIndex == 1)
  { // SICAKLIK ekranı
    lcd.setCursor(4, 0);
    lcd.print("SICAKLIK"); // Başlık
    lcd.setCursor(15, 0);
    lcd.print(sn); // Sağ üst köşe: aktif süre
    lcd.setCursor(2, 1);
    lcd.print(temperature, 2); // Sıcaklık değeri
    lcd.print("-");
    lcd.print(sicaklikDurumu()); // VERY LOW / NORMAL / HIGH
  }
  else if (ekranIndex == 2)
  { // ISIK ekranı
      lcd.setCursor(4, 0);
      lcd.print("ISIK"); // Başlık
      lcd.setCursor(15, 0);
      lcd.print(sn); // Sağ üst köşe: aktif süre
      lcd.setCursor(2, 1);
      lcd.print(lux);          // Lux değerini yaz
      lcd.print("-");
      lcd.print(isikDurumu()); // KARANLIK veya AYDINLIK
  }
}

void setup()
{
  pinMode(RED_PIN, OUTPUT);   // Kırmızı pini çıkış yap
  pinMode(GREEN_PIN, OUTPUT); // Yeşil pini çıkış yap
  pinMode(BLUE_PIN, OUTPUT);  // Mavi pini çıkış yap

  // Başlangıçta LED'i kapat
  setRGB(0, 0, 0);
  delay(100);

  Serial.begin(115200);              // Seri haberleşmeyi başlat
  dht.begin();                       // DHT sensörünü başlat
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Butonu pull-up ile giriş yap

  lcd.init();      // LCD'yi başlat
  lcd.backlight(); // LCD arka ışığını aç

  delay(500); // LCD başlatması için bekle

  // İlk sensör okumalarını yap
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  int ldrValue = analogRead(LDR_PIN);
  lux = map(ldrValue, 0, 4095, 0, 1000);

  // Eğer sensör okuma başarısızsa varsayılan değerleri kullan
  if (isnan(humidity))
    humidity = 50;
  if (isnan(temperature))
    temperature = 25;
  if (lux < 0)
    lux = 500;

  updateLED(); // LED'i sensör değerlerine göre yak
}

void loop()
{
  unsigned long currentMillis = millis(); // Anlık zamanı al

  // LCD Round Robin güncelleme
  if (currentMillis - previousLCDMillis >= lcdInterval)
  {
    previousLCDMillis = currentMillis; // Son LCD zamanını güncelle
    updateLCD();                       // Aktif ekranı göster
    ekranIndex = (ekranIndex + 1) % 3; // Sonraki ekrana geç
  }

  // Sensör okuma (her 3 saniyede bir)
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis; // Son okuma zamanını güncelle

    float h = dht.readHumidity();    // Nem oku
    float t = dht.readTemperature(); // Sıcaklık oku

    if (!isnan(h) && !isnan(t))
    {                  // Geçerli okuma kontrolü
      humidity = h;    // Nem değerini güncelle
      temperature = t; // Sıcaklık değerini güncelle
    }

    int ldrValue = analogRead(LDR_PIN);    // LDR değerini oku (0-4095)
    lux = map(ldrValue, 0, 4095, 0, 1000); // Lux değerine dönüştür

    // Seri monitöre yazdır
    Serial.print("Nem: ");
    Serial.print(humidity, 2);
    Serial.print("% || Sicaklik: ");
    Serial.print(temperature, 2);
    Serial.print((char)176); // ° sembolü
    Serial.print("C || Isik: ");
    Serial.print(lux);
    Serial.println(" lux");

    updateLED(); // LED'i güncelle
  }

  // Buton kontrolü (debounce ile)
  currentButtonState = digitalRead(BUTTON_PIN); // Buton durumunu oku

  if (lastButtonState == HIGH && currentButtonState == LOW)
  { // Basış algıla
    if (currentMillis - lastDebounceTime >= debounceDelay)
    {                                   // Debounce filtresi
      lastDebounceTime = currentMillis; // Zamanı güncelle

      lcdInterval = (lcdInterval == 2000) ? 4000 : 2000; // 2sn ↔ 4sn geçiş

      Serial.print("LCD suresi degistirildi: ");
      Serial.print(lcdInterval / 1000);
      Serial.println(" sn");
    }
  }

  lastButtonState = currentButtonState; // Buton durumunu kaydet
}