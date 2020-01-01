# Asystent parkowania w garażu

Asystent parkowania w garażu to prosty silnik
który wgrywamy na nasz docelowy kontroler.

## Główne założenia projektu:
- główny silnik który jest przenośny między różnymi platformami
- prosta podmiana i wybór komponentów których chcemy użyć w projekcie
- proste, trzy poziomowe wskazanie odległości od ściany
- wykrywanie błędów i ostrzeżenie użytkownia o błędzie zanim ten zacznie parkować
- załączanie wskaźnika odległości tylko gdy wykryto że silnik auta jest włączony

## Jak to działa
Urządzenie aktywuje się tylko po wykryciu odgłosu pracy silnika naszego samochodu.
Gdy jest aktywne, wskazuje nam aktualną odległość między samochodem a ścianą garażu.
Wskazanie odległości polega na mryganiu belką led (znakiem STOP) w różny sposób,
w zależności od odległości. Dla przykładu w odległości 2m od ściany
nasza belka mryga raz na sekundę, w odległości jednego metra od ściany
belka mryga dwa razy na sekundę a w odległości 50cm belka miga 5 razy na sekundę.
Gdy miniemy określony punkt, na przykład 10cm od ściany, belka przestaje mrygać
i świeci ciągłym światłem. Punkt STOP można regulować za pomocą potencjometru.

Sposób wskazania odległości, kalibrację punktu STOP, sposób pobierania odległości,
sposób wykrywania czy silnik auta jest włączony oraz wskaźnik stanu można zmienić
wybierając inny preset, tworząc własny preset lub pisząc własny sterownik urządzenia
jeśli jeszcze nie jest dostępny.

## Sposób montażu
Nasz asystent parkowania powinien być zamontowany na ścianie naprzeciwko wjazdu do garażu.
Sensor odległości i sensor aktywacji silnika muszą być skierowane w stronę samochodu
i być umieszczone na wysokości tablicy rejestracyjnej naszego auta.
Belka LED STOP powinna być zamontowana na wysokości oczu kierowcy.

## Presety
Aby ułatwić proces programowania swojego własnego kontrolera ludziom którzy
nie potrafią kodować projekt zawiera tak zwane **presety**.

**Presety** są to predefiniowane zestawy które pracują z wybranymi czujnikami
prosto po wgraniu na kontroler. Ułatwiają zaprogramowanie swojego asystenta
wymagając tylko podłączenia czujników i innych urządzeń do wybranych pinów
kontrolera, dzięki czemu cały proces budowy jest dużo szybszy.

Wybór presetu zależy od tego jaki kontroler posiadamy oraz jakie i jakiej jakości
czujniki chcemy do tego podłączyć.

Dostępne presety znajdują się w folderze `include/presets/`.
Wszystkie podfoldery w tej ścieżce są typami kontrolerów na które dane presety
zostały napisane. Na przykład w folderze `include/presets/stm32_blue_pill`
znajdują się presety napisane na kontroler `stm32 blue pill`.

Aby wybrać dany preset wystarczy w pliku `include/presets/Presets.hpp`
odkomentować linię odblokowującą wybrany preset.

Jeśli jest potrzeba zmiany pinoutu wystarczy wejść w plik konfiguracyjny
wybranego przez nas presetu i zedytować jego zawartość, ale w większości przypadków
nie powinno być to potrzebne.

## Dostępne presety:
- `stm32_blue_pill/Preset_01` - preset na płytkę `stm32_blue_pill`. Do mierzenia odległości używa taniego i ogólnodostępnego ultrasonicznego czujnika odległości `HC-SR04`. Jako czujnik wykrycia pracy silnika jest użyty czujnik dźwięku `FC-04` którego czułość trzeba skalibrować tak, aby wykrywał hałas pracy silnika naszego samochodu. Kalibracja pozycji stopu działa dzięki dzielnikowi napięcia zbudowanemu z potencjometru, jego maksymalne napięcie to 3.3V a minimum to 0V. Sterowanie znakiem STOP zrealizowane jest za pomocą prostego drivera, który podaje stan wysoki albo niski (w zależności od konfiguracji) na podany w konfiguracji pin gdy oświetlenie jest włączone oraz jego przeciwieństwo gdy oświetlenie jest wyłączone. Użycie tego modułu jest bardzo proste, wystarczy do skonfigurowanego pinu płytki podłączyć tranzystor sterujący oświetleniem znaku STOP. Ten preset nie ma skonfigurowanego modułu wskaźnika stanu urządzenia.

- `stm32_blue_pill/Preset_02` - preset zawiera taką samą konfigurację co `Preset_01`. Różnicą jest wskaźnik stanu urządzenia jako który użyty jest wyświetlacz `SSD1306` podłączony przez szynę I2C.

## Prosta diagnostyka urządzeń
W projekcie dostępne są proste mini projekty diagnostyczne urządzeń
w katalogu `include/diagnostics`, który podzielony jest w stylu <br />
`include/diagnostics/<platforma>/<nazwa_diagnostyki>`.

Aby odblokować dany projekt diagnostyczny należy zmienić wartość pierwszej
dyrektywy `#define` na `true`. Na przykład dla projektu diagnostycznego
dostępnego w pliku `include/diagnostics/stm32/BlinkExample.hpp`
będzie to linia numer 3: <br />
`#define BLINK_DIAGNOSTICS_ENABLED false` <br />
której wartość trzeba zmienić w sposób następujący: <br />
`#define BLINK_DIAGNOSTICS_ENABLED true`

Wtedy wgrywamy nasz projekt na naszą płytkę.

