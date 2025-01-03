# KontrolaNaczynkaKominka
Sterownik z monitorowaniem temperatury i poziomu wody z prezentacją danych na diodach LED

### v1.0.0
- Załączanie przewodu grzejnego do ochrony przeciw zamarzaniu rury i naczynka wzbiorczego instalacji otwartej kominka z płaszczem wodnym, na podstawie temperatury z czujnika DS18B20. Informacja o aktualnej temperaturze oraz stanie przekaźnika załączania przewodu grzejnego prezentowana jest na 4 diodach LED.
  - powyżej 14°C - temperatura wysoka (grzałka wyłączona) - dioda zielona
  - 14°C - 11°C - temperatura ponad przeciętna (grzałka wyłączona) - dioda żółta i zielona
  - 11°C - 8°C - temperatura dopuszczalna (grzałka wyłączona) - dioda żółta
  - 8°C - 5°C - temperatura krytyczna (grzałka wyłączona) - dioda żółta i czerwona
  - 5°C - 2°C - temperatura zamarzania (grzałka włączona) - dioda czerwona i niebieska
  - poniżej 2°C - problem z ogrzewaniem (grzałka włączona) - dioda czerwona, żółta, zielona i niebieska
- Monitorowanie poziomu wody w naczynku wzbiorczym przy użyciu czujnika dwupływakowego i prezentacja stanu na 3 diodach LED.
  - poziom za wysoki - górna dioda czerwona
  - poziom prawidłowy - dioda zielona
  - poziom za niski - dolna dioda czerwona
  - błąd - obie diody czerwone
