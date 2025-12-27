<div align="center">

# **Kainure**

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&style=for-the-badge)
![Node.js](https://img.shields.io/badge/Node.js-v22.21.1-339933?logo=node.js&style=for-the-badge)
![V8](https://img.shields.io/badge/V8-12.4-4B8BF5?logo=v8&style=for-the-badge)
![libuv](https://img.shields.io/badge/libuv-v1.48.0-blue?logo=libuv&style=for-the-badge)
![JavaScript](https://img.shields.io/badge/JavaScript-ES2023-F7DF1E?logo=javascript&style=for-the-badge)
![TypeScript](https://img.shields.io/badge/TypeScript-5.x-3178C6?logo=typescript&style=for-the-badge)
![License](https://img.shields.io/badge/License-Apache_2.0-blue?style=for-the-badge)

**Framework per lo sviluppo moderno di SA-MP con JavaScript e TypeScript.**

</div>

## Lingue

- Português: [README](../../)
- Deutsch: [README](../Deutsch/README.md)
- English: [README](../English/README.md)
- Español: [README](../Espanol/README.md)
- Français: [README](../Francais/README.md)
- Polski: [README](../Polski/README.md)
- Русский: [README](../Русский/README.md)
- Svenska: [README](../Svenska/README.md)
- Türkçe: [README](../Turkce/README.md)

## Indice

- [**Kainure**](#kainure)
  - [Lingue](#lingue)
  - [Indice](#indice)
  - [Cos'è Kainure?](#cosè-kainure)
  - [Come Installare e Usare](#come-installare-e-usare)
    - [1. Download](#1-download)
    - [2. Struttura dei File](#2-struttura-dei-file)
    - [3. Configurazione (config.json)](#3-configurazione-configjson)
  - [API e Funzionalità](#api-e-funzionalità)
    - [Filosofia: Globali e Senza Complicazioni](#filosofia-globali-e-senza-complicazioni)
    - [Modulo Principale: `api.js`](#modulo-principale-apijs)
      - [`Float`](#float)
      - [`Ref`](#ref)
      - [`Public`](#public)
      - [`Call_Public`](#call_public)
      - [`Native`](#native)
      - [`Native_Hook`](#native_hook)
      - [`Include_Storage`](#include_storage)
    - [Modulo Comandi: `commands.js`](#modulo-comandi-commandsjs)
      - [`Command`](#command)
      - [`Command_Params`](#command_params)
      - [`Alias_Command`](#alias_command)
      - [`Call_Command`](#call_command)
    - [Costanti e Tipi](#costanti-e-tipi)
      - [`samp_constants.js`](#samp_constantsjs)
      - [Directory `types/`](#directory-types)
  - [Gamemode Ufficiale](#gamemode-ufficiale)
  - [Compilazione](#compilazione)
    - [Windows](#windows)
    - [Linux](#linux)
  - [Licenza](#licenza)
    - [Termini e Condizioni d'Uso](#termini-e-condizioni-duso)
      - [1. Permessi Concessi](#1-permessi-concessi)
      - [2. Condizioni Obbligatorie](#2-condizioni-obbligatorie)
      - [3. Restrizioni e Limitazioni](#3-restrizioni-e-limitazioni)
      - [4. Proprietà Intellettuale](#4-proprietà-intellettuale)
      - [5. Esclusione di Garanzie e Limitazione di Responsabilità](#5-esclusione-di-garanzie-e-limitazione-di-responsabilità)

## Cos'è Kainure?

Kainure è un framework che funge da ponte tra il tradizionale San Andreas: Multiplayer (SA-MP) e lo sviluppo software moderno. Ti permette di scrivere tutta la logica della tua gamemode utilizzando **JavaScript** o **TypeScript**, aprendo un universo di possibilità con strumenti, pacchetti e pratiche di sviluppo attuali.

Il plugin integra un ambiente **Node.js** direttamente nel tuo server SA-MP. La versione specifica di Node.js utilizzata è la **v22.21.1**, che porta con sé la potenza di componenti ad alte prestazioni:
- **Motore V8:** Lo stesso motore JavaScript ad alte prestazioni di **Google Chrome**, garantendo che il tuo codice venga eseguito con velocità ed efficienza impressionanti.
- **libuv:** Una libreria di supporto che gestisce l'I/O (operazioni di input e output) in modo asincrono, permettendo al tuo server di gestire operazioni multiple simultaneamente senza bloccarsi, ideale per un ambiente di gioco dinamico.

Con Kainure, puoi mettere da parte le limitazioni del Pawn e abbracciare un ecosistema robusto per creare gamemode più complesse, organizzate e facili da mantenere.

## Come Installare e Usare

### 1. Download

L'ultima versione di Kainure, inclusi il plugin e tutti i file necessari, può essere trovata nella pagina delle [**Releases**](https://github.com/aldergrounds/kainure/releases) del progetto.

### 2. Struttura dei File

Dopo il download, devi organizzare i file nella root (radice) del tuo server SA-MP nel seguente modo:

```
/ (Root della Gamemode)
├── plugins/
│   └── Kainure.dll  (o Kainure.so su Linux)
├── Kainure/
│   ├── core/
│   ├── types/
│   ├── kainure.js
│   └── config.json (verrà creato al primo avvio se non esiste)
└── libnode.dll (o libnode.so su Linux)
```

**Importante:**
- Il plugin `Kainure` deve essere aggiunto al tuo file `server.cfg`.
- La libreria `libnode` (`.dll` o `.so`) deve trovarsi nella **root** del server, insieme all'eseguibile `samp-server` (o `samp03svr` su Linux).

### 3. Configurazione (config.json)

All'interno della cartella `Kainure/`, troverai il file `config.json`. Se non esiste, il plugin lo creerà automaticamente la prima volta che verrà eseguito. Questo file controlla il comportamento del framework.

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

- **`main_file`**: Questo è il punto di ingresso della tua gamemode. Kainure inizierà ad eseguire il codice a partire da questo file. Di default, può essere `.js` o `.ts` (se TypeScript è attivato). Il percorso `./` rappresenta la root della tua gamemode.

- **`typescript.enabled`**: Imposta su `true` se desideri scrivere il tuo codice in TypeScript. Kainure transpilerà automaticamente i file `.ts` in `.js`.

- **`typescript.output_dir`**: Definisce la directory dove i file JavaScript transpilation saranno salvati.

- **`typescript.auto_install`**: Se `true`, Kainure verificherà se il compilatore TypeScript (`typescript`) è presente nel `node_modules` del tuo progetto. In caso contrario, lo installerà automaticamente, semplificando la configurazione iniziale.

## API e Funzionalità

### Filosofia: Globali e Senza Complicazioni

Kainure è stato progettato per essere intuitivo. **Non hai bisogno di importare o fare `require`** di nessuno dei moduli principali (`api.js`, `commands.js`, ecc.) nei tuoi file. Il framework carica e inietta automaticamente tutte le sue funzionalità nello scope globale (`globalThis`), rendendole immediatamente accessibili in qualsiasi parte del tuo codice.

### Modulo Principale: `api.js`

Questo modulo contiene le funzioni essenziali per interagire con il server SA-MP.

#### `Float`
Garantisce che un numero sia trattato come `float`, anche se potrebbe essere interpretato come un intero. Questo è cruciale per le native di SA-MP che richiedono float.

**Quando usarlo?** Usa `Float()` solo quando il valore è dinamico e sconosciuto al momento della compilazione (ad esempio, proveniente da un comando o da un'altra funzione). Il suo uso manuale è rilevante solo quando si passano valori a `Native.` e `Call_Public.`.

> [!IMPORTANT]
> Se scrivi un valore esplicito nel codice, come `50.0`, il pre-processore di codice di Kainure applicherà **automaticamente** il trattamento di `Float` prima di inviarlo al motore V8. Pertanto, l'uso manuale è necessario solo in casi specifici.

```javascript
Command('sethealth', (playerid, params) => {
    const health = { value: 0.1 }; // "0.1" affinché JavaScript non ottimizzi in intero.

    if (!Command_Params(params, "f", health))
        return Native.SendClientMessage(playerid, -1, "Uso: /sethealth [vita]");

    // `health.value` è sconosciuto, quindi usiamo Float() per garantire la tipizzazione corretta.
    Native.SetPlayerHealth(playerid, Float(health.value));
});
```

#### `Ref`
Contrassegna una variabile per ricevere un valore per riferimento da una funzione nativa.

**Come funziona?**
- Per **ricevere** un valore da una nativa (parametri di output), usa la sintassi `.$` quando passi la variabile.
- Per **passare** il valore contenuto nella variabile a una nativa, usala normalmente.
- Il valore iniziale passato a `Ref()` determina il tipo di dato atteso.

**Inizializzazione:**
- **`int`**: `Ref()` o `Ref(0)`
- **`float`**: `Ref(0.1)` (o qualsiasi float che non termini con zero assoluto, per evitare l'ottimizzazione in intero da parte di JavaScript)
- **`string`**: `Ref("")`
- **`bool`**: `Ref(true)` o `Ref(false)`

```javascript
Command('health', (playerid) => {
    const health = Ref(0.1); // Ci aspettiamo di ricevere un float.

    // Usiamo health.$ affinché la nativa riempia la variabile con la vita del giocatore.
    Native.GetPlayerHealth(playerid, health.$);

    // Usiamo solamente `health` per leggere il valore. Il Ref viene convertito automaticamente nel suo valore.
    Native.SendClientMessage(playerid, -1, `La tua vita: ${health}`);
});
```

#### `Public`
Dichiara una callback (public) di SA-MP, permettendo al tuo codice di reagire agli eventi del gioco.

**Firme dei Parametri:**
È **cruciale** fornire una firma per i parametri che sono `string` o `float` affinché Kainure possa convertire i tipi correttamente.
- **`string`**: `(param = "s")`
- **`float`**: `(param = "f")`
- `int` e `bool` non necessitano di firma.

**Valori di Ritorno:**
- `return 1;` o `return true;`: Permette alla callback di continuare ad essere eseguita in altri script (se presenti). Questo è il comportamento predefinito se non viene ritornato nulla.
- `return 0;` o `return false;`: Impedisce l'esecuzione della callback in altri script.

```javascript
// Public semplice
Public('OnPlayerSpawn', (playerid) => {
    Native.SendClientMessage(playerid, -1, "Sei spawnato nel server.");
    return true;
});

// Public con firma di string e float
Public('PublicCustom', (text = "s", value = "f") => {
    console.log(`Testo: ${text}, Valore: ${value}`);
});
```

#### `Call_Public`
Chiama qualsiasi public, sia essa della gamemode stessa, di un filterscript, o una public personalizzata creata con `Public()`.

```javascript
Command('callpublic', (playerid) => {
    // Chiama la public OnPlayerSpawn per il giocatore che ha usato il comando.
    Call_Public.OnPlayerSpawn(playerid);
});
```

#### `Native`
Invoca una funzione nativa di SA-MP o di qualsiasi plugin caricato. Aggiungi semplicemente il prefisso `Native.` al nome della funzione.

```javascript
Public('OnPlayerConnect', (playerid) => {
    Native.SendClientMessage(playerid, -1, "Benvenuto nel server.");
});
```

#### `Native_Hook`
Intercetta la chiamata di una funzione nativa, permettendo di modificare il suo comportamento o aggiungere logica extra.

**Ordine di Esecuzione:** Gli hook seguono uno standard **LIFO (Last-In, First-Out)**. L'ultimo hook creato per una nativa sarà il primo ad essere eseguito.

**Valori di Ritorno:**
- `return 1;` o `return true;`: Permette che l'esecuzione della nativa continui (chiamando il prossimo hook nella pila o la funzione originale).
- `return 0;` o `return false;`: Blocca l'esecuzione della nativa originale.

```javascript
Native_Hook('SetPlayerHealth', (playerid, health) => {
    console.log(`[Hook]: La vita dell'utente ${playerid} è stata impostata a ${health}.`);
    
    // Permette che la nativa SetPlayerHealth originale venga eseguita.
    return true; 
});
```

#### `Include_Storage`
Fornisce un percorso di directory unico e sicuro affinché le `includes` (librerie) possano archiviare i propri dati, evitando conflitti. La cartella verrà sempre creata all'interno di `Kainure/includes_storage/`.

```javascript
// Ritorna il percorso: "Kainure/includes_storage/MyInclude"
const my_data_path = Include_Storage("MyInclude"); 
```

### Modulo Comandi: `commands.js`

Questo modulo offre un sistema completo per la creazione e la gestione dei comandi.

#### `Command`
Registra un nuovo comando nel server. La funzione di callback può ricevere fino a due parametri: `playerid` e `params`. Entrambi sono opzionali.

```javascript
// Comando semplice senza parametri
Command('pos', (playerid) => {
    const x = Ref(0.1), y = Ref(0.1), z = Ref(0.1);

    Native.GetPlayerPos(playerid, x.$, y.$, z.$);
    Native.SendClientMessage(playerid, -1, `Posizione: ${x}, ${y}, ${z}`);
});
```

#### `Command_Params`
Elabora ed estrae parametri da una stringa di comando, in modo simile a `sscanf`.

**Come funziona?**
- La variabile che riceverà il valore deve essere un oggetto con una proprietà `value` (es: `{ value: 0 }`).
- Il valore iniziale della proprietà `value` definisce il tipo di dato atteso.
- Dopo la funzione, il valore estratto sarà accessibile attraverso `.value`.

```javascript
Command('givemoney', (playerid, params) => {
    const giveid = { value: 0 }; // Attende un intero
    const amount = { value: 0 }; // Attende un intero

    if (!Command_Params(params, "ii", giveid, amount))
        return Native.SendClientMessage(playerid, -1, "Uso corretto: /givemoney [ID] [Quantità]");

    Native.GivePlayerMoney(giveid.value, amount.value);
    // Nota l'uso di .value per accedere ai valori elaborati.
});
```

#### `Alias_Command`
Crea uno o più alias (nomi alternativi) per un comando esistente.

```javascript
// Crea il comando principale
Command('showlife', (playerid) => { /* ... */ });

// Crea alias per il comando 'mostravita'
Alias_Command('showlife', 'life', 'hp', 'health');
```
Ora, `/showlife`, `/life`, `/hp` e `/health` eseguiranno lo stesso codice.

#### `Call_Command`
Chiama un comando programmaticamente a partire dal tuo codice.

```javascript
// Esempio: Forza il giocatore 0 ad eseguire /dargrana 1 500, ovvero, darà 500 di denaro al giocatore 1.
Call_Command("givemoney", "1 500", 0 /* o playerid */);

// Il playerid alla fine è opzionale. Tuttavia, questo è illogico, dato che i comandi sono basati sui giocatori.
Call_Command("announcement", "Il server si riavvierà tra 1 minuto!");
```

### Costanti e Tipi

#### `samp_constants.js`
Questo file contiene tutte le costanti e le definizioni standard di SA-MP (come `MAX_PLAYERS`, `WEAPON_DEAGLE`, ecc.). Viene caricato globalmente, quindi puoi usare queste costanti direttamente nel tuo codice senza doverle dichiarare.

#### Directory `types/`
Questa directory archivia i file di definizione dei tipi (`.d.ts`) per tutta l'API di Kainure. Se stai usando TypeScript, questi file forniranno autocompletamento, verifica dei tipi e un'esperienza di sviluppo molto più ricca e sicura nel tuo editor di codice.

## Gamemode Ufficiale

Per accelerare il tuo apprendimento e vedere Kainure in azione, **AlderGrounds** mantiene una gamemode completa e open source costruita interamente con questo framework. Serve come un pezzo fondamentale di documentazione viva e il posto migliore per trovare esempi pratici.

Se hai domande su come strutturare il tuo codice, usare l'API o implementare funzionalità complesse, questo repository è la tua principale fonte di riferimento.

➡️ **Esplora la Gamemode Ufficiale:** [Gamemode AlderGrounds](https://github.com/aldergrounds/gamemode)

## Compilazione

Se desideri compilare Kainure a partire dal codice sorgente, segui le istruzioni per il tuo sistema operativo.

### Windows

1. **Requisiti**:
   - [Visual Studio](https://visualstudio.microsoft.com/) con il set di strumenti "Sviluppo di applicazioni desktop con C++" installato.
2. **Passaggi**:
   - Naviga fino alla directory `build/Windows/`.
   - Apri il file di soluzione appropriato per la tua versione di Visual Studio:
      - `Kainure.sln`: Per Visual Studio 2022 o inferiore.
      - `Kainure.slnx`: Per Visual Studio 2026.
   - All'interno di Visual Studio, assicurati che la configurazione di compilazione sia impostata su **Release** e la piattaforma su **x86**.
   - Compila la soluzione (generalmente premendo `Ctrl+Shift+B` o attraverso il menu `Build > Build Solution`).
   - Il `Kainure.dll` compilato si troverà nella cartella `compiled/Windows/`.

### Linux

1. **Requisiti**:
   - [Docker](https://www.docker.com/) installato e in esecuzione.
2. **Passaggi**:
   - Il processo è automatizzato da uno script. Basta eseguire il seguente comando dalla root del progetto:
      ```bash
      ./build/Linux/compile.sh
      ```
   - Lo script:
      1. Costruirà un'immagine Docker (`kainure-builder-x86`) con tutte le dipendenze necessarie (Ubuntu 18.04, g++-9, CMake).
      2. Eseguirà un container a partire dall'immagine per compilare il progetto.
      3. Copierà il `Kainure.so` risultante nella cartella `compiled/Linux/`.
      4. Pulirà l'immagine Docker creata.

## Licenza

Copyright © **AlderGrounds**

Questo software è concesso in licenza secondo i termini della Licenza Apache, Versione 2.0 ("Licenza"); non è possibile utilizzare questo software se non in conformità con la Licenza. Una copia della Licenza può essere ottenuta su: [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)

### Termini e Condizioni d'Uso

#### 1. Permessi Concessi

La presente licenza concede, gratuitamente, a chiunque ottenga una copia di questo software e dei file di documentazione associati, i seguenti diritti:
- Utilizzare, copiare, modificare e distribuire il software in qualsiasi mezzo o formato, per qualsiasi scopo, commerciale o non commerciale
- Unire, pubblicare, distribuire, concedere in sub-licenza e/o vendere copie del software
- Permettere alle persone a cui il software è fornito di fare lo stesso

#### 2. Condizioni Obbligatorie

Tutte le distribuzioni del software o lavori derivati devono:
- Includere una copia completa di questa licenza
- Indicare chiaramente qualsiasi modifica apportata al codice sorgente originale
- Preservare tutti gli avvisi di copyright, brevetti, marchi registrati e attribuzioni
- Fornire documentazione adeguata delle modifiche implementate
- Mantenere l'avviso di licenza e garanzia in tutte le copie

#### 3. Restrizioni e Limitazioni

- Questa licenza non concede il permesso per l'uso di marchi registrati, loghi o nomi commerciali di **AlderGrounds**
- I contributi al codice sorgente devono essere concessi in licenza secondo gli stessi termini di questa licenza
- L'uso dei nomi dei contributori per sostenere o promuovere prodotti derivati da questo software richiede un permesso preventivo specifico

#### 4. Proprietà Intellettuale

Il software e tutta la documentazione associata sono protetti dalle leggi sul copyright e dai trattati internazionali. **AlderGrounds** mantiene tutti i diritti, titoli e interessi non espressamente concessi da questa licenza.

#### 5. Esclusione di Garanzie e Limitazione di Responsabilità

IL SOFTWARE VIENE FORNITO "COSÌ COM'È", SENZA GARANZIE DI ALCUN TIPO, ESPRESSE O IMPLICITE, INCLUSE, MA NON LIMITATE A, GARANZIE DI COMMERCIABILITÀ, IDONEITÀ A UNO SCOPO PARTICOLARE E NON VIOLAZIONE.

IN NESSUNA CIRCOSTANZA GLI AUTORI O I TITOLARI DEL COPYRIGHT SARANNO RESPONSABILI PER QUALSIASI RIVENDICAZIONE, DANNO O ALTRA RESPONSABILITÀ, SIA IN UN'AZIONE CONTRATTUALE, ILLECITO CIVILE O IN ALTRO MODO, DERIVANTE DA, O IN CONNESSIONE CON IL SOFTWARE O L'USO O ALTRE NEGOZIAZIONI NEL SOFTWARE.

---

Per informazioni dettagliate sulla Licenza Apache 2.0, consultare: http://www.apache.org/licenses/LICENSE-2.0