W analogiczny sposób postępujemy w innych projektach diagnostycznych.


# Kod

## Budowa programu
Główny rdzeń programu pobierający dane i aktualizujący pozostałe moduły znajduje się
w pliku `include/core/Program.hpp`.

Klasa `Core::Program` korzysta z pięciu zewnętrznych zależności.
Zależności te są wstrzykiwane przez konstruktor. Są one abstrakcjami
na różne urządzenia zewnętrzne, można je znaleźć w folderze
`include/devices/abstract`.

## Nasze zależności to:
- `Devices::DistanceSensor::Abstract::IDistanceSensor` - jest to interfejs sensora odległości. Posiada on jedną metodę `const Distance meansure()` która mierzy oraz zwraca odległość do samochodu. Typ zwracany `Devices::DistanceSensor::Distance` zawiera trzy własności:
    - `float distance` - zawierająca dystans dzielący samochód i czujnik
    - `bool tooFar` - jest ustawiona na `true` gdy dystans dzielący samochód i czujnik jest zbyt duży przez co czujnik nie może podać poprawnej odległości
    - `unsigned char error` - jeśli sterownik czujnika wykryje błąd zmienna ta ustawiana jest na wartość inną niż 0. Gdy wartość jest inna niż 0, zostanie wyświetlona na wyświetlaczu jeśli jest taka możliwość.

- `Devices::CarEngineStateSensor::Abstract::ICarEngineState` - interfejs czujnika włączonego silnika. Metoda `const EngineState sense()` sprawdza czy silnik jest włączony i zwraca wynik w postaci struktury `Devices::CarEngineStateSensor::EngineState` która zawiera dwie własności:
    - `bool isEngineStarted` - ustawiona na `true` gdy silnik jest włączony
    - `unsigned char error` - jeśli jest ustawiona na coś innego od zera znaczy że wykryto błąd. Kod błędu będzie wyświetlony na wyświetlaczu jeśli będzie taka możliwość.

- `Devices::DistanceCalibrator::Abstract::IDistanceCalibrator` - interfejs potencjometru lub innego modułu dzięki któremu możemy dopasować wyprzedzenie lub opóźnienie zaświecenia oświetlenia STOP. Do pobrania tej wartości program używa metody `const DistanceCalibrationValue getValue()`. Struktura `Devices::DistanceCalibrator::DistanceCalibrationValue` ma dwie własności:
    - `short value` - wartość kalibracyjna w centymetrach
    - `unsigned char error` - jeśli jest ustawiona na inną wartość niż zero znaczy że wykryto błąd. Kod błędu będzie wyświetlony na wyświetlaczu jeśli tylko będzie można.

- `Devices::HighlightedSign::Abstract::IHighlightedSign` - interfejs podświetlanego znaku STOP. Zawiera metodę `setBlinkingLevel` służącą do ustawienia aktualnego trybu świecenia znaku. Zwraca ona obiekt typu `enum Devices::HighlightedSign::Blinkinglevel` i przyjmuje dwa argumenty:
    - `const BlinkingLevel &level` - aktualny tryb świecenia
    - `const unsigned short &distanceInCm` - aktualny dystans w centymetrach.

    Struktura `Devices::HighlightedSign::HighlightSetResult` zawiera trzy własności:
    - `enum Devices::HighlightedSign::Blinkinglevel level` - aktualny tryb świecenia, który może przyjąć jedną z pięciu wartości:
        - `STOP` - znak dla kierowcy żeby się zatrzymał
        - `NEAR` - tryb mrygania mówiący kierowcy że ma już się zatrzymywać
        - `MEDIUM` - oznacza że należy się przygotować do hamowania
        - `FAR` - służy głównie pokazaniu że asystent parkowania jest aktywny i funkcjonuje poprawnie
        - `OFF` - wyłącza mryganie, samochód może być zbyt daleko lub być wyłączony
    - `bool actualState` - aktualny stan świateł, czy się świecą czy też nie
    - `unsigned char error` - jeśli jest ustawiona na wartość inną niż zero znaczy że wykryto błąd. Kod błędu będzie wyświetlony na wyświetlaczu jeśli będzie taka możliwość.

- `Devices::StateIndicator::Abstract::IStateIndicator` - interfejs wskaźnika stanu kontrolera. Zawiera tylko jedną metodę `void update(...)`. Argumenty podane do tej funkcji to najnowsze dostępne dane pobrane z sensorów i wyniki wykonania funkcji. Można je wyświetlić lub wypisać na serial.

## Implementacje abstrakcji urządzeń
Każda implementacja interfejsu musi implementować wszystkie metody danego interfejsu.
Przykłady implementacji można znaleźć w folderze `devices/implementation`.
Dzieli się on na subfoldery, po jednym na każdą platformę a potem na typ abstrakcji.
Niżej w każdym folderze znajdziemy już prawdziwe implementacje abstrakcji sensorów i innych urządzeń.

W skrócie katalogi implementacji wyglądają tak: <br />
`devices/implementation/<platforma>/<typ_sensora>/<nazwa_sensora>` <br />
Na przykład: <br />
`devices/implementation/stm32/distanceSensor/HC_SR04` <br />

## Kontrybucje
Kontrybucje są mile widziane.
Jeśli tworzysz asystent parkowania oparty o własne komponenty
miło byłoby gdybyś przyłączył się do projektu i zrobił pull request.
Pamiętaj tylko o używaniu przestrzeni nazw, zachowaniu jakości kodu
i odpowiedniej dokumentacji stworzonych presetów. 

## Licencja
Licencja jest dostępna w repozytorium w pliku `LICENSE`.