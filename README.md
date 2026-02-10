# Smart LED - Inteligentny Sysytem Oświetlenia LED

**Autor:** Mateusz Smuda
**Sekcja:** 10 
**Nr albumu:** 312441 
**Rok:** 2025 

## Opis Projektu
Projekt "Smart LED" to system automatycznego sterowania oświetleniem LED WS2812B opartym na mikrokontrolerze ESP32-WROOM-32E. System wykorzystuje zaawansowany radar obecności (LD2410C) zamiast tradycyjnych czujników PIR. Dodatkowo barwa światła dostosowuje się do temperatury otoczenia przy użyciu czujnika DHT22, a jasność świecenia jest regulowana automatycznie w zależności od jasności otoczenia za pomocą fotorezystora.

Układ zarządzany jest przez własny serwer WWW postawiony na ESP32, umożliwiający zdalne sterowanie z poziomu przeglądarki (telefon/komputer).

## Główne Funkcjonalności
- **Wykrywanie obecności:** Radar LD2410C (wykrywanie ruchu i bezruchu).
- **Adaptacja barwna:** Zmiana koloru LED w zależności od temperatury (Niebieski <20°C / Zielony / Czerwony >26°C).
- **Adaptacja jasności:** Automatyczne ściemnianie w nocy i rozjaśnianie w dzień.
- **Interfejs WWW:** Panel sterowania (HTML/JS) z podglądem parametrów na żywo.
- **Tryby pracy:** 
  - **AUTO:** Pełna automatyka sensorowa.
  - **MANUAL:** Ręczne ustawianie koloru (RGB) i jasności suwakiem.

## Wykorzystany Sprzęt
1. **Mikrokontroler:** ESP32-WROOM-32E
2. **Oświetlenie:** Taśma LED adresowalna WS2812B (60 diod)
3. **Sensor ruchu:** Radar HLK-LD2410C
4. **Sensor temp/wilg:** DHT22
5. **Sensor światła:** Moduł z fotorezystorem

## Technologie
- **Język:** C++ (Arduino IDE)
- **Web:** HTML, CSS, JavaScript
- **Biblioteki:** Adafruit NeoPixel, DHT Sensor Library, WiFi.h, WebServer.h

## Instrukcja Uruchomienia
1. Podłącz zasilanie 5V.
2. Połącz się z siecią WiFi.

3. Wpisz w przeglądarce adres IP urządzenia.
