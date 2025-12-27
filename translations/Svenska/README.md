<div align="center">

# **Kainure**

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&style=for-the-badge)
![Node.js](https://img.shields.io/badge/Node.js-v22.21.1-339933?logo=node.js&style=for-the-badge)
![V8](https://img.shields.io/badge/V8-12.4-4B8BF5?logo=v8&style=for-the-badge)
![libuv](https://img.shields.io/badge/libuv-v1.48.0-blue?logo=libuv&style=for-the-badge)
![JavaScript](https://img.shields.io/badge/JavaScript-ES2023-F7DF1E?logo=javascript&style=for-the-badge)
![TypeScript](https://img.shields.io/badge/TypeScript-5.x-3178C6?logo=typescript&style=for-the-badge)
![License](https://img.shields.io/badge/License-Apache_2.0-blue?style=for-the-badge)

**Ramverk för modern utveckling av SA-MP med JavaScript och TypeScript.**

</div>

## Språk

- Português: [README](../../)
- Deutsch: [README](../Deutsch/README.md)
- English: [README](../English/README.md)
- Español: [README](../Espanol/README.md)
- Français: [README](../Francais/README.md)
- Italiano: [README](../Italiano/README.md)
- Polski: [README](../Polski/README.md)
- Русский: [README](../Русский/README.md)
- Türkçe: [README](../Turkce/README.md)

## Innehållsförteckning

- [**Kainure**](#kainure)
  - [Språk](#språk)
  - [Innehållsförteckning](#innehållsförteckning)
  - [Vad är Kainure?](#vad-är-kainure)
  - [Hur man installerar och använder](#hur-man-installerar-och-använder)
    - [1. Ladda ner](#1-ladda-ner)
    - [2. Filstruktur](#2-filstruktur)
    - [3. Konfiguration (config.json)](#3-konfiguration-configjson)
  - [API och Funktioner](#api-och-funktioner)
    - [Filosofi: Globala och utan krångel](#filosofi-globala-och-utan-krångel)
    - [Huvudmodul: `api.js`](#huvudmodul-apijs)
      - [`Float`](#float)
      - [`Ref`](#ref)
      - [`Public`](#public)
      - [`Call_Public`](#call_public)
      - [`Native`](#native)
      - [`Native_Hook`](#native_hook)
      - [`Include_Storage`](#include_storage)
    - [Kommandomodul: `commands.js`](#kommandomodul-commandsjs)
      - [`Command`](#command)
      - [`Command_Params`](#command_params)
      - [`Alias_Command`](#alias_command)
      - [`Call_Command`](#call_command)
    - [Konstanter och Typer](#konstanter-och-typer)
      - [`samp_constants.js`](#samp_constantsjs)
      - [Katalogen `types/`](#katalogen-types)
  - [Officiellt Gamemode](#officiellt-gamemode)
  - [Kompilering](#kompilering)
    - [Windows](#windows)
    - [Linux](#linux)
  - [Licens](#licens)
    - [Villkor och Bestämmelser för Användning](#villkor-och-bestämmelser-för-användning)
      - [1. Beviljade Tillstånd](#1-beviljade-tillstånd)
      - [2. Obligatoriska Villkor](#2-obligatoriska-villkor)
      - [3. Restriktioner och Begränsningar](#3-restriktioner-och-begränsningar)
      - [4. Immateriell Egendom](#4-immateriell-egendom)
      - [5. Friskrivning från Garantier och Ansvarsbegränsning](#5-friskrivning-från-garantier-och-ansvarsbegränsning)

## Vad är Kainure?

Kainure är ett ramverk som fungerar som en brygga mellan det traditionella San Andreas: Multiplayer (SA-MP) och modern mjukvaruutveckling. Det låter dig skriva all logik för ditt gamemode med hjälp av **JavaScript** eller **TypeScript**, vilket öppnar upp ett universum av möjligheter med aktuella verktyg, paket och utvecklingsmetoder.

Insticksprogrammet integrerar en **Node.js**-miljö direkt i din SA-MP-server. Den specifika versionen av Node.js som används är **v22.21.1**, vilket medför kraften hos högpresterande komponenter:
- **V8-motor:** Samma högpresterande JavaScript-motor som i **Google Chrome**, vilket säkerställer att din kod exekveras med imponerande hastighet och effektivitet.
- **libuv:** Ett stödbibliotek som hanterar I/O (in- och utmatningsoperationer) asynkront, vilket gör att din server kan hantera flera operationer samtidigt utan att låsa sig, idealiskt för en dynamisk spelmiljö.

Med Kainure kan du lämna begränsningarna i Pawn bakom dig och omfamna ett robust ekosystem för att skapa mer komplexa, organiserade och lättskötta gamemodes.

## Hur man installerar och använder

### 1. Ladda ner

Den senaste versionen av Kainure, inklusive insticksprogrammet och alla nödvändiga filer, finns på projektets sida för [**Releases**](https://github.com/aldergrounds/kainure/releases).

### 2. Filstruktur

Efter nedladdning måste du organisera filerna i roten av din SA-MP-server på följande sätt:

```
/ (Gamemodens rot)
├── plugins/
│   └── Kainure.dll  (eller Kainure.so på Linux)
├── Kainure/
│   ├── core/
│   ├── types/
│   ├── kainure.js
│   └── config.json (kommer att skapas vid första uppstart om den inte finns)
└── libnode.dll (eller libnode.so på Linux)
```

**Viktigt:**
- Insticksprogrammet `Kainure` måste läggas till i din `server.cfg`-fil.
- Biblioteket `libnode` (`.dll` eller `.so`) måste ligga i serverns **rot**, tillsammans med den körbara filen `samp-server` (eller `samp03svr` på Linux).

### 3. Konfiguration (config.json)

Inuti mappen `Kainure/` hittar du filen `config.json`. Om den inte existerar, skapar insticksprogrammet den automatiskt första gången det körs. Denna fil styr ramverkets beteende.

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

- **`main_file`**: Detta är ingångspunkten för ditt gamemode. Kainure börjar exekvera koden från denna fil. Som standard kan det vara `.js` eller `.ts` (om TypeScript är aktiverat). Sökvägen `./` representerar roten av ditt gamemode.

- **`typescript.enabled`**: Ändra till `true` om du vill skriva din kod i TypeScript. Kainure kommer automatiskt att transpilera `.ts`-filerna till `.js`.

- **`typescript.output_dir`**: Definierar katalogen där de transpilerade JavaScript-filerna kommer att sparas.

- **`typescript.auto_install`**: Om `true`, kontrollerar Kainure om TypeScript-kompilatorn (`typescript`) finns i ditt projekts `node_modules`. Om den inte gör det, installerar den det automatiskt, vilket förenklar den initiala konfigurationen.

## API och Funktioner

### Filosofi: Globala och utan krångel

Kainure designades för att vara intuitivt. Du **behöver inte importera eller göra `require`** av någon av huvudmodulerna (`api.js`, `commands.js`, etc.) i dina filer. Ramverket laddar och injicerar automatiskt alla sina funktioner i det globala omfånget (`globalThis`), vilket gör dem omedelbart tillgängliga i alla delar av din kod.

### Huvudmodul: `api.js`

Denna modul innehåller de väsentliga funktionerna för att interagera med SA-MP-servern.

#### `Float`
Säkerställer att ett nummer behandlas som `float`, även om det kan tolkas som ett heltal. Detta är avgörande för SA-MP-natives som kräver flyttal.

**När ska det användas?** Använd `Float()` endast när värdet är dynamiskt och okänt vid kompileringstillfället (t.ex. kommer från ett kommando eller en annan funktion). Dess manuella användning är relevant endast när man skickar värden till `Native.` och `Call_Public.`.

> [!IMPORTANT]
> Om du skriver ett explicit värde i koden, som `50.0`, tillämpar Kainures kod-preprocessor **automatiskt** `Float`-behandlingen innan det skickas till V8-motorn. Därför är manuell användning endast nödvändig i specifika fall.

```javascript
Command('sethealth', (playerid, params) => {
    const health = { value: 0.1 }; // "0.1" för att JavaScript inte ska optimera till heltal.

    if (!Command_Params(params, "f", health))
        return Native.SendClientMessage(playerid, -1, "Uso: /sethealth [vida]");

    // `health.value` är okänt, så vi använder Float() för att säkerställa rätt typning.
    Native.SetPlayerHealth(playerid, Float(health.value));
});
```

#### `Ref`
Markerar en variabel för att ta emot ett värde via referens från en native-funktion.

**Hur fungerar det?**
- För att **ta emot** ett värde från en native (ut-parametrar), använd syntaxen `.$` när du skickar variabeln.
- För att **skicka** värdet som finns i variabeln till en native, använd den som vanligt.
- Det initiala värdet som skickas till `Ref()` bestämmer vilken datatyp som förväntas.

**Initialisering:**
- **`int`**: `Ref()` eller `Ref(0)`
- **`float`**: `Ref(0.1)` (eller vilket flyttal som helst som inte slutar på absolut noll, för att undvika optimering till heltal av JavaScript)
- **`string`**: `Ref("")`
- **`bool`**: `Ref(true)` eller `Ref(false)`

```javascript
Command('health', (playerid) => {
    const health = Ref(0.1); // Vi förväntar oss att ta emot en float.

    // Vi använder health.$ för att nativen ska fylla variabeln med spelarens liv.
    Native.GetPlayerHealth(playerid, health.$);

    // Vi använder endast `health` för att läsa värdet. Ref konverteras automatiskt till sitt värde.
    Native.SendClientMessage(playerid, -1, `Sua vida: ${health}`);
});
```

#### `Public`
Deklarerar en callback (public) från SA-MP, vilket låter din kod reagera på spelhändelser.

**Parametersignaturer:**
Det är **avgörande** att tillhandahålla en signatur för parametrar som är `string` eller `float` så att Kainure kan konvertera typerna korrekt.
- **`string`**: `(param = "s")`
- **`float`**: `(param = "f")`
- `int` och `bool` behöver ingen signatur.

**Returvärden:**
- `return 1;` eller `return true;`: Tillåter callbacken att fortsätta exekveras i andra skript (om det finns några). Detta är standardbeteendet om inget returneras.
- `return 0;` eller `return false;`: Förhindrar exekveringen av callbacken i andra skript.

```javascript
// Enkel Public
Public('OnPlayerSpawn', (playerid) => {
    Native.SendClientMessage(playerid, -1, "Você spawnou no servidor.");
    return true;
});

// Public med signatur för string och float
Public('PublicCustom', (text = "s", value = "f") => {
    console.log(`Texto: ${text}, Valor: ${value}`);
});
```

#### `Call_Public`
Anropar vilken public som helst, oavsett om den är från själva gamemodet, från ett filterscript, eller en anpassad public skapad med `Public()`.

```javascript
Command('callpublic', (playerid) => {
    // Anropar publicen OnPlayerSpawn för spelaren som använde kommandot.
    Call_Public.OnPlayerSpawn(playerid);
});
```

#### `Native`
Anropar en inbyggd funktion (native) från SA-MP eller från något laddat insticksprogram. Lägg helt enkelt till prefixet `Native.` framför funktionsnamnet.

```javascript
Public('OnPlayerConnect', (playerid) => {
    Native.SendClientMessage(playerid, -1, "Seja bem vindo ao servidor.");
});
```

#### `Native_Hook`
Fångar upp anropet till en native-funktion, vilket gör det möjligt att ändra dess beteende eller lägga till extra logik.

**Exekveringsordning:** Hookar följer ett **LIFO (Last-In, First-Out)**-mönster. Den sista hooken som skapades för en native kommer att vara den första som exekveras.

**Returvärden:**
- `return 1;` eller `return true;`: Tillåter exekveringen av nativen att fortsätta (anropar nästa hook i stacken eller originalfunktionen).
- `return 0;` eller `return false;`: Blockerar exekveringen av original-nativen.

```javascript
Native_Hook('SetPlayerHealth', (playerid, health) => {
    console.log(`[Hook]: A vida do usuário ${playerid} foi definida para ${health}.`);
    
    // Tillåter att original-nativen SetPlayerHealth exekveras.
    return true; 
});
```

#### `Include_Storage`
Tillhandahåller en unik och säker katalogsökväg så att `includes` (bibliotek) kan lagra sina egna data, vilket undviker konflikter. Mappen kommer alltid att skapas inuti `Kainure/includes_storage/`.

```javascript
// Returnerar sökvägen: "Kainure/includes_storage/MyInclude"
const my_data_path = Include_Storage("MyInclude"); 
```

### Kommandomodul: `commands.js`

Denna modul erbjuder ett komplett system för att skapa och hantera kommandon.

#### `Command`
Registrerar ett nytt kommando på servern. Callback-funktionen kan ta emot upp till två parametrar: `playerid` och `params`. Båda är valfria.

```javascript
// Enkelt kommando utan parametrar
Command('pos', (playerid) => {
    const x = Ref(0.1), y = Ref(0.1), z = Ref(0.1);

    Native.GetPlayerPos(playerid, x.$, y.$, z.$);
    Native.SendClientMessage(playerid, -1, `Posição: ${x}, ${y}, ${z}`);
});
```

#### `Command_Params`
Bearbetar och extraherar parametrar från en kommandosträng, på liknande sätt som `sscanf`.

**Hur fungerar det?**
- Variabeln som ska ta emot värdet måste vara ett objekt med en egenskap `value` (t.ex: `{ value: 0 }`).
- Det initiala värdet på egenskapen `value` definierar den förväntade datatypen.
- Efter funktionen kommer det extraherade värdet att vara tillgängligt via `.value`.

```javascript
Command('givemoney', (playerid, params) => {
    const giveid = { value: 0 }; // Förväntar sig ett heltal
    const amount = { value: 0 }; // Förväntar sig ett heltal

    if (!Command_Params(params, "ii", giveid, amount))
        return Native.SendClientMessage(playerid, -1, "Uso correto: /givemoney [ID] [Quantia]");

    Native.GivePlayerMoney(giveid.value, amount.value);
    // Notera användningen av .value för att komma åt de bearbetade värdena.
});
```

#### `Alias_Command`
Skapar ett eller flera alias (alternativa namn) för ett existerande kommando.

```javascript
// Skapar huvudkommandot
Command('showlife', (playerid) => { /* ... */ });

// Skapar alias för kommandot 'mostrarvida'
Alias_Command('showlife', 'life', 'hp', 'health');
```
Nu kommer `/showlife`, `/life`, `/hp` och `/health` att köra samma kod.

#### `Call_Command`
Anropar ett kommando programmatiskt från din kod.

```javascript
// Exempel: Tvingar spelare 0 att köra /dargrana 1 500, dvs. han kommer ge 500 i pengar till spelare 1.
Call_Command("givemoney", "1 500", 0 /* eller playerid */);

// Playerid på slutet är valfritt. Men detta är ologiskt, då kommandon baseras på spelarna.
Call_Command("announcement", "O servidor vai reiniciar em 1 minuto!");
```

### Konstanter och Typer

#### `samp_constants.js`
Denna fil innehåller alla standardkonstanter och definitioner för SA-MP (såsom `MAX_PLAYERS`, `WEAPON_DEAGLE`, etc.). Den laddas globalt, så du kan använda dessa konstanter direkt i din kod utan att behöva deklarera dem.

#### Katalogen `types/`
Denna katalog lagrar typdefinitionsfilerna (`.d.ts`) för hela Kainure API:t. Om du använder TypeScript, kommer dessa filer att tillhandahålla autokomplettering, typkontroll och en mycket rikare och säkrare utvecklingsupplevelse i din kodredigerare.

## Officiellt Gamemode

För att påskynda ditt lärande och se Kainure i aktion, underhåller **AlderGrounds** ett komplett gamemode med öppen källkod byggt helt och hållet med detta ramverk. Det fungerar som en fundamental del av levande dokumentation och den bästa platsen att hitta praktiska exempel.

Om du har frågor om hur du ska strukturera din kod, använda API:t eller implementera komplexa funktioner, är detta arkiv din främsta referenskälla.

➡️ **Utforska det Officiella Gamemodet:** [Gamemode AlderGrounds](https://github.com/aldergrounds/gamemode)

## Kompilering

Om du vill kompilera Kainure från källkoden, följ instruktionerna för ditt operativsystem.

### Windows

1. **Krav**:
   - [Visual Studio](https://visualstudio.microsoft.com/) med verktygsuppsättningen "Utveckling för skrivbordet med C++" installerad.
2. **Steg**:
   - Navigera till katalogen `build/Windows/`.
   - Öppna lämplig lösningsfil för din version av Visual Studio:
      - `Kainure.sln`: För Visual Studio 2022 eller lägre.
      - `Kainure.slnx`: För Visual Studio 2026.
   - Inuti Visual Studio, se till att byggkonfigurationen är inställd på **Release** och plattformen på **x86**.
   - Kompilera lösningen (vanligtvis genom att trycka `Ctrl+Shift+B` eller via menyn `Build > Build Solution`).
   - Den kompilerade `Kainure.dll` kommer att hittas i mappen `compiled/Windows/`.

### Linux

1. **Krav**:
   - [Docker](https://www.docker.com/) installerat och körandes.
2. **Steg**:
   - Processen är automatiserad med ett skript. Kör bara följande kommando från projektets rot:
      ```bash
      ./build/Linux/compile.sh
      ```
   - Skriptet kommer att:
      1. Bygga en Docker-image (`kainure-builder-x86`) med alla nödvändiga beroenden (Ubuntu 18.04, g++-9, CMake).
      2. Köra en container från imagen för att kompilera projektet.
      3. Kopiera den resulterande `Kainure.so` till mappen `compiled/Linux/`.
      4. Rensa den skapade Docker-imagen.

## Licens

Copyright © **AlderGrounds**

Denna programvara är licensierad under villkoren för Apache License, Version 2.0 ("Licensen"); du får inte använda denna programvara förutom i enlighet med Licensen. En kopia av Licensen kan erhållas på: [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)

### Villkor och Bestämmelser för Användning

#### 1. Beviljade Tillstånd

Denna licens beviljar, kostnadsfritt, varje person som erhåller en kopia av denna programvara och tillhörande dokumentationsfiler, följande rättigheter:
- Använda, kopiera, modifiera och distribuera programvaran i vilket medium eller format som helst, för vilket syfte som helst, kommersiellt eller icke-kommersiellt
- Slå samman, publicera, distribuera, underlicensiera och/eller sälja kopior av programvaran
- Tillåta personer till vilka programvaran tillhandahålls att göra detsamma

#### 2. Obligatoriska Villkor

Alla distributioner av programvaran eller härledda verk måste:
- Inkludera en fullständig kopia av denna licens
- Tydligt ange eventuella ändringar som gjorts i den ursprungliga källkoden
- Bevara alla meddelanden om upphovsrätt, patent, varumärken och tillskrivningar
- Tillhandahålla adekvat dokumentation av de implementerade ändringarna
- Behålla licensmeddelandet och garantin i alla kopior

#### 3. Restriktioner och Begränsningar

- Denna licens ger inte tillstånd att använda registrerade varumärken, logotyper eller handelsnamn som tillhör **AlderGrounds**
- Bidrag till källkoden måste licensieras under samma villkor som denna licens
- Användning av bidragsgivares namn för att stödja eller marknadsföra produkter härledda från denna programvara kräver specifikt förhandstillstånd

#### 4. Immateriell Egendom

Programvaran och all tillhörande dokumentation skyddas av upphovsrättslagar och internationella fördrag. **AlderGrounds** behåller alla rättigheter, titlar och intressen som inte uttryckligen beviljas av denna licens.

#### 5. Friskrivning från Garantier och Ansvarsbegränsning

PROGRAMVARAN TILLHANDAHÅLLS "I BEFINTLIGT SKICK", UTAN NÅGRA GARANTIER AV NÅGOT SLAG, UTTRYCKLIGA ELLER UNDERFÖRSTÅDDA, INKLUSIVE, MEN INTE BEGRÄNSAT TILL, GARANTIER FÖR SÄLJBARHET, LÄMPLIGHET FÖR ETT VISST SYFTE OCH ICKE-INTRÅNG.

UNDER INGA OMSTÄNDIGHETER SKALL UPPHOVSRÄTTSINNEHAVARNA ELLER RÄTTIGHETSHAVARNA HÅLLAS ANSVARIGA FÖR NÅGRA ANSPRÅK, SKADOR ELLER ANNAT ANSVAR, VARE SIG I EN TALAN OM KONTRAKT, ÅTALBAR HANDLING ELLER PÅ ANNAT SÄTT, SOM UPPSTÅR UR, ELLER I SAMBAND MED PROGRAMVARAN ELLER ANVÄNDNINGEN ELLER ANDRA TRANSAKTIONER I PROGRAMVARAN.

---

För detaljerad information om Apache License 2.0, se: http://www.apache.org/licenses/LICENSE-2.0