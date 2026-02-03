<div align="center">

# **Kainure**

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&style=for-the-badge)
![Node.js](https://img.shields.io/badge/Node.js-v22.21.1-339933?logo=node.js&style=for-the-badge)
![V8](https://img.shields.io/badge/V8-12.4-4B8BF5?logo=v8&style=for-the-badge)
![libuv](https://img.shields.io/badge/libuv-v1.48.0-blue?logo=libuv&style=for-the-badge)
![JavaScript](https://img.shields.io/badge/JavaScript-ES2023-F7DF1E?logo=javascript&style=for-the-badge)
![TypeScript](https://img.shields.io/badge/TypeScript-5.x-3178C6?logo=typescript&style=for-the-badge)
![License](https://img.shields.io/badge/License-Apache_2.0-blue?style=for-the-badge)

**Framework para o desenvolvimento moderno de SA-MP com JavaScript e TypeScript.**

</div>

## Idiomas

- Deutsch: [README](translations/Deutsch/README.md)
- English: [README](translations/English/README.md)
- Español: [README](translations/Espanol/README.md)
- Français: [README](translations/Francais/README.md)
- Italiano: [README](translations/Italiano/README.md)
- Polski: [README](translations/Polski/README.md)
- Русский: [README](translations/Русский/README.md)
- Svenska: [README](translations/Svenska/README.md)
- Türkçe: [README](translations/Turkce/README.md)

## Índice

- [**Kainure**](#kainure)
  - [Idiomas](#idiomas)
  - [Índice](#índice)
  - [O que é o Kainure?](#o-que-é-o-kainure)
  - [Como Instalar e Usar](#como-instalar-e-usar)
    - [1. Download](#1-download)
    - [2. Estrutura de Arquivos](#2-estrutura-de-arquivos)
    - [3. Configuração (config.json)](#3-configuração-configjson)
  - [API e Funcionalidades](#api-e-funcionalidades)
    - [Filosofia: Globais e Sem Complicação](#filosofia-globais-e-sem-complicação)
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
      - [`External_Commands`](#external_commands)
    - [Constantes e Tipos](#constantes-e-tipos)
      - [`samp_constants.js`](#samp_constantsjs)
      - [Diretório `types/`](#diretório-types)
  - [Gamemode Oficial](#gamemode-oficial)
  - [Compilação](#compilação)
    - [Windows](#windows)
    - [Linux](#linux)
  - [Licença](#licença)
    - [Termos e Condições de Uso](#termos-e-condições-de-uso)
      - [1. Permissões Concedidas](#1-permissões-concedidas)
      - [2. Condições Obrigatórias](#2-condições-obrigatórias)
      - [3. Restrições e Limitações](#3-restrições-e-limitações)
      - [4. Propriedade Intelectual](#4-propriedade-intelectual)
      - [5. Isenção de Garantias e Limitação de Responsabilidade](#5-isenção-de-garantias-e-limitação-de-responsabilidade)

## O que é o Kainure?

O Kainure é um framework que serve como uma ponte entre o tradicional San Andreas: Multiplayer (SA-MP) e o desenvolvimento de software moderno. Ele permite que você escreva toda a lógica do seu gamemode utilizando **JavaScript** ou **TypeScript**, abrindo um universo de possibilidades com ferramentas, pacotes e práticas de desenvolvimento atuais.

O plugin integra um ambiente **Node.js** diretamente no seu servidor SA-MP. A versão específica do Node.js utilizada é a **v22.21.1**, que traz consigo o poder de componentes de alta performance:
- **Motor V8:** O mesmo motor JavaScript de alta performance do **Google Chrome**, garantindo que seu código seja executado com velocidade e eficiência impressionantes.
- **libuv:** Uma biblioteca de suporte que gerencia o I/O (operações de entrada e saída) de forma assíncrona, permitindo que seu servidor lide com múltiplas operações simultaneamente sem travar, ideal para um ambiente de jogo dinâmico.

Com o Kainure, você pode deixar de lado as limitações do Pawn e abraçar um ecossistema robusto para criar gamemodes mais complexos, organizados e fáceis de manter.

## Como Instalar e Usar

### 1. Download

A versão mais recente do Kainure, incluindo o plugin e todos os arquivos necessários, pode ser encontrada na página de [**Releases**](https://github.com/aldergrounds/kainure/releases) do projeto.

### 2. Estrutura de Arquivos

Após o download, você precisa organizar os arquivos na raiz do seu servidor SA-MP da seguinte forma:

```
/ (Raiz do Gamemode)
├── plugins/
│   └── Kainure.dll  (ou Kainure.so no Linux)
├── Kainure/
│   ├── core/
│   ├── types/
│   ├── kainure.js
│   └── config.json (será criado na primeira inicialização se não existir)
└── libnode.dll (ou libnode.so no Linux)
```

**Importante:**
- O plugin `Kainure` deve ser adicionado ao seu arquivo `server.cfg`.
- A biblioteca `libnode` (`.dll` ou `.so`) deve estar na **raiz** do servidor, junto com o executável `samp-server` (ou `samp03svr` no Linux).

### 3. Configuração (config.json)

Dentro da pasta `Kainure/`, você encontrará o arquivo `config.json`. Se ele não existir, o plugin o criará automaticamente na primeira vez que for executado. Este arquivo controla o comportamento do framework.

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

- **`main_file`**: Este é o ponto de entrada do seu gamemode. O Kainure começará a executar o código a partir deste arquivo. Por padrão, ele pode ser `.js` ou `.ts` (se o TypeScript estiver ativado). O caminho `./` representa a raiz do seu gamemode.

- **`encoding.enabled`**: Define se o Kainure deve converter as strings entre o formato UTF-8 (padrão do JavaScript) e a codificação do seu servidor. Se definido como `false`, o framework utilizará o padrão `65001` (UTF-8).

- **`encoding.target`**: Define a codificação alvo.
   - **Windows**: É obrigatório o uso do **número da Codepage** (ex: `1252`).
   - **Linux**: É obrigatório o uso do **nome da Codepage** (ex: `WINDOWS-1252`).

- **`typescript.enabled`**: Mude para `true` se você deseja escrever seu código em TypeScript. O Kainure irá transpilar automaticamente os arquivos `.ts` para `.js`.
- **`typescript.output_dir`**: Define o diretório onde os arquivos JavaScript transpilados serão salvos.

## API e Funcionalidades

### Filosofia: Globais e Sem Complicação

O Kainure foi projetado para ser intuitivo. Você **não precisa importar ou `require`** nenhum dos módulos principais (`api.js`, `commands.js`, etc.) nos seus arquivos. O framework carrega e injeta automaticamente todas as suas funcionalidades no escopo global (`globalThis`), tornando-as imediatamente acessíveis em qualquer parte do seu código.

### Módulo Principal: `api.js`

Este módulo contém as funções essenciais para interagir com o servidor SA-MP.

#### `Float`

Garante que um número seja tratado como `float`, mesmo que ele possa ser interpretado como um inteiro. Isso é crucial para nativas do SA-MP que exigem floats.

**Quando usar?** Use `Float()` apenas quando o valor é dinâmico e desconhecido em tempo de compilação (por exemplo, vindo de um comando ou de outra função). Seu uso manual é relevante somente ao passar valores para `Native.` e `Call_Public.`.

> [!IMPORTANT]
> Se você escrever um valor explícito no código, como `50.0`, o pré-processador de código do Kainure **automaticamente** aplicará o tratamento de `Float` antes de enviá-lo para o motor V8. Portanto, o uso manual só é necessário em casos específicos.

```javascript
Command('sethealth', (playerid, params) => {
    const health = { value: 0.1 }; // "0.1" para o JavaScript não otimizar para inteiro.

    if (!Command_Params(params, "f", health))
        return Native.SendClientMessage(playerid, -1, "Uso: /sethealth [vida]");

    // `health.value` é desconhecido, então usamos Float() para garantir a tipagem correta.
    Native.SetPlayerHealth(playerid, Float(health.value));
});
```

#### `Ref`

Marca uma variável para receber um valor por referência de uma função nativa.

**Como funciona?**
- Para **receber** um valor de uma nativa (parâmetros de saída), use a sintaxe `.$` ao passar a variável.
- Para **passar** o valor contido na variável para uma nativa, use-a normalmente.
- O valor inicial passado para `Ref()` determina o tipo de dado esperado.

**Inicialização:**
- **`int`**: `Ref()` ou `Ref(0)`
- **`float`**: `Ref(0.1)` (ou qualquer float que não termine em zero absoluto, para evitar otimização para inteiro pelo JavaScript)
- **`string`**: `Ref("")`
- **`bool`**: `Ref(true)` ou `Ref(false)`

```javascript
Command('health', (playerid) => {
    const health = Ref(0.1); // Esperamos receber um float.

    // Usamos health.$ para que a nativa preencha a variável com a vida do jogador.
    Native.GetPlayerHealth(playerid, health.$);

    // Usamos somente `health` para ler o valor. O Ref é convertido automaticamente para seu valor.
    Native.SendClientMessage(playerid, -1, `Sua vida: ${health}`);
});
```

#### `Public`

Declara uma callback (public) do SA-MP, permitindo que seu código reaja a eventos do jogo.

**Assinaturas de Parâmetros:** Para que o Kainure converta os tipos corretamente, é obrigatório fornecer uma assinatura como segundo parâmetro caso a callback possua argumentos.
- **`i`**: Inteiro
- **`f`**: Float
- **`s`**: String
- **`b`**: Booleano

**Validações e Erros:** O Kainure impede a execução se:
1. A função possuir parâmetros, mas nenhuma assinatura for fornecida.
2. A quantidade de caracteres na assinatura for diferente da quantidade de parâmetros definidos na função.
3. A assinatura contiver tipos inválidos (diferentes de `i, f, s, b`).

**Valores de Retorno:**
- `return 1;` ou `return true;`: Permite que a callback continue a ser executada em outros scripts (se houver). Este é o comportamento padrão se nada for retornado.
- `return 0;` ou `return false;`: Impede a execução da callback em outros scripts.

```javascript
// Public simples sem parâmetros
Public('OnGameModeInit', () => {
    // Lógica
    return true;
});

// Public com assinatura
Public('OnPlayerText', 'is', (playerid, text) => {
    // Lógica
    return true;
});
```

#### `Call_Public`

Chama qualquer public, seja ela do próprio gamemode, de um filterscript, ou uma public customizada criada com `Public()`.

```javascript
Command('callpublic', (playerid) => {
    // Chama a public OnPlayerSpawn para o jogador que usou o comando.
    Call_Public.OnPlayerSpawn(playerid);
});
```

#### `Native`

Invoca uma função nativa do SA-MP ou de qualquer plugin carregado. Simplesmente adicione o prefixo `Native.` ao nome da função.

```javascript
Public('OnPlayerConnect', (playerid) => {
    Native.SendClientMessage(playerid, -1, "Seja bem vindo ao servidor.");
});
```

#### `Native_Hook`

Intercepta a chamada de uma função nativa, permitindo modificar seu comportamento ou adicionar lógica extra.

**Ordem de Execução:** Os hooks seguem um padrão **LIFO (Last-In, First-Out)**. O último hook criado para uma nativa será o primeiro a ser executado.

**Valores de Retorno:**
- `return 1;` ou `return true;`: Permite que a execução da nativa continue (chamando o próximo hook na pilha ou a função original).
- `return 0;` ou `return false;`: Bloqueia a execução da nativa original.

```javascript
Native_Hook('SetPlayerHealth', (playerid, health) => {
    console.log(`[Hook]: A vida do usuário ${playerid} foi definida para ${health}.`);
    
    // Permite que a nativa SetPlayerHealth original seja executada.
    return true; 
});
```

#### `Include_Storage`

Fornece um caminho de diretório único e seguro para que `includes` (bibliotecas) possam armazenar seus próprios dados, evitando conflitos. A pasta sempre será criada dentro de `Kainure/includes_storage/`.

```javascript
// Retorna o caminho: "Kainure/includes_storage/MyInclude"
const my_data_path = Include_Storage("MyInclude"); 
```

### Módulo de Comandos: `commands.js`

Este módulo oferece um sistema completo para criação e gerenciamento de comandos.

#### `Command`

Registra um novo comando no servidor. A função de callback pode receber até dois parâmetros: `playerid` e `params`. Ambos são opcionais.

```javascript
// Comando simples sem parâmetros
Command('pos', (playerid) => {
    const x = Ref(0.1), y = Ref(0.1), z = Ref(0.1);

    Native.GetPlayerPos(playerid, x.$, y.$, z.$);
    Native.SendClientMessage(playerid, -1, `Posição: ${x}, ${y}, ${z}`);
});
```

#### `Command_Params`

Processa e extrai parâmetros de uma string de comando, de forma semelhante ao `sscanf`.

**Como funciona?**
- A variável que receberá o valor deve ser um objeto com uma propriedade `value` (ex: `{ value: 0 }`).
- O valor inicial da propriedade `value` define o tipo de dado esperado.
- Após a função, o valor extraído estará acessível através de `.value`.

```javascript
Command('givemoney', (playerid, params) => {
    const giveid = { value: 0 }; // Espera um inteiro
    const amount = { value: 0 }; // Espera um inteiro

    if (!Command_Params(params, "ii", giveid, amount))
        return Native.SendClientMessage(playerid, -1, "Uso correto: /givemoney [ID] [Quantia]");

    Native.GivePlayerMoney(giveid.value, amount.value);
    // Note o uso de .value para acessar os valores processados.
});
```

#### `Alias_Command`

Cria um ou mais aliases (nomes alternativos) para um comando existente.

```javascript
// Cria o comando principal
Command('showlife', (playerid) => { /* ... */ });

// Cria aliases para o comando 'mostrarvida'
Alias_Command('showlife', 'life', 'hp', 'health');
```
Agora, `/showlife`, `/life`, `/hp` e `/health` executarão o mesmo código.

#### `Call_Command`

Chama um comando programaticamente a partir do seu código.

```javascript
// Exemplo: Força o jogador 0 a executar /dargrana 1 500, ou seja, ele vai dar 500 de dinheiro para o jogador 1.
Call_Command("givemoney", "1 500", 0 /* ou playerid */);

// O playerid no final é opcional. Porém, isso é ilógico, já que os comandos são baseados nos jogadores.
Call_Command("announcement", "O servidor vai reiniciar em 1 minuto!");
```

#### `External_Commands`

Esta função é utilizada para registrar comandos que **não foram criados no ambiente Kainure**, mas que existem em outros scripts.

Ao registrar um comando como externo, o Kainure garante que as callbacks globais de comandos (`OnPlayerCommandReceived` e `OnPlayerCommandPerformed`) sejam disparadas corretamente para esses comandos, integrando-os ao fluxo do framework.

```javascript
// Registrando comandos que existem em um filterscript
External_Commands("login", "register", "help");
```

### Constantes e Tipos

#### `samp_constants.js`

Este arquivo contém todas as constantes e definições padrão do SA-MP (como `MAX_PLAYERS`, `WEAPON_DEAGLE`, etc.). Ele é carregado globalmente, então você pode usar essas constantes diretamente no seu código sem precisar declará-las.

#### Diretório `types/`

Este diretório armazena os arquivos de definição de tipos (`.d.ts`) para toda a API do Kainure. Se você estiver usando TypeScript, esses arquivos fornecerão autocompletar, verificação de tipos e uma experiência de desenvolvimento muito mais rica e segura no seu editor de código.

## Gamemode Oficial

Para acelerar seu aprendizado e ver o Kainure em ação, a **AlderGrounds** mantém um gamemode completo e de código aberto construído inteiramente com este framework. Ele serve como uma peça fundamental de documentação viva e o melhor lugar para encontrar exemplos práticos.

Se você tiver dúvidas sobre como estruturar seu código, usar a API ou implementar funcionalidades complexas, este repositório é sua principal fonte de referência.

➡️ **Explore o Gamemode Oficial:** [Gamemode AlderGrounds](https://github.com/aldergrounds/gamemode)

## Compilação

Se você deseja compilar o Kainure a partir do código-fonte, siga as instruções para o seu sistema operacional.

### Windows

1. **Requisitos**:
   - [Visual Studio](https://visualstudio.microsoft.com/) com o conjunto de ferramentas "Desenvolvimento para desktop com C++" instalado.
2. **Passos**:
   - Navegue até o diretório `build/Windows/`.
   - Abra o arquivo de solução apropriado para sua versão do Visual Studio:
      - `Kainure.sln`: Para Visual Studio 2022 ou inferior.
      - `Kainure.slnx`: Para Visual Studio 2026.
   - Dentro do Visual Studio, certifique-se de que a configuração de compilação está definida como **Release** e a plataforma como **x86**.
   - Compile a solução (geralmente pressionando `Ctrl+Shift+B` ou através do menu `Build > Build Solution`).
   - O `Kainure.dll` compilado será encontrado na pasta `compiled/Windows/`.

### Linux

1. **Requisitos**:
   - [Docker](https://www.docker.com/) instalado e em execução.
2. **Passos**:
   - O processo é automatizado por um script. Basta executar o seguinte comando a partir da raiz do projeto:
      ```bash
      ./build/Linux/compile.sh
      ```
   - O script irá:
      1. Construir uma imagem Docker (`kainure-builder-x86`) com todas as dependências necessárias (Ubuntu 18.04, g++-9, CMake).
      2. Executar um container a partir da imagem para compilar o projeto.
      3. Copiar o `Kainure.so` resultante para a pasta `compiled/Linux/`.
      4. Limpar a imagem Docker criada.

## Licença

Copyright © **AlderGrounds**

Este software é licenciado sob os termos da Licença Apache, Versão 2.0 ("Licença"); você não pode utilizar este software exceto em conformidade com a Licença. Uma cópia da Licença pode ser obtida em: [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)

### Termos e Condições de Uso

#### 1. Permissões Concedidas

A presente licença concede, gratuitamente, a qualquer pessoa que obtenha uma cópia deste software e arquivos de documentação associados, os seguintes direitos:
- Utilizar, copiar, modificar e distribuir o software em qualquer meio ou formato, para qualquer finalidade, comercial ou não-comercial
- Mesclar, publicar, distribuir, sublicenciar e/ou vender cópias do software
- Permitir que pessoas para as quais o software é fornecido façam o mesmo

#### 2. Condições Obrigatórias

Todas as distribuições do software ou trabalhos derivados devem:
- Incluir uma cópia completa desta licença
- Indicar claramente quaisquer modificações realizadas no código-fonte original
- Preservar todos os avisos de direitos autorais, patentes, marcas registradas e atribuições
- Fornecer documentação adequada das alterações implementadas
- Manter o aviso de licença e garantia em todas as cópias

#### 3. Restrições e Limitações

- Esta licença não concede permissão para uso de marcas registradas, logotipos ou nomes comerciais da **AlderGrounds**
- As contribuições para o código-fonte devem ser licenciadas sob os mesmos termos desta licença
- O uso de nomes dos contribuidores para endossar ou promover produtos derivados deste software requer permissão prévia específica

#### 4. Propriedade Intelectual

O software e toda a documentação associada são protegidos por leis de direitos autorais e tratados internacionais. A **AlderGrounds** retém todos os direitos, títulos e interesses não expressamente concedidos por esta licença.

#### 5. Isenção de Garantias e Limitação de Responsabilidade

O SOFTWARE É FORNECIDO "COMO ESTÁ", SEM GARANTIAS DE QUALQUER NATUREZA, EXPRESSAS OU IMPLÍCITAS, INCLUINDO, MAS NÃO SE LIMITANDO A, GARANTIAS DE COMERCIALIZAÇÃO, ADEQUAÇÃO A UM PROPÓSITO ESPECÍFICO E NÃO VIOLAÇÃO. 

EM NENHUMA CIRCUNSTÂNCIA OS AUTORES OU TITULARES DOS DIREITOS AUTORAIS SERÃO RESPONSÁVEIS POR QUAISQUER REIVINDICAÇÕES, DANOS OU OUTRAS RESPONSABILIDADES, SEJA EM AÇÃO DE CONTRATO, DELITO OU DE OUTRA FORMA, DECORRENTES DE, OU EM CONEXÃO COM O SOFTWARE OU O USO OU OUTRAS NEGOCIAÇÕES NO SOFTWARE.

---

Para informações detalhadas sobre a Licença Apache 2.0, consulte: http://www.apache.org/licenses/LICENSE-2.0