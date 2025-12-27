<div align="center">

# **Kainure**

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&style=for-the-badge)
![Node.js](https://img.shields.io/badge/Node.js-v22.21.1-339933?logo=node.js&style=for-the-badge)
![V8](https://img.shields.io/badge/V8-12.4-4B8BF5?logo=v8&style=for-the-badge)
![libuv](https://img.shields.io/badge/libuv-v1.48.0-blue?logo=libuv&style=for-the-badge)
![JavaScript](https://img.shields.io/badge/JavaScript-ES2023-F7DF1E?logo=javascript&style=for-the-badge)
![TypeScript](https://img.shields.io/badge/TypeScript-5.x-3178C6?logo=typescript&style=for-the-badge)
![License](https://img.shields.io/badge/License-Apache_2.0-blue?style=for-the-badge)

**Framework für die moderne SA-MP-Entwicklung mit JavaScript und TypeScript.**

</div>

## Sprachen

- Português: [README](../../)
- English: [README](../English/README.md)
- Español: [README](../Espanol/README.md)
- Français: [README](../Francais/README.md)
- Italiano: [README](../Italiano/README.md)
- Polski: [README](../Polski/README.md)
- Русский: [README](../Русский/README.md)
- Svenska: [README](../Svenska/README.md)
- Türkçe: [README](../Turkce/README.md)

## Inhaltsverzeichnis

- [**Kainure**](#kainure)
  - [Sprachen](#sprachen)
  - [Inhaltsverzeichnis](#inhaltsverzeichnis)
  - [Was ist Kainure?](#was-ist-kainure)
  - [Installation und Verwendung](#installation-und-verwendung)
    - [1. Download](#1-download)
    - [2. Dateistruktur](#2-dateistruktur)
    - [3. Konfiguration (config.json)](#3-konfiguration-configjson)
  - [API und Funktionen](#api-und-funktionen)
    - [Philosophie: Global und Unkompliziert](#philosophie-global-und-unkompliziert)
    - [Hauptmodul: `api.js`](#hauptmodul-apijs)
      - [`Float`](#float)
      - [`Ref`](#ref)
      - [`Public`](#public)
      - [`Call_Public`](#call_public)
      - [`Native`](#native)
      - [`Native_Hook`](#native_hook)
      - [`Include_Storage`](#include_storage)
    - [Befehlsmodul: `commands.js`](#befehlsmodul-commandsjs)
      - [`Command`](#command)
      - [`Command_Params`](#command_params)
      - [`Alias_Command`](#alias_command)
      - [`Call_Command`](#call_command)
    - [Konstanten und Typen](#konstanten-und-typen)
      - [`samp_constants.js`](#samp_constantsjs)
      - [Verzeichnis `types/`](#verzeichnis-types)
  - [Offizieller Gamemode](#offizieller-gamemode)
  - [Kompilierung](#kompilierung)
    - [Windows](#windows)
    - [Linux](#linux)
  - [Lizenz](#lizenz)
    - [Nutzungsbedingungen](#nutzungsbedingungen)
      - [1. Gewährte Berechtigungen](#1-gewährte-berechtigungen)
      - [2. Obligatorische Bedingungen](#2-obligatorische-bedingungen)
      - [3. Einschränkungen und Begrenzungen](#3-einschränkungen-und-begrenzungen)
      - [4. Geistiges Eigentum](#4-geistiges-eigentum)
      - [5. Gewährleistungsausschluss und Haftungsbeschränkung](#5-gewährleistungsausschluss-und-haftungsbeschränkung)

## Was ist Kainure?

Kainure ist ein Framework, das als Brücke zwischen dem traditionellen San Andreas: Multiplayer (SA-MP) und moderner Softwareentwicklung dient. Es ermöglicht Ihnen, die gesamte Logik Ihres Gamemodes mit **JavaScript** oder **TypeScript** zu schreiben und eröffnet so ein Universum an Möglichkeiten mit aktuellen Tools, Paketen und Entwicklungspraktiken.

Das Plugin integriert eine **Node.js**-Umgebung direkt in Ihren SA-MP-Server. Die verwendete spezifische Node.js-Version ist **v22.21.1**, die die Leistung von Hochleistungskomponenten mitbringt:
- **V8 Engine:** Derselbe Hochleistungs-JavaScript-Motor von **Google Chrome**, der sicherstellt, dass Ihr Code mit beeindruckender Geschwindigkeit und Effizienz ausgeführt wird.
- **libuv:** Eine Support-Bibliothek, die I/O (Ein- und Ausgabeoperationen) asynchron verwaltet und es Ihrem Server ermöglicht, mehrere Operationen gleichzeitig zu verarbeiten, ohne zu blockieren – ideal für eine dynamische Spielumgebung.

Mit Kainure können Sie die Einschränkungen von Pawn hinter sich lassen und ein robustes Ökosystem nutzen, um komplexere, organisiertere und leichter zu wartende Gamemodes zu erstellen.

## Installation und Verwendung

### 1. Download

Die neueste Version von Kainure, einschließlich des Plugins und aller erforderlichen Dateien, finden Sie auf der Seite [**Releases**](https://github.com/aldergrounds/kainure/releases) des Projekts.

### 2. Dateistruktur

Nach dem Download müssen Sie die Dateien im Stammverzeichnis Ihres SA-MP-Servers wie folgt organisieren:

```
/ (Gamemode-Stammverzeichnis)
├── plugins/
│   └── Kainure.dll  (oder Kainure.so unter Linux)
├── Kainure/
│   ├── core/
│   ├── types/
│   ├── kainure.js
│   └── config.json (wird beim ersten Start erstellt, falls nicht vorhanden)
└── libnode.dll (oder libnode.so unter Linux)
```

**Wichtig:**
- Das Plugin `Kainure` muss zu Ihrer `server.cfg`-Datei hinzugefügt werden.
- Die Bibliothek `libnode` (`.dll` oder `.so`) muss sich im **Stammverzeichnis** des Servers befinden, zusammen mit der ausführbaren Datei `samp-server` (oder `samp03svr` unter Linux).

### 3. Konfiguration (config.json)

Im Ordner `Kainure/` finden Sie die Datei `config.json`. Wenn sie nicht existiert, erstellt das Plugin sie automatisch beim ersten Ausführen. Diese Datei steuert das Verhalten des Frameworks.

```json
{
    "configs": {
        "main_file": "./main.js"
    },
    "typescript": {
        "enabled": false,
        "output_dir": "./dist",
        "auto_install": true
    }
}
```

- **`main_file`**: Dies ist der Einstiegspunkt Ihres Gamemodes. Kainure beginnt mit der Ausführung des Codes ab dieser Datei. Standardmäßig kann es `.js` oder `.ts` sein (wenn TypeScript aktiviert ist). Der Pfad `./` repräsentiert das Stammverzeichnis Ihres Gamemodes.

- **`typescript.enabled`**: Ändern Sie dies auf `true`, wenn Sie Ihren Code in TypeScript schreiben möchten. Kainure transpiliert die `.ts`-Dateien automatisch in `.js`.

- **`typescript.output_dir`**: Definiert das Verzeichnis, in dem die transpilierten JavaScript-Dateien gespeichert werden.

- **`typescript.auto_install`**: Wenn `true`, prüft Kainure, ob der TypeScript-Compiler (`typescript`) in den `node_modules` Ihres Projekts vorhanden ist. Falls nicht, wird er automatisch installiert, was die Erstkonfiguration vereinfacht.

## API und Funktionen

### Philosophie: Global und Unkompliziert

Kainure wurde entwickelt, um intuitiv zu sein. Sie **müssen keines** der Hauptmodule (`api.js`, `commands.js`, usw.) in Ihre Dateien importieren oder via `require` einbinden. Das Framework lädt und injiziert alle seine Funktionen automatisch in den globalen Bereich (`globalThis`), wodurch sie in jedem Teil Ihres Codes sofort verfügbar sind.

### Hauptmodul: `api.js`

Dieses Modul enthält die wesentlichen Funktionen zur Interaktion mit dem SA-MP-Server.

#### `Float`
Stellt sicher, dass eine Zahl als `float` behandelt wird, auch wenn sie als Ganzzahl interpretiert werden könnte. Dies ist entscheidend für SA-MP-Natives, die Floats erfordern.

**Wann zu verwenden?** Verwenden Sie `Float()` nur, wenn der Wert dynamisch und zur Kompilierzeit unbekannt ist (z. B. aus einem Befehl oder einer anderen Funktion). Die manuelle Verwendung ist nur relevant, wenn Werte an `Native.` und `Call_Public.` übergeben werden.

> [!IMPORTANT]
> Wenn Sie einen expliziten Wert im Code schreiben, wie `50.0`, wendet der Code-Präprozessor von Kainure **automatisch** die `Float`-Behandlung an, bevor er ihn an die V8-Engine sendet. Daher ist die manuelle Verwendung nur in speziellen Fällen erforderlich.

```javascript
Command('sethealth', (playerid, params) => {
    const health = { value: 0.1 }; // "0.1", damit JavaScript nicht zu einer Ganzzahl optimiert.

    if (!Command_Params(params, "f", health))
        return Native.SendClientMessage(playerid, -1, "Verwendung: /sethealth [Gesundheit]");

    // `health.value` ist unbekannt, daher verwenden wir Float(), um die korrekte Typisierung sicherzustellen.
    Native.SetPlayerHealth(playerid, Float(health.value));
});
```

#### `Ref`
Markiert eine Variable, um einen Wert per Referenz von einer nativen Funktion zu erhalten.

**Wie funktioniert es?**
- Um einen Wert von einer Native zu **empfangen** (Ausgabeparameter), verwenden Sie die Syntax `.$` beim Übergeben der Variable.
- Um den in der Variable enthaltenen Wert an eine Native zu **übergeben**, verwenden Sie sie normal.
- Der an `Ref()` übergebene Anfangswert bestimmt den erwarteten Datentyp.

**Initialisierung:**
- **`int`**: `Ref()` oder `Ref(0)`
- **`float`**: `Ref(0.1)` (oder jeder Float, der nicht auf absoluter Null endet, um Optimierung zu Ganzzahl durch JavaScript zu vermeiden)
- **`string`**: `Ref("")`
- **`bool`**: `Ref(true)` oder `Ref(false)`

```javascript
Command('health', (playerid) => {
    const health = Ref(0.1); // Wir erwarten, einen Float zu empfangen.

    // Wir verwenden health.$, damit die Native die Variable mit der Gesundheit des Spielers füllt.
    Native.GetPlayerHealth(playerid, health.$);

    // Wir verwenden nur `health`, um den Wert zu lesen. Das Ref wird automatisch in seinen Wert konvertiert.
    Native.SendClientMessage(playerid, -1, `Ihre Gesundheit: ${health}`);
});
```

#### `Public`
Deklariert einen Callback (Public) von SA-MP, sodass Ihr Code auf Spielereignisse reagieren kann.

**Parametersignaturen:**
Es ist **entscheidend**, eine Signatur für Parameter bereitzustellen, die `string` oder `float` sind, damit Kainure die Typen korrekt konvertieren kann.
- **`string`**: `(param = "s")`
- **`float`**: `(param = "f")`
- `int` und `bool` benötigen keine Signatur.

**Rückgabewerte:**
- `return 1;` oder `return true;`: Erlaubt dem Callback, in anderen Skripten weiter ausgeführt zu werden (falls vorhanden). Dies ist das Standardverhalten, wenn nichts zurückgegeben wird.
- `return 0;` oder `return false;`: Verhindert die Ausführung des Callbacks in anderen Skripten.

```javascript
// Einfaches Public
Public('OnPlayerSpawn', (playerid) => {
    Native.SendClientMessage(playerid, -1, "Sie sind auf dem Server gespawnt.");
    return true;
});

// Public mit String- und Float-Signatur
Public('PublicCustom', (text = "s", value = "f") => {
    console.log(`Text: ${text}, Wert: ${value}`);
});
```

#### `Call_Public`
Ruft ein beliebiges Public auf, sei es vom Gamemode selbst, von einem Filterscript oder ein mit `Public()` erstelltes benutzerdefiniertes Public.

```javascript
Command('callpublic', (playerid) => {
    // Ruft das OnPlayerSpawn-Public für den Spieler auf, der den Befehl verwendet hat.
    Call_Public.OnPlayerSpawn(playerid);
});
```

#### `Native`
Ruft eine native Funktion von SA-MP oder einem geladenen Plugin auf. Fügen Sie einfach das Präfix `Native.` zum Funktionsnamen hinzu.

```javascript
Public('OnPlayerConnect', (playerid) => {
    Native.SendClientMessage(playerid, -1, "Willkommen auf dem Server.");
});
```

#### `Native_Hook`
Fängt den Aufruf einer nativen Funktion ab und ermöglicht es, ihr Verhalten zu ändern oder zusätzliche Logik hinzuzufügen.

**Ausführungsreihenfolge:** Hooks folgen einem **LIFO (Last-In, First-Out)** Muster. Der letzte für eine Native erstellte Hook wird als erster ausgeführt.

**Rückgabewerte:**
- `return 1;` oder `return true;`: Erlaubt die Fortsetzung der Native (Aufruf des nächsten Hooks im Stapel oder der Originalfunktion).
- `return 0;` oder `return false;`: Blockiert die Ausführung der ursprünglichen Native.

```javascript
Native_Hook('SetPlayerHealth', (playerid, health) => {
    console.log(`[Hook]: Die Gesundheit von Benutzer ${playerid} wurde auf ${health} gesetzt.`);
    
    // Erlaubt die Ausführung der ursprünglichen SetPlayerHealth-Native.
    return true; 
});
```

#### `Include_Storage`
Bietet einen eindeutigen und sicheren Verzeichnispfad, damit `includes` (Bibliotheken) ihre eigenen Daten speichern können, um Konflikte zu vermeiden. Der Ordner wird immer innerhalb von `Kainure/includes_storage/` erstellt.

```javascript
// Gibt den Pfad zurück: "Kainure/includes_storage/MyInclude"
const my_data_path = Include_Storage("MyInclude"); 
```

### Befehlsmodul: `commands.js`

Dieses Modul bietet ein komplettes System zur Erstellung und Verwaltung von Befehlen.

#### `Command`
Registriert einen neuen Befehl auf dem Server. Die Callback-Funktion kann bis zu zwei Parameter empfangen: `playerid` und `params`. Beide sind optional.

```javascript
// Einfacher Befehl ohne Parameter
Command('pos', (playerid) => {
    const x = Ref(0.1), y = Ref(0.1), z = Ref(0.1);

    Native.GetPlayerPos(playerid, x.$, y.$, z.$);
    Native.SendClientMessage(playerid, -1, `Position: ${x}, ${y}, ${z}`);
});
```

#### `Command_Params`
Verarbeitet und extrahiert Parameter aus einer Befehlszeichenfolge, ähnlich wie `sscanf`.

**Wie funktioniert es?**
- Die Variable, die den Wert empfangen soll, muss ein Objekt mit einer Eigenschaft `value` sein (z. B.: `{ value: 0 }`).
- Der Anfangswert der Eigenschaft `value` definiert den erwarteten Datentyp.
- Nach der Funktion ist der extrahierte Wert über `.value` zugänglich.

```javascript
Command('givemoney', (playerid, params) => {
    const giveid = { value: 0 }; // Erwartet eine Ganzzahl
    const amount = { value: 0 }; // Erwartet eine Ganzzahl

    if (!Command_Params(params, "ii", giveid, amount))
        return Native.SendClientMessage(playerid, -1, "Richtige Verwendung: /givemoney [ID] [Betrag]");

    Native.GivePlayerMoney(giveid.value, amount.value);
    // Beachten Sie die Verwendung von .value, um auf die verarbeiteten Werte zuzugreifen.
});
```

#### `Alias_Command`
Erstellt einen oder mehrere Aliases (alternative Namen) für einen bestehenden Befehl.

```javascript
// Erstellt den Hauptbefehl
Command('showlife', (playerid) => { /* ... */ });

// Erstellt Aliases für den Befehl 'showlife'
Alias_Command('showlife', 'life', 'hp', 'health');
```
Jetzt führen `/showlife`, `/life`, `/hp` und `/health` denselben Code aus.

#### `Call_Command`
Ruft einen Befehl programmgesteuert aus Ihrem Code auf.

```javascript
// Beispiel: Zwingt Spieler 0, /givemoney 1 500 auszuführen, d.h. er gibt Spieler 1 500 Geld.
Call_Command("givemoney", "1 500", 0 /* oder playerid */);

// Die playerid am Ende ist optional. Dies ist jedoch unlogisch, da Befehle auf Spielern basieren.
Call_Command("announcement", "Der Server wird in 1 Minute neu gestartet!");
```

### Konstanten und Typen

#### `samp_constants.js`
Diese Datei enthält alle Standardkonstanten und -definitionen von SA-MP (wie `MAX_PLAYERS`, `WEAPON_DEAGLE`, usw.). Es wird global geladen, sodass Sie diese Konstanten direkt in Ihrem Code verwenden können, ohne sie deklarieren zu müssen.

#### Verzeichnis `types/`
Dieses Verzeichnis speichert die Typdefinitionsdateien (`.d.ts`) für die gesamte Kainure-API. Wenn Sie TypeScript verwenden, bieten diese Dateien Autovervollständigung, Typprüfung und eine viel reichhaltigere und sicherere Entwicklungserfahrung in Ihrem Code-Editor.

## Offizieller Gamemode

Um Ihr Lernen zu beschleunigen und Kainure in Aktion zu sehen, pflegt **AlderGrounds** einen kompletten Open-Source-Gamemode, der vollständig mit diesem Framework erstellt wurde. Er dient als grundlegendes Stück lebendiger Dokumentation und als bester Ort, um praktische Beispiele zu finden.

Wenn Sie Fragen haben, wie Sie Ihren Code strukturieren, die API verwenden oder komplexe Funktionen implementieren sollen, ist dieses Repository Ihre Hauptreferenzquelle.

➡️ **Entdecken Sie den offiziellen Gamemode:** [Gamemode AlderGrounds](https://github.com/aldergrounds/gamemode)

## Kompilierung

Wenn Sie Kainure aus dem Quellcode kompilieren möchten, befolgen Sie die Anweisungen für Ihr Betriebssystem.

### Windows

1. **Anforderungen**:
   - [Visual Studio](https://visualstudio.microsoft.com/) mit dem Toolset "Desktopentwicklung mit C++" installiert.
2. **Schritte**:
   - Navigieren Sie zum Verzeichnis `build/Windows/`.
   - Öffnen Sie die passende Lösungsdatei für Ihre Version von Visual Studio:
      - `Kainure.sln`: Für Visual Studio 2022 oder niedriger.
      - `Kainure.slnx`: Für Visual Studio 2026.
   - Stellen Sie in Visual Studio sicher, dass die Build-Konfiguration auf **Release** und die Plattform auf **x86** eingestellt ist.
   - Kompilieren Sie die Lösung (normalerweise durch Drücken von `Ctrl+Shift+B` oder über das Menü `Erstellen > Lösung erstellen`).
   - Die kompilierte `Kainure.dll` finden Sie im Ordner `compiled/Windows/`.

### Linux

1. **Anforderungen**:
   - [Docker](https://www.docker.com/) installiert und laufend.
2. **Schritte**:
   - Der Prozess ist durch ein Skript automatisiert. Führen Sie einfach den folgenden Befehl vom Projektstammverzeichnis aus:
      ```bash
      ./build/Linux/compile.sh
      ```
   - Das Skript wird:
      1. Ein Docker-Image (`kainure-builder-x86`) mit allen erforderlichen Abhängigkeiten erstellen (Ubuntu 18.04, g++-9, CMake).
      2. Einen Container aus dem Image ausführen, um das Projekt zu kompilieren.
      3. Die resultierende `Kainure.so` in den Ordner `compiled/Linux/` kopieren.
      4. Das erstellte Docker-Image bereinigen.

## Lizenz

Copyright © **AlderGrounds**

Diese Software ist unter den Bedingungen der Apache-Lizenz, Version 2.0 ("Lizenz") lizenziert; Sie dürfen diese Software nur in Übereinstimmung mit der Lizenz verwenden. Eine Kopie der Lizenz erhalten Sie unter: [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)

### Nutzungsbedingungen

#### 1. Gewährte Berechtigungen

Die vorliegende Lizenz gewährt jeder Person, die eine Kopie dieser Software und der zugehörigen Dokumentationsdateien erhält, kostenlos die folgenden Rechte:
- Die Software in jedem Medium oder Format zu nutzen, zu kopieren, zu modifizieren und zu verbreiten, für jeden Zweck, kommerziell oder nicht-kommerziell.
- Kopien der Software zusammenzuführen, zu veröffentlichen, zu verbreiten, unterzulizensieren und/oder zu verkaufen.
- Personen, denen die Software zur Verfügung gestellt wird, zu gestatten, dasselbe zu tun.

#### 2. Obligatorische Bedingungen

Alle Verbreitungen der Software oder abgeleiteter Werke müssen:
- Eine vollständige Kopie dieser Lizenz enthalten.
- Alle am ursprünglichen Quellcode vorgenommenen Änderungen klar angeben.
- Alle Urheberrechts-, Patent-, Marken- und Zuordnungshinweise bewahren.
- Eine angemessene Dokumentation der implementierten Änderungen bereitstellen.
- Den Lizenz- und Garantiehinweis in allen Kopien beibehalten.

#### 3. Einschränkungen und Begrenzungen

- Diese Lizenz gewährt keine Erlaubnis zur Nutzung von eingetragenen Marken, Logos oder Handelsnamen von **AlderGrounds**.
- Beiträge zum Quellcode müssen unter denselben Bedingungen dieser Lizenz lizenziert werden.
- Die Verwendung der Namen der Mitwirkenden zur Unterstützung oder Förderung von Produkten, die von dieser Software abgeleitet sind, erfordert eine vorherige ausdrückliche Genehmigung.

#### 4. Geistiges Eigentum

Die Software und die gesamte zugehörige Dokumentation sind durch Urheberrechtsgesetze und internationale Verträge geschützt. **AlderGrounds** behält alle Rechte, Titel und Interessen, die durch diese Lizenz nicht ausdrücklich gewährt werden.

#### 5. Gewährleistungsausschluss und Haftungsbeschränkung

DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE GARANTIEN JEGLICHER ART, WEDER AUSDRÜCKLICH NOCH STILLSCHWEIGEND, EINSCHLIEßLICH, ABER NICHT BESCHRÄNKT AUF GARANTIEN DER MARKTGÄNGIGKEIT, EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER.

UNTER KEINEN UMSTÄNDEN SIND DIE AUTOREN ODER URHEBERRECHTSINHABER HAFTBAR FÜR ANSPRÜCHE, SCHÄDEN ODER ANDERE VERPFLICHTUNGEN, SEI ES IN EINER VERTRAGSKLAGE, UNERLAUBTEN HANDLUNG ODER ANDERWEITIG, DIE SICH AUS ODER IN VERBINDUNG MIT DER SOFTWARE ODER DER NUTZUNG ODER ANDEREN GESCHÄFTEN MIT DER SOFTWARE ERGEBEN.

---

Für detaillierte Informationen zur Apache-Lizenz 2.0, besuchen Sie: http://www.apache.org/licenses/LICENSE-2.0