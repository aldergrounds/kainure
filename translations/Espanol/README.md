<div align="center">

# **Kainure**

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&style=for-the-badge)
![Node.js](https://img.shields.io/badge/Node.js-v22.21.1-339933?logo=node.js&style=for-the-badge)
![V8](https://img.shields.io/badge/V8-12.4-4B8BF5?logo=v8&style=for-the-badge)
![libuv](https://img.shields.io/badge/libuv-v1.48.0-blue?logo=libuv&style=for-the-badge)
![JavaScript](https://img.shields.io/badge/JavaScript-ES2023-F7DF1E?logo=javascript&style=for-the-badge)
![TypeScript](https://img.shields.io/badge/TypeScript-5.x-3178C6?logo=typescript&style=for-the-badge)
![License](https://img.shields.io/badge/License-Apache_2.0-blue?style=for-the-badge)

**Framework para el desarrollo moderno de SA-MP con JavaScript y TypeScript.**

</div>

## Idiomas

- Português: [README](../../)
- Deutsch: [README](../Deutsch/README.md)
- English: [README](../English/README.md)
- Français: [README](../Francais/README.md)
- Italiano: [README](../Italiano/README.md)
- Polski: [README](../Polski/README.md)
- Русский: [README](../Русский/README.md)
- Svenska: [README](../Svenska/README.md)
- Türkçe: [README](../Turkce/README.md)

## Índice

- [**Kainure**](#kainure)
  - [Idiomas](#idiomas)
  - [Índice](#índice)
  - [¿Qué es Kainure?](#qué-es-kainure)
  - [Cómo Instalar y Usar](#cómo-instalar-y-usar)
    - [1. Download](#1-download)
    - [2. Estructura de Archivos](#2-estructura-de-archivos)
    - [3. Configuración (config.json)](#3-configuración-configjson)
  - [API y Funcionalidades](#api-y-funcionalidades)
    - [Filosofía: Globales y Sin Complicaciones](#filosofía-globales-y-sin-complicaciones)
    - [Módulo Principal: `api.js`](#módulo-principal-apijs)
      - [`Float`](#float)
      - [`Ref`](#ref)
      - [`Public`](#public)
      - [`Call_Public`](#call_public)
      - [`Native`](#native)
      - [`Native_Hook`](#native_hook)
      - [`Include_Storage`](#include_storage)
    - [Módulo de Comandos: `commands.js`](#módulo-de-comandos-commandsjs)
      - [`Command`](#command)
      - [`Command_Params`](#command_params)
      - [`Alias_Command`](#alias_command)
      - [`Call_Command`](#call_command)
    - [Constantes y Tipos](#constantes-y-tipos)
      - [`samp_constants.js`](#samp_constantsjs)
      - [Directorio `types/`](#directorio-types)
  - [Gamemode Oficial](#gamemode-oficial)
  - [Compilación](#compilación)
    - [Windows](#windows)
    - [Linux](#linux)
  - [Licencia](#licencia)
    - [Términos y Condiciones de Uso](#términos-y-condiciones-de-uso)
      - [1. Permisos Concedidos](#1-permisos-concedidos)
      - [2. Condiciones Obligatorias](#2-condiciones-obligatorias)
      - [3. Restricciones y Limitaciones](#3-restricciones-y-limitaciones)
      - [4. Propiedad Intelectual](#4-propiedad-intelectual)
      - [5. Exención de Garantías y Limitación de Responsabilidad](#5-exención-de-garantías-y-limitación-de-responsabilidad)

## ¿Qué es Kainure?

Kainure es un framework que sirve como un puente entre el tradicional San Andreas: Multiplayer (SA-MP) y el desarrollo de software moderno. Permite escribir toda la lógica de su gamemode utilizando **JavaScript** o **TypeScript**, abriendo un universo de posibilidades con herramientas, paquetes y prácticas de desarrollo actuales.

El plugin integra un entorno **Node.js** directamente en su servidor SA-MP. La versión específica de Node.js utilizada es la **v22.21.1**, que trae consigo el poder de componentes de alto rendimiento:
- **Motor V8:** El mismo motor JavaScript de alto rendimiento de **Google Chrome**, asegurando que su código se ejecute con una velocidad y eficiencia impresionantes.
- **libuv:** Una biblioteca de soporte que gestiona el I/O (operaciones de entrada y salida) de forma asíncrona, permitiendo que su servidor maneje múltiples operaciones simultáneamente sin bloquearse, ideal para un entorno de juego dinámico.

Con Kainure, puede dejar de lado las limitaciones de Pawn y adoptar un ecosistema robusto para crear gamemodes más complejos, organizados y fáciles de mantener.

## Cómo Instalar y Usar

### 1. Download

La versión más reciente de Kainure, incluyendo el plugin y todos los archivos necesarios, puede ser encontrada en la página de [**Releases**](https://github.com/aldergrounds/kainure/releases) del proyecto.

### 2. Estructura de Archivos

Después de la descarga, necesita organizar los archivos en la raíz de su servidor SA-MP de la siguiente manera:

```
/ (Raíz del Gamemode)
├── plugins/
│   └── Kainure.dll  (o Kainure.so en Linux)
├── Kainure/
│   ├── core/
│   ├── types/
│   ├── kainure.js
│   └── config.json (será creado en la primera inicialización si no existe)
└── libnode.dll (o libnode.so en Linux)
```

**Importante:**
- El plugin `Kainure` debe ser añadido a su archivo `server.cfg`.
- La biblioteca `libnode` (`.dll` o `.so`) debe estar en la **raíz** del servidor, junto con el ejecutable `samp-server` (o `samp03svr` en Linux).

### 3. Configuración (config.json)

Dentro de la carpeta `Kainure/`, encontrará el archivo `config.json`. Si no existe, el plugin lo creará automáticamente la primera vez que se ejecute. Este archivo controla el comportamiento del framework.

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

- **`main_file`**: Este es el punto de entrada de su gamemode. Kainure comenzará a ejecutar el código a partir de este archivo. Por defecto, puede ser `.js` o `.ts` (si TypeScript está activado). La ruta `./` representa la raíz de su gamemode.

- **`typescript.enabled`**: Cambie a `true` si desea escribir su código en TypeScript. Kainure transpilará automáticamente los archivos `.ts` a `.js`.

- **`typescript.output_dir`**: Define el directorio donde los archivos JavaScript transpilados serán guardados.

- **`typescript.auto_install`**: Si es `true`, Kainure verificará si el compilador de TypeScript (`typescript`) está presente en el `node_modules` de su proyecto. En caso de que no esté, lo instalará automáticamente, simplificando la configuración inicial.

## API y Funcionalidades

### Filosofía: Globales y Sin Complicaciones

Kainure fue diseñado para ser intuitivo. Usted **no necesita importar o hacer `require`** de ninguno de los módulos principales (`api.js`, `commands.js`, etc.) en sus archivos. El framework carga e inyecta automáticamente todas sus funcionalidades en el alcance global (`globalThis`), haciéndolas inmediatamente accesibles en cualquier parte de su código.

### Módulo Principal: `api.js`

Este módulo contiene las funciones esenciales para interactuar con el servidor SA-MP.

#### `Float`
Garantiza que un número sea tratado como `float`, incluso si puede ser interpretado como un entero. Esto es crucial para nativas de SA-MP que exigen floats.

**¿Cuándo usar?** Use `Float()` solo cuando el valor es dinámico y desconocido en tiempo de compilación (por ejemplo, viniendo de un comando o de otra función). Su uso manual es relevante solamente al pasar valores a `Native.` y `Call_Public.`.

> [!IMPORTANT]
> Si escribe un valor explícito en el código, como `50.0`, el preprocesador de código de Kainure **automáticamente** aplicará el tratamiento de `Float` antes de enviarlo al motor V8. Por lo tanto, el uso manual solo es necesario en casos específicos.

```javascript
Command('sethealth', (playerid, params) => {
    const health = { value: 0.1 }; // "0.1" para que JavaScript no optimice a entero.

    if (!Command_Params(params, "f", health))
        return Native.SendClientMessage(playerid, -1, "Uso: /sethealth [vida]");

    // `health.value` es desconocido, entonces usamos Float() para garantizar el tipado correcto.
    Native.SetPlayerHealth(playerid, Float(health.value));
});
```

#### `Ref`
Marca una variable para recibir un valor por referencia de una función nativa.

**¿Cómo funciona?**
- Para **recibir** un valor de una nativa (parámetros de salida), use la sintaxis `.$` al pasar la variable.
- Para **pasar** el valor contenido en la variable a una nativa, úsela normalmente.
- El valor inicial pasado a `Ref()` determina el tipo de dato esperado.

**Inicialización:**
- **`int`**: `Ref()` o `Ref(0)`
- **`float`**: `Ref(0.1)` (o cualquier float que no termine en cero absoluto, para evitar optimización a entero por JavaScript)
- **`string`**: `Ref("")`
- **`bool`**: `Ref(true)` o `Ref(false)`

```javascript
Command('health', (playerid) => {
    const health = Ref(0.1); // Esperamos recibir un float.

    // Usamos health.$ para que la nativa rellene la variable con la vida del jugador.
    Native.GetPlayerHealth(playerid, health.$);

    // Usamos solamente `health` para leer el valor. El Ref es convertido automáticamente a su valor.
    Native.SendClientMessage(playerid, -1, `Su vida: ${health}`);
});
```

#### `Public`
Declara un callback (public) de SA-MP, permitiendo que su código reaccione a eventos del juego.

**Firmas de Parámetros:**
Es **crucial** proporcionar una firma para parámetros que son `string` o `float` para que Kainure pueda convertir los tipos correctamente.
- **`string`**: `(param = "s")`
- **`float`**: `(param = "f")`
- `int` y `bool` no necesitan firma.

**Valores de Retorno:**
- `return 1;` o `return true;`: Permite que el callback continúe ejecutándose en otros scripts (si los hay). Este es el comportamiento predeterminado si no se devuelve nada.
- `return 0;` o `return false;`: Impide la ejecución del callback en otros scripts.

```javascript
// Public simple
Public('OnPlayerSpawn', (playerid) => {
    Native.SendClientMessage(playerid, -1, "Has aparecido en el servidor.");
    return true;
});

// Public con firma de string y float
Public('PublicCustom', (text = "s", value = "f") => {
    console.log(`Texto: ${text}, Valor: ${value}`);
});
```

#### `Call_Public`
Llama a cualquier public, ya sea del propio gamemode, de un filterscript, o una public personalizada creada con `Public()`.

```javascript
Command('callpublic', (playerid) => {
    // Llama a la public OnPlayerSpawn para el jugador que usó el comando.
    Call_Public.OnPlayerSpawn(playerid);
});
```

#### `Native`
Invoca una función nativa de SA-MP o de cualquier plugin cargado. Simplemente añada el prefijo `Native.` al nombre de la función.

```javascript
Public('OnPlayerConnect', (playerid) => {
    Native.SendClientMessage(playerid, -1, "Bienvenido al servidor.");
});
```

#### `Native_Hook`
Intercepta la llamada de una función nativa, permitiendo modificar su comportamiento o añadir lógica extra.

**Orden de Ejecución:** Los hooks siguen un patrón **LIFO (Last-In, First-Out)**. El último hook creado para una nativa será el primero en ejecutarse.

**Valores de Retorno:**
- `return 1;` o `return true;`: Permite que la ejecución de la nativa continúe (llamando al siguiente hook en la pila o a la función original).
- `return 0;` o `return false;`: Bloquea la ejecución de la nativa original.

```javascript
Native_Hook('SetPlayerHealth', (playerid, health) => {
    console.log(`[Hook]: La vida del usuario ${playerid} fue definida en ${health}.`);
    
    // Permite que la nativa SetPlayerHealth original sea ejecutada.
    return true; 
});
```

#### `Include_Storage`
Proporciona una ruta de directorio única y segura para que `includes` (bibliotecas) puedan almacenar sus propios datos, evitando conflictos. La carpeta siempre será creada dentro de `Kainure/includes_storage/`.

```javascript
// Retorna la ruta: "Kainure/includes_storage/MyInclude"
const my_data_path = Include_Storage("MyInclude"); 
```

### Módulo de Comandos: `commands.js`

Este módulo ofrece un sistema completo para creación y gestión de comandos.

#### `Command`
Registra un nuevo comando en el servidor. La función de callback puede recibir hasta dos parámetros: `playerid` y `params`. Ambos son opcionales.

```javascript
// Comando simple sin parámetros
Command('pos', (playerid) => {
    const x = Ref(0.1), y = Ref(0.1), z = Ref(0.1);

    Native.GetPlayerPos(playerid, x.$, y.$, z.$);
    Native.SendClientMessage(playerid, -1, `Posición: ${x}, ${y}, ${z}`);
});
```

#### `Command_Params`
Procesa y extrae parámetros de una cadena de comando, de forma similar a `sscanf`.

**¿Cómo funciona?**
- La variable que recibirá el valor debe ser un objeto con una propiedad `value` (ej: `{ value: 0 }`).
- El valor inicial de la propiedad `value` define el tipo de dato esperado.
- Después de la función, el valor extraído estará accesible a través de `.value`.

```javascript
Command('givemoney', (playerid, params) => {
    const giveid = { value: 0 }; // Espera un entero
    const amount = { value: 0 }; // Espera un entero

    if (!Command_Params(params, "ii", giveid, amount))
        return Native.SendClientMessage(playerid, -1, "Uso correcto: /givemoney [ID] [Cantidad]");

    Native.GivePlayerMoney(giveid.value, amount.value);
    // Note el uso de .value para acceder a los valores procesados.
});
```

#### `Alias_Command`
Crea uno o más alias (nombres alternativos) para un comando existente.

```javascript
// Crea el comando principal
Command('showlife', (playerid) => { /* ... */ });

// Crea alias para el comando 'mostrarvida'
Alias_Command('showlife', 'life', 'hp', 'health');
```
Ahora, `/showlife`, `/life`, `/hp` y `/health` ejecutarán el mismo código.

#### `Call_Command`
Llama a un comando programáticamente a partir de su código.

```javascript
// Ejemplo: Fuerza al jugador 0 a ejecutar /dargrana 1 500, o sea, él dará 500 de dinero al jugador 1.
Call_Command("givemoney", "1 500", 0 /* o playerid */);

// El playerid al final es opcional. Sin embargo, esto es ilógico, ya que los comandos se basan en los jugadores.
Call_Command("announcement", "¡El servidor se reiniciará en 1 minuto!");
```

### Constantes y Tipos

#### `samp_constants.js`
Este archivo contiene todas las constantes y definiciones estándar de SA-MP (como `MAX_PLAYERS`, `WEAPON_DEAGLE`, etc.). Se carga globalmente, así que puede usar estas constantes directamente en su código sin necesidad de declararlas.

#### Directorio `types/`
Este directorio almacena los archivos de definición de tipos (`.d.ts`) para toda la API de Kainure. Si está usando TypeScript, estos archivos proporcionarán autocompletado, verificación de tipos y una experiencia de desarrollo mucho más rica y segura en su editor de código.

## Gamemode Oficial

Para acelerar su aprendizaje y ver Kainure en acción, **AlderGrounds** mantiene un gamemode completo y de código abierto construido enteramente con este framework. Sirve como una pieza fundamental de documentación viva y el mejor lugar para encontrar ejemplos prácticos.

Si tiene dudas sobre cómo estructurar su código, usar la API o implementar funcionalidades complejas, este repositorio es su principal fuente de referencia.

➡️ **Explore el Gamemode Oficial:** [Gamemode AlderGrounds](https://github.com/aldergrounds/gamemode)

## Compilación

Si desea compilar Kainure a partir del código fuente, siga las instrucciones para su sistema operativo.

### Windows

1. **Requisitos**:
   - [Visual Studio](https://visualstudio.microsoft.com/) con el conjunto de herramientas "Desarrollo para el escritorio con C++" instalado.
2. **Pasos**:
   - Navegue hasta el directorio `build/Windows/`.
   - Abra el archivo de solución apropiado para su versión de Visual Studio:
      - `Kainure.sln`: Para Visual Studio 2022 o inferior.
      - `Kainure.slnx`: Para Visual Studio 2026.
   - Dentro de Visual Studio, asegúrese de que la configuración de compilación está definida como **Release** y la plataforma como **x86**.
   - Compile la solución (generalmente presionando `Ctrl+Shift+B` o a través del menú `Build > Build Solution`).
   - El `Kainure.dll` compilado será encontrado en la carpeta `compiled/Windows/`.

### Linux

1. **Requisitos**:
   - [Docker](https://www.docker.com/) instalado y en ejecución.
2. **Pasos**:
   - El proceso está automatizado por un script. Basta con ejecutar el siguiente comando desde la raíz del proyecto:
      ```bash
      ./build/Linux/compile.sh
      ```
   - El script hará:
      1. Construir una imagen Docker (`kainure-builder-x86`) con todas las dependencias necesarias (Ubuntu 18.04, g++-9, CMake).
      2. Ejecutar un contenedor a partir de la imagen para compilar el proyecto.
      3. Copiar el `Kainure.so` resultante a la carpeta `compiled/Linux/`.
      4. Limpiar la imagen Docker creada.

## Licencia

Copyright © **AlderGrounds**

Este software está licenciado bajo los términos de la Licencia Apache, Versión 2.0 ("Licencia"); usted no puede utilizar este software excepto en conformidad con la Licencia. Una copia de la Licencia se puede obtener en: [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)

### Términos y Condiciones de Uso

#### 1. Permisos Concedidos

La presente licencia concede, gratuitamente, a cualquier persona que obtenga una copia de este software y archivos de documentación asociados, los siguientes derechos:
- Utilizar, copiar, modificar y distribuir el software en cualquier medio o formato, para cualquier finalidad, comercial o no comercial
- Mezclar, publicar, distribuir, sublicenciar y/o vender copias del software
- Permitir que las personas para las cuales el software es suministrado hagan lo mismo

#### 2. Condiciones Obligatorias

Todas las distribuciones del software o trabajos derivados deben:
- Incluir una copia completa de esta licencia
- Indicar claramente cualquier modificación realizada en el código fuente original
- Preservar todos los avisos de derechos de autor, patentes, marcas registradas y atribuciones
- Proporcionar documentación adecuada de las alteraciones implementadas
- Mantener el aviso de licencia y garantía en todas las copias

#### 3. Restricciones y Limitaciones

- Esta licencia no concede permiso para el uso de marcas registradas, logotipos o nombres comerciales de **AlderGrounds**
- Las contribuciones para el código fuente deben ser licenciadas bajo los mismos términos de esta licencia
- El uso de nombres de los contribuidores para respaldar o promover productos derivados de este software requiere permiso previo específico

#### 4. Propiedad Intelectual

El software y toda la documentación asociada están protegidos por leyes de derechos de autor y tratados internacionales. **AlderGrounds** retiene todos los derechos, títulos e intereses no expresamente concedidos por esta licencia.

#### 5. Exención de Garantías y Limitación de Responsabilidad

EL SOFTWARE SE PROPORCIONA "TAL CUAL", SIN GARANTÍAS DE NINGÚN TIPO, EXPRESAS O IMPLÍCITAS, INCLUYENDO, PERO NO LIMITADO A, GARANTÍAS DE COMERCIALIZACIÓN, IDONEIDAD PARA UN PROPÓSITO ESPECÍFICO Y NO VIOLACIÓN.

EN NINGUNA CIRCUNSTANCIA LOS AUTORES O TITULARES DOS DERECHOS DE AUTOR SERÁN RESPONSABLES POR CUALQUIER RECLAMACIÓN, DAÑOS U OTRAS RESPONSABILIDADES, YA SEA EN ACCIÓN DE CONTRATO, AGRAVIO O DE OTRA FORMA, DERIVADAS DE, O EN CONEXIÓN CON EL SOFTWARE O EL USO U OTRAS NEGOCIACIONES EN EL SOFTWARE.

---

Para información detallada sobre la Licencia Apache 2.0, consulte: http://www.apache.org/licenses/LICENSE-2.0