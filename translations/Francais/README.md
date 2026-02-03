<div align="center">

# **Kainure**

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&style=for-the-badge)
![Node.js](https://img.shields.io/badge/Node.js-v22.21.1-339933?logo=node.js&style=for-the-badge)
![V8](https://img.shields.io/badge/V8-12.4-4B8BF5?logo=v8&style=for-the-badge)
![libuv](https://img.shields.io/badge/libuv-v1.48.0-blue?logo=libuv&style=for-the-badge)
![JavaScript](https://img.shields.io/badge/JavaScript-ES2023-F7DF1E?logo=javascript&style=for-the-badge)
![TypeScript](https://img.shields.io/badge/TypeScript-5.x-3178C6?logo=typescript&style=for-the-badge)
![License](https://img.shields.io/badge/License-Apache_2.0-blue?style=for-the-badge)

**Framework pour le développement moderne de SA-MP avec JavaScript et TypeScript.**

</div>

## Langues

- Português: [README](../../)
- Deutsch: [README](../Deutsch/README.md)
- English: [README](../English/README.md)
- Español: [README](../Espanol/README.md)
- Italiano: [README](../Italiano/README.md)
- Polski: [README](../Polski/README.md)
- Русский: [README](../Русский/README.md)
- Svenska: [README](../Svenska/README.md)
- Türkçe: [README](../Turkce/README.md)

## Table des matières

- [**Kainure**](#kainure)
  - [Langues](#langues)
  - [Table des matières](#table-des-matières)
  - [Qu'est-ce que Kainure ?](#quest-ce-que-kainure-)
  - [Comment Installer et Utiliser](#comment-installer-et-utiliser)
    - [1. Téléchargement](#1-téléchargement)
    - [2. Structure des Fichiers](#2-structure-des-fichiers)
    - [3. Configuration (config.json)](#3-configuration-configjson)
  - [API et Fonctionnalités](#api-et-fonctionnalités)
    - [Philosophie : Globaux et Sans Complication](#philosophie--globaux-et-sans-complication)
    - [Module Principal : `api.js`](#module-principal--apijs)
      - [`Float`](#float)
      - [`Ref`](#ref)
      - [`Public`](#public)
      - [`Call_Public`](#call_public)
      - [`Native`](#native)
      - [`Native_Hook`](#native_hook)
      - [`Include_Storage`](#include_storage)
    - [Module de Commandes : `commands.js`](#module-de-commandes--commandsjs)
      - [`Command`](#command)
      - [`Command_Params`](#command_params)
      - [`Alias_Command`](#alias_command)
      - [`Call_Command`](#call_command)
      - [`External_Commands`](#external_commands)
    - [Constantes et Types](#constantes-et-types)
      - [`samp_constants.js`](#samp_constantsjs)
      - [Répertoire `types/`](#répertoire-types)
  - [Gamemode Officiel](#gamemode-officiel)
  - [Compilation](#compilation)
    - [Windows](#windows)
    - [Linux](#linux)
  - [Licence](#licence)
    - [Conditions Générales d'Utilisation](#conditions-générales-dutilisation)
      - [1. Permissions Accordées](#1-permissions-accordées)
      - [2. Conditions Obligatoires](#2-conditions-obligatoires)
      - [3. Restrictions et Limitations](#3-restrictions-et-limitations)
      - [4. Propriété Intellectuelle](#4-propriété-intellectuelle)
      - [5. Exclusion de Garanties et Limitation de Responsabilité](#5-exclusion-de-garanties-et-limitation-de-responsabilité)

## Qu'est-ce que Kainure ?

Kainure est un framework qui sert de pont entre le traditionnel San Andreas: Multiplayer (SA-MP) et le développement de logiciels moderne. Il vous permet d'écrire toute la logique de votre gamemode en utilisant **JavaScript** ou **TypeScript**, ouvrant un univers de possibilités avec des outils, des paquets et des pratiques de développement actuels.

Le plugin intègre un environnement **Node.js** directement dans votre serveur SA-MP. La version spécifique de Node.js utilisée est la **v22.21.1**, qui apporte avec elle la puissance de composants de haute performance :
- **Moteur V8 :** Le même moteur JavaScript de haute performance de **Google Chrome**, garantissant que votre code soit exécuté avec une vitesse et une efficacité impressionnantes.
- **libuv :** Une bibliothèque de support qui gère les E/S (opérations d'entrée et de sortie) de manière asynchrone, permettant à votre serveur de traiter plusieurs opérations simultanément sans se bloquer, idéal pour un environnement de jeu dynamique.

Avec Kainure, vous pouvez laisser de côté les limitations du Pawn et adopter un écosystème robuste pour créer des gamemodes plus complexes, organisés et faciles à maintenir.

## Comment Installer et Utiliser

### 1. Téléchargement

La version la plus récente de Kainure, incluant le plugin et tous les fichiers nécessaires, peut être trouvée sur la page de [**Releases**](https://github.com/aldergrounds/kainure/releases) du projet.

### 2. Structure des Fichiers

Après le téléchargement, vous devez organiser les fichiers à la racine de votre serveur SA-MP comme suit :

```
/ (Racine du Gamemode)
├── plugins/
│   └── Kainure.dll  (ou Kainure.so sous Linux)
├── Kainure/
│   ├── core/
│   ├── types/
│   ├── kainure.js
│   └── config.json (sera créé lors du premier démarrage s'il n'existe pas)
└── libnode.dll (ou libnode.so sous Linux)
```

**Important :**
- Le plugin `Kainure` doit être ajouté à votre fichier `server.cfg`.
- La bibliothèque `libnode` (`.dll` ou `.so`) doit se trouver à la **racine** du serveur, avec l'exécutable `samp-server` (ou `samp03svr` sous Linux).

### 3. Configuration (config.json)

Dans le dossier `Kainure/`, vous trouverez le fichier `config.json`. S'il n'existe pas, le plugin le créera automatiquement lors de la première exécution. Ce fichier contrôle le comportement du framework.

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

- **`main_file`**: C'est le point d'entrée de votre gamemode. Kainure commencera à exécuter le code à partir de ce fichier. Par défaut, il peut s'agir de `.js` ou `.ts` (si TypeScript est activé). Le chemin `./` représente la racine de votre gamemode.

- **`encoding.enabled`**: Définit si Kainure doit convertir les chaînes de caractères entre le format UTF-8 (standard JavaScript) et l'encodage de votre serveur. S'il est défini sur `false`, le framework utilisera le standard `65001` (UTF-8).

- **`encoding.target`**: Définit l'encodage cible.
   - **Windows**: L'utilisation du **numéro de Codepage** est obligatoire (ex : `1252`).
   - **Linux**: L'utilisation du **nom de la Codepage** est obligatoire (ex : `WINDOWS-1252`).

- **`typescript.enabled`**: Passez à `true` si vous souhaitez écrire votre code en TypeScript. Kainure transpilera automatiquement les fichiers `.ts` en `.js`.
- **`typescript.output_dir`**: Définit le répertoire où les fichiers JavaScript transpilés seront sauvegardés.

## API et Fonctionnalités

### Philosophie : Globaux et Sans Complication

Kainure a été conçu pour être intuitif. Vous **n'avez pas besoin d'importer ou de `require`** l'un des modules principaux (`api.js`, `commands.js`, etc.) dans vos fichiers. Le framework charge et injecte automatiquement toutes ses fonctionnalités dans la portée globale (`globalThis`), les rendant immédiatement accessibles dans n'importe quelle partie de votre code.

### Module Principal : `api.js`

Ce module contient les fonctions essentielles pour interagir avec le serveur SA-MP.

#### `Float`

Garantit qu'un nombre soit traité comme un `float`, même s'il pourrait être interprété comme un entier. Ceci est crucial pour les natives de SA-MP qui exigent des floats.

**Quand l'utiliser ?** Utilisez `Float()` uniquement lorsque la valeur est dynamique et inconnue au moment de la compilation (par exemple, venant d'une commande ou d'une autre fonction). Son utilisation manuelle n'est pertinente que lors du passage de valeurs à `Native.` et `Call_Public.`.

> [!IMPORTANT]
> Si vous écrivez une valeur explicite dans le code, comme `50.0`, le préprocesseur de code de Kainure appliquera **automatiquement** le traitement de `Float` avant de l'envoyer au moteur V8. Par conséquent, l'utilisation manuelle n'est nécessaire que dans des cas spécifiques.

```javascript
Command('sethealth', (playerid, params) => {
    const health = { value: 0.1 }; // "0.1" pour que JavaScript n'optimise pas en entier.

    if (!Command_Params(params, "f", health))
        return Native.SendClientMessage(playerid, -1, "Usage : /sethealth [vie]");

    // `health.value` est inconnu, nous utilisons donc Float() pour garantir le typage correct.
    Native.SetPlayerHealth(playerid, Float(health.value));
});
```

#### `Ref`

Marque une variable pour recevoir une valeur par référence d'une fonction native.

**Comment ça marche ?**
- Pour **recevoir** une valeur d'une native (paramètres de sortie), utilisez la syntaxe `.$` lors du passage de la variable.
- Pour **passer** la valeur contenue dans la variable à une native, utilisez-la normalement.
- La valeur initiale passée à `Ref()` détermine le type de donnée attendu.

**Initialisation :**
- **`int`**: `Ref()` ou `Ref(0)`
- **`float`**: `Ref(0.1)` (ou n'importe quel float qui ne se termine pas par un zéro absolu, pour éviter l'optimisation en entier par JavaScript)
- **`string`**: `Ref("")`
- **`bool`**: `Ref(true)` ou `Ref(false)`

```javascript
Command('health', (playerid) => {
    const health = Ref(0.1); // Nous nous attendons à recevoir un float.

    // Nous utilisons health.$ pour que la native remplisse la variable avec la vie du joueur.
    Native.GetPlayerHealth(playerid, health.$);

    // Nous utilisons seulement `health` pour lire la valeur. Le Ref est converti automatiquement en sa valeur.
    Native.SendClientMessage(playerid, -1, `Votre vie : ${health}`);
});
```

#### `Public`

Déclare un callback (public) de SA-MP, permettant à votre code de réagir aux événements du jeu.

**Signatures de Paramètres :** Pour que Kainure convertisse les types correctement, il est obligatoire de fournir une signature en tant que deuxième paramètre si le callback possède des arguments.
- **`i`**: Entier
- **`f`**: Float
- **`s`**: String
- **`b`**: Booléen

**Validations et Erreurs :** Kainure empêche l'exécution si :
1. La fonction possède des paramètres, mais aucune signature n'est fournie.
2. Le nombre de caractères dans la signature est différent du nombre de paramètres définis dans la fonction.
3. La signature contient des types invalides (différents de `i, f, s, b`).

**Valeurs de Retour :**
- `return 1;` ou `return true;` : Permet au callback de continuer à être exécuté dans d'autres scripts (le cas échéant). C'est le comportement par défaut si rien n'est retourné.
- `return 0;` ou `return false;` : Empêche l'exécution du callback dans d'autres scripts.

```javascript
// Public simple sans paramètres
Public('OnGameModeInit', () => {
    // Logique
    return true;
});

// Public avec signature
Public('OnPlayerText', 'is', (playerid, text) => {
    // Logique
    return true;
});
```

#### `Call_Public`

Appelle n'importe quelle public, qu'elle provienne du gamemode lui-même, d'un filterscript, ou d'une public personnalisée créée avec `Public()`.

```javascript
Command('callpublic', (playerid) => {
    // Appelle la public OnPlayerSpawn pour le joueur qui a utilisé la commande.
    Call_Public.OnPlayerSpawn(playerid);
});
```

#### `Native`

Invoque une fonction native de SA-MP ou de n'importe quel plugin chargé. Ajoutez simplement le préfixe `Native.` au nom de la fonction.

```javascript
Public('OnPlayerConnect', (playerid) => {
    Native.SendClientMessage(playerid, -1, "Bienvenue sur le serveur.");
});
```

#### `Native_Hook`

Intercepte l'appel d'une fonction native, permettant de modifier son comportement ou d'ajouter une logique supplémentaire.

**Ordre d'Exécution :** Les hooks suivent un modèle **LIFO (Last-In, First-Out)**. Le dernier hook créé pour une native sera le premier à être exécuté.

**Valeurs de Retour :**
- `return 1;` ou `return true;`: Permet à l'exécution de la native de continuer (appelant le prochain hook dans la pile ou la fonction originale).
- `return 0;` ou `return false;`: Bloque l'exécution de la native originale.

```javascript
Native_Hook('SetPlayerHealth', (playerid, health) => {
    console.log(`[Hook] : La vie de l'utilisateur ${playerid} a été définie à ${health}.`);
    
    // Permet à la native SetPlayerHealth originale d'être exécutée.
    return true; 
});
```

#### `Include_Storage`

Fournit un chemin de répertoire unique et sécurisé pour que les `includes` (bibliothèques) puissent stocker leurs propres données, évitant les conflits. Le dossier sera toujours créé dans `Kainure/includes_storage/`.

```javascript
// Retourne le chemin : "Kainure/includes_storage/MyInclude"
const my_data_path = Include_Storage("MyInclude"); 
```

### Module de Commandes : `commands.js`

Ce module offre un système complet pour la création et la gestion des commandes.

#### `Command`

Enregistre une nouvelle commande sur le serveur. La fonction de callback peut recevoir jusqu'à deux paramètres : `playerid` et `params`. Les deux sont optionnels.

```javascript
// Commande simple sans paramètres
Command('pos', (playerid) => {
    const x = Ref(0.1), y = Ref(0.1), z = Ref(0.1);

    Native.GetPlayerPos(playerid, x.$, y.$, z.$);
    Native.SendClientMessage(playerid, -1, `Position : ${x}, ${y}, ${z}`);
});
```

#### `Command_Params`

Traite et extrait des paramètres d'une chaîne de commande, de manière similaire à `sscanf`.

**Comment ça marche ?**
- La variable qui recevra la valeur doit être un objet avec une propriété `value` (ex : `{ value: 0 }`).
- La valeur initiale de la propriété `value` définit le type de donnée attendu.
- Après la fonction, la valeur extraite sera accessible via `.value`.

```javascript
Command('givemoney', (playerid, params) => {
    const giveid = { value: 0 }; // Attend un entier
    const amount = { value: 0 }; // Attend un entier

    if (!Command_Params(params, "ii", giveid, amount))
        return Native.SendClientMessage(playerid, -1, "Usage correct : /givemoney [ID] [Montant]");

    Native.GivePlayerMoney(giveid.value, amount.value);
    // Notez l'utilisation de .value pour accéder aux valeurs traitées.
});
```

#### `Alias_Command`

Crée un ou plusieurs alias (noms alternatifs) pour une commande existante.

```javascript
// Crée la commande principale
Command('showlife', (playerid) => { /* ... */ });

// Crée des alias pour la commande 'showlife'
Alias_Command('showlife', 'life', 'hp', 'health');
```
Maintenant, `/showlife`, `/life`, `/hp` et `/health` exécuteront le même code.

#### `Call_Command`

Appelle une commande par programmation à partir de votre code.

```javascript
// Exemple : Force le joueur 0 à exécuter /givemoney 1 500, c'est-à-dire qu'il va donner 500 d'argent au joueur 1.
Call_Command("givemoney", "1 500", 0 /* ou playerid */);

// Le playerid à la fin est optionnel. Cependant, cela est illogique, car les commandes sont basées sur les joueurs.
Call_Command("announcement", "Le serveur va redémarrer dans 1 minute !");
```

#### `External_Commands`

Cette fonction est utilisée pour enregistrer des commandes qui **n'ont pas été créées dans l'environnement Kainure**, mais qui existent dans d'autres scripts.

En enregistrant une commande comme externe, Kainure garantit que les callbacks globaux de commandes (`OnPlayerCommandReceived` et `OnPlayerCommandPerformed`) soient déclenchés correctement pour ces commandes, les intégrant ainsi au flux du framework.

```javascript
// Enregistrement de commandes existant dans un filterscript
External_Commands("login", "register", "help");
```

### Constantes et Types

#### `samp_constants.js`

Ce fichier contient toutes les constantes et définitions par défaut de SA-MP (comme `MAX_PLAYERS`, `WEAPON_DEAGLE`, etc.). Il est chargé globalement, vous pouvez donc utiliser ces constantes directement dans votre code sans avoir besoin de les déclarer.

#### Répertoire `types/`

Ce répertoire stocke les fichiers de définition de types (`.d.ts`) pour toute l'API de Kainure. Si vous utilisez TypeScript, ces fichiers fourniront l'autocomplétion, la vérification de types et une expérience de développement beaucoup plus riche et sûre dans votre éditeur de code.

## Gamemode Officiel

Pour accélérer votre apprentissage et voir Kainure en action, **AlderGrounds** maintient un gamemode complet et open source construit entièrement avec ce framework. Il sert de pièce fondamentale de documentation vivante et le meilleur endroit pour trouver des exemples pratiques.

Si vous avez des questions sur la façon de structurer votre code, d'utiliser l'API ou d'implémenter des fonctionnalités complexes, ce dépôt est votre principale source de référence.

➡️ **Explorez le Gamemode Officiel :** [Gamemode AlderGrounds](https://github.com/aldergrounds/gamemode)

## Compilation

Si vous souhaitez compiler Kainure à partir du code source, suivez les instructions pour votre système d'exploitation.

### Windows

1. **Prérequis** :
   - [Visual Studio](https://visualstudio.microsoft.com/) avec l'ensemble d'outils "Développement Desktop en C++" installé.
2. **Étapes** :
   - Naviguez vers le répertoire `build/Windows/`.
   - Ouvrez le fichier de solution approprié pour votre version de Visual Studio :
      - `Kainure.sln` : Pour Visual Studio 2022 ou inférieure.
      - `Kainure.slnx` : Pour Visual Studio 2026.
   - Dans Visual Studio, assurez-vous que la configuration de compilation est définie sur **Release** et la plateforme sur **x86**.
   - Compilez la solution (généralement en appuyant sur `Ctrl+Shift+B` ou via le menu `Build > Build Solution`).
   - Le `Kainure.dll` compilé se trouvera dans le dossier `compiled/Windows/`.

### Linux

1. **Prérequis** :
   - [Docker](https://www.docker.com/) installé et en cours d'exécution.
2. **Étapes** :
   - Le processus est automatisé par un script. Exécutez simplement la commande suivante depuis la racine du projet :
      ```bash
      ./build/Linux/compile.sh
      ```
   - Le script va :
      1. Construire une image Docker (`kainure-builder-x86`) avec toutes les dépendances nécessaires (Ubuntu 18.04, g++-9, CMake).
      2. Exécuter un conteneur à partir de l'image pour compiler le projet.
      3. Copier le `Kainure.so` résultant dans le dossier `compiled/Linux/`.
      4. Nettoyer l'image Docker créée.

## Licence

Copyright © **AlderGrounds**

Ce logiciel est sous licence selon les termes de la Licence Apache, Version 2.0 ("Licence") ; vous ne pouvez pas utiliser ce logiciel sauf en conformité avec la Licence. Une copie de la Licence peut être obtenue sur : [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)

### Conditions Générales d'Utilisation

#### 1. Permissions Accordées

La présente licence accorde, gratuitement, à toute personne obtenant une copie de ce logiciel et des fichiers de documentation associés, les droits suivants :
- Utiliser, copier, modifier et distribuer le logiciel sur tout support ou format, à toute fin, commerciale ou non commerciale
- Fusionner, publier, distribuer, sous-licencier et/ou vendre des copies du logiciel
- Autoriser les personnes auxquelles le logiciel est fourni à faire de même

#### 2. Conditions Obligatoires

Toutes les distributions du logiciel ou travaux dérivés doivent :
- Inclure une copie complète de cette licence
- Indiquer clairement toute modification apportée au code source original
- Préserver tous les avis de droits d'auteur, brevets, marques déposées et attributions
- Fournir une documentation adéquate des modifications mises en œuvre
- Maintenir l'avis de licence et de garantie sur toutes les copies

#### 3. Restrictions et Limitations

- Cette licence n'accorde pas la permission d'utiliser des marques déposées, des logos ou des noms commerciaux d'**AlderGrounds**
- Les contributions au code source doivent être sous licence selon les mêmes termes que cette licence
- L'utilisation des noms des contributeurs pour approuver ou promouvoir des produits dérivés de ce logiciel nécessite une autorisation préalable spécifique

#### 4. Propriété Intellectuelle

Le logiciel et toute la documentation associée sont protégés par des lois sur les droits d'auteur et des traités internationaux. **AlderGrounds** conserve tous les droits, titres et intérêts non expressément accordés par cette licence.

#### 5. Exclusion de Garanties et Limitation de Responsabilité

LE LOGICIEL EST FOURNI "TEL QUEL", SANS GARANTIE D'AUCUNE SORTE, EXPRESSE OU IMPLICITE, Y COMPRIS, MAIS SANS S'Y LIMITER, LES GARANTIES DE QUALITÉ MARCHANDE, D'ADÉQUATION À UN USAGE PARTICULIER ET D'ABSENCE DE CONTREFAÇON. 

EN AUCUN CAS LES AUTEURS OU TITULAIRES DES DROITS D'AUTEUR NE SERONT RESPONSABLES DE TOUTE RÉCLAMATION, DOMMAGE OU AUTRE RESPONSABILITÉ, QUE CE SOIT DANS LE CADRE D'UNE ACTION CONTRACTUELLE, DÉLICTUELLE OU AUTRE, DÉCOULANT DE, OU EN RELATION AVEC LE LOGICIEL OU L'UTILISATION OU D'AUTRES NÉGOCIATIONS DANS LE LOGICIEL.

---

Pour des informations détaillées sur la Licence Apache 2.0, consultez : http://www.apache.org/licenses/LICENSE-2.0