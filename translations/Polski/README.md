<div align="center">

# **Kainure**

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&style=for-the-badge)
![Node.js](https://img.shields.io/badge/Node.js-v22.21.1-339933?logo=node.js&style=for-the-badge)
![V8](https://img.shields.io/badge/V8-12.4-4B8BF5?logo=v8&style=for-the-badge)
![libuv](https://img.shields.io/badge/libuv-v1.48.0-blue?logo=libuv&style=for-the-badge)
![JavaScript](https://img.shields.io/badge/JavaScript-ES2023-F7DF1E?logo=javascript&style=for-the-badge)
![TypeScript](https://img.shields.io/badge/TypeScript-5.x-3178C6?logo=typescript&style=for-the-badge)
![License](https://img.shields.io/badge/License-Apache_2.0-blue?style=for-the-badge)

**Framework do nowoczesnego tworzenia SA-MP przy użyciu JavaScript i TypeScript.**

</div>

## Języki

- Português: [README](../../)
- Deutsch: [README](../Deutsch/README.md)
- English: [README](../English/README.md)
- Español: [README](../Espanol/README.md)
- Français: [README](../Francais/README.md)
- Italiano: [README](../Italiano/README.md)
- Русский: [README](../Русский/README.md)
- Svenska: [README](../Svenska/README.md)
- Türkçe: [README](../Turkce/README.md)

## Spis treści

- [**Kainure**](#kainure)
  - [Języki](#języki)
  - [Spis treści](#spis-treści)
  - [Czym jest Kainure?](#czym-jest-kainure)
  - [Jak zainstalować i używać](#jak-zainstalować-i-używać)
    - [1. Pobieranie](#1-pobieranie)
    - [2. Struktura plików](#2-struktura-plików)
    - [3. Konfiguracja (config.json)](#3-konfiguracja-configjson)
  - [API i Funkcjonalności](#api-i-funkcjonalności)
    - [Filozofia: Globalne i Bez Komplikacji](#filozofia-globalne-i-bez-komplikacji)
    - [Główny moduł: `api.js`](#główny-moduł-apijs)
      - [`Float`](#float)
      - [`Ref`](#ref)
      - [`Public`](#public)
      - [`Call_Public`](#call_public)
      - [`Native`](#native)
      - [`Native_Hook`](#native_hook)
      - [`Include_Storage`](#include_storage)
    - [Moduł komend: `commands.js`](#moduł-komend-commandsjs)
      - [`Command`](#command)
      - [`Command_Params`](#command_params)
      - [`Alias_Command`](#alias_command)
      - [`Call_Command`](#call_command)
      - [`External_Commands`](#external_commands)
    - [Stałe i Typy](#stałe-i-typy)
      - [`samp_constants.js`](#samp_constantsjs)
      - [Katalog `types/`](#katalog-types)
  - [Oficjalny Gamemode](#oficjalny-gamemode)
  - [Kompilacja](#kompilacja)
    - [Windows](#windows)
    - [Linux](#linux)
  - [Licencja](#licencja)
    - [Warunki i Zasady Użytkowania](#warunki-i-zasady-użytkowania)
      - [1. Przyznane Uprawnienia](#1-przyznane-uprawnienia)
      - [2. Warunki Obowiązkowe](#2-warunki-obowiązkowe)
      - [3. Ograniczenia i Restrykcje](#3-ograniczenia-i-restrykcje)
      - [4. Własność Intelektualna](#4-własność-intelektualna)
      - [5. Wyłączenie Gwarancji i Ograniczenie Odpowiedzialności](#5-wyłączenie-gwarancji-i-ograniczenie-odpowiedzialności)

## Czym jest Kainure?

Kainure to framework służący jako pomost między tradycyjnym San Andreas: Multiplayer (SA-MP) a nowoczesnym tworzeniem oprogramowania. Pozwala on na pisanie całej logiki gamemode'u przy użyciu **JavaScript** lub **TypeScript**, otwierając świat możliwości dzięki nowoczesnym narzędziom, pakietom i praktykom programistycznym.

Plugin integruje środowisko **Node.js** bezpośrednio z Twoim serwerem SA-MP. Używana konkretna wersja Node.js to **v22.21.1**, która niesie ze sobą moc komponentów o wysokiej wydajności:
- **Silnik V8:** Ten sam wysokowydajny silnik JavaScript z **Google Chrome**, gwarantujący, że Twój kod jest wykonywany z imponującą szybkością i wydajnością.
- **libuv:** Biblioteka wspierająca, która zarządza I/O (operacjami wejścia i wyjścia) asynchronicznie, pozwalając serwerowi obsługiwać wiele operacji jednocześnie bez zawieszania się, idealna dla dynamicznego środowiska gry.

Dzięki Kainure możesz porzucić ograniczenia Pawn i przyjąć solidny ekosystem do tworzenia bardziej złożonych, zorganizowanych i łatwych w utrzymaniu gamemode'ów.

## Jak zainstalować i używać

### 1. Pobieranie

Najnowszą wersję Kainure, w tym plugin i wszystkie niezbędne pliki, można znaleźć na stronie [**Releases**](https://github.com/aldergrounds/kainure/releases) projektu.

### 2. Struktura plików

Po pobraniu musisz zorganizować pliki w głównym katalogu serwera SA-MP w następujący sposób:

```
/ (Katalog Główny Gamemode'u)
├── plugins/
│   └── Kainure.dll  (lub Kainure.so na Linuxie)
├── Kainure/
│   ├── core/
│   ├── types/
│   ├── kainure.js
│   └── config.json (zostanie utworzony przy pierwszym uruchomieniu, jeśli nie istnieje)
└── libnode.dll (lub libnode.so na Linuxie)
```

**Ważne:**
- Plugin `Kainure` musi zostać dodany do pliku `server.cfg`.
- Biblioteka `libnode` (`.dll` lub `.so`) musi znajdować się w **katalogu głównym** serwera, razem z plikiem wykonywalnym `samp-server` (lub `samp03svr` na Linuxie).

### 3. Konfiguracja (config.json)

Wewnątrz folderu `Kainure/` znajdziesz plik `config.json`. Jeśli on nie istnieje, plugin utworzy go automatycznie przy pierwszym uruchomieniu. Ten plik kontroluje zachowanie frameworka.

```json
{
    "configs": {
        "main_file": "./main.js",
        "encoding": {
            "enabled": false,
            "target": 65001
        }
    },
    "typescript": {
        "enabled": false,
        "output_dir": "./dist"
    }
}
```

- **`main_file`**: To jest punkt wejścia Twojego gamemode'u. Kainure zacznie wykonywać kod od tego pliku. Domyślnie może to być `.js` lub `.ts` (jeśli TypeScript jest włączony). Ścieżka `./` reprezentuje katalog główny Twojego gamemode'u.

- **`encoding.enabled`**: Określa, czy Kainure powinien konwertować ciągi znaków między formatem UTF-8 (standard JavaScript) a kodowaniem Twojego serwera. Jeśli ustawione na `false`, framework będzie używać standardu `65001` (UTF-8).

- **`encoding.target`**: Definiuje docelowe kodowanie.
   - **Windows**: Wymagane jest użycie **numeru Codepage** (np. `1252`).
   - **Linux**: Wymagane jest użycie **nazwy Codepage** (np. `WINDOWS-1252`).

- **`typescript.enabled`**: Zmień na `true`, jeśli chcesz pisać swój kod w TypeScript. Kainure automatycznie przetranspiluje pliki `.ts` na `.js`.
- **`typescript.output_dir`**: Definiuje katalog, w którym zostaną zapisane przetranspilowane pliki JavaScript.

## API i Funkcjonalności

### Filozofia: Globalne i Bez Komplikacji

Kainure został zaprojektowany tak, aby był intuicyjny. Nie musisz **importować ani używać `require`** dla żadnego z głównych modułów (`api.js`, `commands.js` itp.) w swoich plikach. Framework automatycznie ładuje i wstrzykuje wszystkie swoje funkcjonalności do zakresu globalnego (`globalThis`), czyniąc je natychmiast dostępnymi w dowolnej części kodu.

### Główny moduł: `api.js`

Ten moduł zawiera niezbędne funkcje do interakcji z serwerem SA-MP.

#### `Float`

Gwarantuje, że liczba jest traktowana jako `float`, nawet jeśli może być zinterpretowana jako liczba całkowita. Jest to kluczowe dla natywnych funkcji SA-MP, które wymagają floatów.

**Kiedy używać?** Używaj `Float()` tylko wtedy, gdy wartość jest dynamiczna i nieznana w czasie kompilacji (na przykład pochodzi z komendy lub innej funkcji). Jego ręczne użycie jest istotne tylko przy przekazywaniu wartości do `Native.` i `Call_Public.`.

> [!IMPORTANT]
> Jeśli wpiszesz w kodzie jawną wartość, taką jak `50.0`, preprocesor kodu Kainure **automatycznie** zastosuje traktowanie `Float` przed wysłaniem jej do silnika V8. Dlatego ręczne użycie jest konieczne tylko w specyficznych przypadkach.

```javascript
Command('sethealth', (playerid, params) => {
    const health = { value: 0.1 }; // "0.1" aby JavaScript nie zoptymalizował do liczby całkowitej.

    if (!Command_Params(params, "f", health))
        return Native.SendClientMessage(playerid, -1, "Użycie: /sethealth [życie]");

    // `health.value` jest nieznane, więc używamy Float(), aby zapewnić poprawne typowanie.
    Native.SetPlayerHealth(playerid, Float(health.value));
});
```

#### `Ref`

Oznacza zmienną, aby otrzymała wartość przez referencję z funkcji natywnej.

**Jak to działa?**
- Aby **otrzymać** wartość z funkcji natywnej (parametry wyjściowe), użyj składni `.$` podczas przekazywania zmiennej.
- Aby **przekazać** wartość zawartą w zmiennej do funkcji natywnej, użyj jej normalnie.
- Wartość początkowa przekazana do `Ref()` określa oczekiwany typ danych.

**Inicjalizacja:**
- **`int`**: `Ref()` lub `Ref(0)`
- **`float`**: `Ref(0.1)` (lub dowolny float, który nie kończy się absolutnym zerem, aby uniknąć optymalizacji do liczby całkowitej przez JavaScript)
- **`string`**: `Ref("")`
- **`bool`**: `Ref(true)` lub `Ref(false)`

```javascript
Command('health', (playerid) => {
    const health = Ref(0.1); // Oczekujemy otrzymania float.

    // Używamy health.$, aby funkcja natywna wypełniła zmienną życiem gracza.
    Native.GetPlayerHealth(playerid, health.$);

    // Używamy tylko `health`, aby odczytać wartość. Ref jest automatycznie konwertowany na swoją wartość.
    Native.SendClientMessage(playerid, -1, `Twoje życie: ${health}`);
});
```

#### `Public`

Deklaruje callback (public) SA-MP, pozwalając Twojemu kodowi reagować na zdarzenia w grze.

**Sygnatury parametrów:** Aby Kainure poprawnie konwertował typy, wymagane jest podanie sygnatury jako drugiego parametru, jeśli callback posiada argumenty.
- **`i`**: Inteiro (Liczba całkowita)
- **`f`**: Float
- **`s`**: String
- **`b`**: Booleano (Wartość logiczna)

**Walidacje i błędy:** Kainure uniemożliwi wykonanie, jeśli:
1. Funkcja posiada parametry, ale nie podano żadnej sygnatury.
2. Liczba znaków w sygnaturze różni się od liczby parametrów zdefiniowanych w funkcji.
3. Sygnatura zawiera nieprawidłowe typy (inne niż `i, f, s, b`).

**Wartości zwracane:**
- `return 1;` lub `return true;`: Pozwala na dalsze wykonywanie callbacku w innych skryptach (jeśli istnieją). Jest to domyślne zachowanie, jeśli nic nie zostanie zwrócone.
- `return 0;` lub `return false;`: Uniemożliwia wykonanie callbacku w innych skryptach.

```javascript
// Prosty Public bez parametrów
Public('OnGameModeInit', () => {
    // Logika
    return true;
});

// Public z sygnaturą
Public('OnPlayerText', 'is', (playerid, text) => {
    // Logika
    return true;
});
```

#### `Call_Public`

Wywołuje dowolny public, niezależnie od tego, czy należy do gamemode'u, filterscriptu, czy jest niestandardowym public utworzonym za pomocą `Public()`.

```javascript
Command('callpublic', (playerid) => {
    // Wywołuje public OnPlayerSpawn dla gracza, który użył komendy.
    Call_Public.OnPlayerSpawn(playerid);
});
```

#### `Native`

Wywołuje natywną funkcję SA-MP lub dowolnego załadowanego pluginu. Po prostu dodaj prefiks `Native.` do nazwy funkcji.

```javascript
Public('OnPlayerConnect', (playerid) => {
    Native.SendClientMessage(playerid, -1, "Witaj na serwerze.");
});
```

#### `Native_Hook`

Przechwytuje wywołanie natywnej funkcji, pozwalając na modyfikację jej zachowania lub dodanie dodatkowej logiki.

**Kolejność Wykonywania:** Hooki podążają za wzorcem **LIFO (Last-In, First-Out)**. Ostatni hook utworzony dla danej funkcji natywnej będzie pierwszym wykonanym.

**Wartości Zwracane:**
- `return 1;` lub `return true;`: Pozwala na kontynuowanie wykonywania funkcji natywnej (wywołując następny hook w stosie lub oryginalną funkcję).
- `return 0;` lub `return false;`: Blokuje wykonanie oryginalnej funkcji natywnej.

```javascript
Native_Hook('SetPlayerHealth', (playerid, health) => {
    console.log(`[Hook]: Życie użytkownika ${playerid} zostało ustawione na ${health}.`);
    
    // Pozwala na wykonanie oryginalnej funkcji natywnej SetPlayerHealth.
    return true; 
});
```

#### `Include_Storage`

Zapewnia unikalną i bezpieczną ścieżkę katalogu, aby `includes` (biblioteki) mogły przechowywać własne dane, unikając konfliktów. Folder zawsze zostanie utworzony wewnątrz `Kainure/includes_storage/`.

```javascript
// Zwraca ścieżkę: "Kainure/includes_storage/MyInclude"
const my_data_path = Include_Storage("MyInclude"); 
```

### Moduł komend: `commands.js`

Ten moduł oferuje kompletny system do tworzenia i zarządzania komendami.

#### `Command`

Rejestruje nową komendę na serwerze. Funkcja callback może przyjąć do dwóch parametrów: `playerid` i `params`. Oba są opcjonalne.

```javascript
// Prosta komenda bez parametrów
Command('pos', (playerid) => {
    const x = Ref(0.1), y = Ref(0.1), z = Ref(0.1);

    Native.GetPlayerPos(playerid, x.$, y.$, z.$);
    Native.SendClientMessage(playerid, -1, `Pozycja: ${x}, ${y}, ${z}`);
});
```

#### `Command_Params`

Przetwarza i wyodrębnia parametry z ciągu znaków komendy, podobnie jak `sscanf`.

**Jak to działa?**
- Zmienna, która otrzyma wartość, musi być obiektem z właściwością `value` (np. `{ value: 0 }`).
- Wartość początkowa właściwości `value` definiuje oczekiwany typ danych.
- Po wykonaniu funkcji, wyodrębniona wartość będzie dostępna przez `.value`.

```javascript
Command('givemoney', (playerid, params) => {
    const giveid = { value: 0 }; // Oczekuje liczby całkowitej
    const amount = { value: 0 }; // Oczekuje liczby całkowitej

    if (!Command_Params(params, "ii", giveid, amount))
        return Native.SendClientMessage(playerid, -1, "Poprawne użycie: /givemoney [ID] [Kwota]");

    Native.GivePlayerMoney(giveid.value, amount.value);
    // Zwróć uwagę na użycie .value, aby uzyskać dostęp do przetworzonych wartości.
});
```

#### `Alias_Command`

Tworzy jeden lub więcej aliasów (alternatywnych nazw) dla istniejącej komendy.

```javascript
// Tworzy główną komendę
Command('showlife', (playerid) => { /* ... */ });

// Tworzy aliasy dla komendy 'mostrarvida'
Alias_Command('showlife', 'life', 'hp', 'health');
```
Teraz `/showlife`, `/life`, `/hp` i `/health` wykonają ten sam kod.

#### `Call_Command`

Wywołuje komendę programowo z twojego kodu.

```javascript
// Przykład: Zmusza gracza 0 do wykonania /dargrana 1 500, co oznacza, że da on 500 pieniędzy graczowi 1.
Call_Command("givemoney", "1 500", 0 /* lub playerid */);

// Playerid na końcu jest opcjonalne. Jednak jest to nielogiczne, ponieważ komendy opierają się na graczach.
Call_Command("announcement", "Serwer zrestartuje się za 1 minutę!");
```

#### `External_Commands`

Ta funkcja służy do rejestrowania poleceń, które **nie zostały utworzone w środowisku Kainure**, ale istnieją w innych skryptach.

Rejestrując polecenie jako zewnętrzne, Kainure gwarantuje, że globalne callbacki poleceń (`OnPlayerCommandReceived` i `OnPlayerCommandPerformed`) zostaną poprawnie wywołane dla tych poleceń, integrując je z przepływem pracy frameworka.

```javascript
// Rejestrowanie poleceń, które istnieją w filterscripcie
External_Commands("login", "register", "help");
```

### Stałe i Typy

#### `samp_constants.js`

Ten plik zawiera wszystkie stałe i standardowe definicje SA-MP (takie jak `MAX_PLAYERS`, `WEAPON_DEAGLE` itp.). Jest ładowany globalnie, więc możesz używać tych stałych bezpośrednio w swoim kodzie bez konieczności ich deklarowania.

#### Katalog `types/`

Ten katalog przechowuje pliki definicji typów (`.d.ts`) dla całego API Kainure. Jeśli używasz TypeScript, te pliki zapewnią autouzupełnianie, weryfikację typów oraz znacznie bogatsze i bezpieczniejsze doświadczenie programistyczne w twoim edytorze kodu.

## Oficjalny Gamemode

Aby przyspieszyć naukę i zobaczyć Kainure w akcji, **AlderGrounds** utrzymuje kompletny gamemode o otwartym kodzie źródłowym, zbudowany w całości przy użyciu tego frameworka. Służy on jako kluczowy element żywej dokumentacji i najlepsze miejsce do znalezienia praktycznych przykładów.

Jeśli masz pytania dotyczące struktury kodu, używania API lub implementacji złożonych funkcjonalności, to repozytorium jest twoim głównym źródłem odniesienia.

➡️ **Odkryj Oficjalny Gamemode:** [Gamemode AlderGrounds](https://github.com/aldergrounds/gamemode)

## Kompilacja

Jeśli chcesz skompilować Kainure z kodu źródłowego, postępuj zgodnie z instrukcjami dla swojego systemu operacyjnego.

### Windows

1. **Wymagania**:
   - [Visual Studio](https://visualstudio.microsoft.com/) z zestawem narzędzi "Programowanie aplikacji klasycznych dla języka C++" zainstalowanym.
2. **Kroki**:
   - Przejdź do katalogu `build/Windows/`.
   - Otwórz odpowiedni plik rozwiązania dla twojej wersji Visual Studio:
      - `Kainure.sln`: Dla Visual Studio 2022 lub starszego.
      - `Kainure.slnx`: Dla Visual Studio 2026.
   - Wewnątrz Visual Studio upewnij się, że konfiguracja kompilacji jest ustawiona na **Release**, a platforma na **x86**.
   - Skompiluj rozwiązanie (zazwyczaj naciskając `Ctrl+Shift+B` lub poprzez menu `Build > Build Solution`).
   - Skompilowany `Kainure.dll` znajdzie się w folderze `compiled/Windows/`.

### Linux

1. **Wymagania**:
   - [Docker](https://www.docker.com/) zainstalowany i uruchomiony.
2. **Kroki**:
   - Proces jest zautomatyzowany przez skrypt. Wystarczy wykonać następujące polecenie z katalogu głównego projektu:
      ```bash
      ./build/Linux/compile.sh
      ```
   - Skrypt:
      1. Zbuduje obraz Docker (`kainure-builder-x86`) ze wszystkimi niezbędnymi zależnościami (Ubuntu 18.04, g++-9, CMake).
      2. Uruchomi kontener z obrazu, aby skompilować projekt.
      3. Skopiuje wynikowy `Kainure.so` do folderu `compiled/Linux/`.
      4. Wyczyści utworzony obraz Docker.

## Licencja

Copyright © **AlderGrounds**

To oprogramowanie jest licencjonowane na warunkach Licencji Apache, Wersja 2.0 („Licencja”); nie możesz używać tego oprogramowania inaczej niż zgodnie z Licencją. Kopię Licencji można uzyskać pod adresem: [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)

### Warunki i Zasady Użytkowania

#### 1. Przyznane Uprawnienia

Niniejsza licencja przyznaje bezpłatnie każdej osobie, która uzyska kopię tego oprogramowania i powiązanych plików dokumentacji, następujące prawa:
- Używania, kopiowania, modyfikowania i dystrybuowania oprogramowania na dowolnym nośniku lub w dowolnym formacie, w celach komercyjnych lub niekomercyjnych
- Łączenia, publikowania, dystrybuowania, sublicencjonowania i/lub sprzedaży kopii oprogramowania
- Zezwalania osobom, którym oprogramowanie jest dostarczane, na to samo

#### 2. Warunki Obowiązkowe

Wszystkie dystrybucje oprogramowania lub prac pochodnych muszą:
- Zawierać pełną kopię tej licencji
- Wyraźnie wskazywać wszelkie modyfikacje wprowadzone do oryginalnego kodu źródłowego
- Zachować wszystkie informacje o prawach autorskich, patentach, znakach towarowych i atrybucjach
- Dostarczyć odpowiednią dokumentację wprowadzonych zmian
- Zachować informację o licencji i gwarancji we wszystkich kopiach

#### 3. Ograniczenia i Restrykcje

- Ta licencja nie przyznaje pozwolenia na używanie znaków towarowych, logo ani nazw handlowych **AlderGrounds**
- Wkład do kodu źródłowego musi być licencjonowany na tych samych warunkach co niniejsza licencja
- Użycie nazwisk kontrybutorów do popierania lub promowania produktów pochodnych tego oprogramowania wymaga uprzedniej, wyraźnej zgody

#### 4. Własność Intelektualna

Oprogramowanie i cała powiązana dokumentacja są chronione prawami autorskimi i traktatami międzynarodowymi. **AlderGrounds** zachowuje wszelkie prawa, tytuły i udziały, które nie zostały wyraźnie przyznane na mocy niniejszej licencji.

#### 5. Wyłączenie Gwarancji i Ograniczenie Odpowiedzialności

OPROGRAMOWANIE JEST DOSTARCZANE "TAKIE JAKIE JEST", BEZ JAKICHKOLWIEK GWARANCJI, WYRAŹNYCH LUB DOROZUMIANYCH, W TYM MIĘDZY INNYMI GWARANCJI PRZYDATNOŚCI HANDLOWEJ, PRZYDATNOŚCI DO OKREŚLONEGO CELU I NIENARUSZANIA PRAW. 

W ŻADNYM WYPADKU AUTORZY LUB POSIADACZE PRAW AUTORSKICH NIE PONOSZĄ ODPOWIEDZIALNOŚCI ZA JAKIEKOLWIEK ROSZCZENIA, SZKODY LUB INNĄ ODPOWIEDZIALNOŚĆ, CZY TO Z TYTUŁU UMOWY, DELIKTU CZY W INNY SPOSÓB, WYNIKAJĄCE Z, LUB W ZWIĄZKU Z OPROGRAMOWANIEM LUB UŻYTKOWANIEM LUB INNYMI CZYNNOŚCIAMI ZWIĄZANYMI Z OPROGRAMOWANIEM.

---

Szczegółowe informacje na temat licencji Apache 2.0 można znaleźć na stronie: http://www.apache.org/licenses/LICENSE-2.0