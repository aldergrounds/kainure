<div align="center">

# **Kainure**

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&style=for-the-badge)
![Node.js](https://img.shields.io/badge/Node.js-v22.21.1-339933?logo=node.js&style=for-the-badge)
![V8](https://img.shields.io/badge/V8-12.4-4B8BF5?logo=v8&style=for-the-badge)
![libuv](https://img.shields.io/badge/libuv-v1.48.0-blue?logo=libuv&style=for-the-badge)
![JavaScript](https://img.shields.io/badge/JavaScript-ES2023-F7DF1E?logo=javascript&style=for-the-badge)
![TypeScript](https://img.shields.io/badge/TypeScript-5.x-3178C6?logo=typescript&style=for-the-badge)
![License](https://img.shields.io/badge/License-Apache_2.0-blue?style=for-the-badge)

**JavaScript e TypeScript ile modern SA-MP geliştirme framework'ü.**

</div>

## Diller

- Português: [README](../../)
- Deutsch: [README](../Deutsch/README.md)
- English: [README](../English/README.md)
- Español: [README](../Espanol/README.md)
- Français: [README](../Francais/README.md)
- Italiano: [README](../Italiano/README.md)
- Polski: [README](../Polski/README.md)
- Русский: [README](../Русский/README.md)
- Svenska: [README](../Svenska/README.md)

## İçindekiler

- [**Kainure**](#kainure)
  - [Diller](#diller)
  - [İçindekiler](#i̇çindekiler)
  - [Kainure Nedir?](#kainure-nedir)
  - [Kurulum ve Kullanım](#kurulum-ve-kullanım)
    - [1. İndirme](#1-i̇ndirme)
    - [2. Dosya Yapısı](#2-dosya-yapısı)
    - [3. Yapılandırma (config.json)](#3-yapılandırma-configjson)
  - [API ve Özellikler](#api-ve-özellikler)
    - [Felsefe: Globaller ve Karmaşıklık Yok](#felsefe-globaller-ve-karmaşıklık-yok)
    - [Ana Modül: `api.js`](#ana-modül-apijs)
      - [`Float`](#float)
      - [`Ref`](#ref)
      - [`Public`](#public)
      - [`Call_Public`](#call_public)
      - [`Native`](#native)
      - [`Native_Hook`](#native_hook)
      - [`Include_Storage`](#include_storage)
    - [Komut Modülü: `commands.js`](#komut-modülü-commandsjs)
      - [`Command`](#command)
      - [`Command_Params`](#command_params)
      - [`Alias_Command`](#alias_command)
      - [`Call_Command`](#call_command)
      - [`External_Commands`](#external_commands)
    - [Sabitler ve Türler](#sabitler-ve-türler)
      - [`samp_constants.js`](#samp_constantsjs)
      - [`types/` Dizini](#types-dizini)
  - [Resmi Oyun Modu](#resmi-oyun-modu)
  - [Derleme](#derleme)
    - [Windows](#windows)
    - [Linux](#linux)
  - [Lisans](#lisans)
    - [Kullanım Şartları ve Koşulları](#kullanım-şartları-ve-koşulları)
      - [1. Verilen İzinler](#1-verilen-i̇zinler)
      - [2. Zorunlu Koşullar](#2-zorunlu-koşullar)
      - [3. Kısıtlamalar ve Sınırlamalar](#3-kısıtlamalar-ve-sınırlamalar)
      - [4. Fikri Mülkiyet](#4-fikri-mülkiyet)
      - [5. Garanti Reddi ve Sorumluluğun Sınırlandırılması](#5-garanti-reddi-ve-sorumluluğun-sınırlandırılması)

## Kainure Nedir?

Kainure, geleneksel San Andreas: Multiplayer (SA-MP) ile modern yazılım geliştirme arasında bir köprü görevi gören bir framework'tür. Oyun modunuzun tüm mantığını **JavaScript** veya **TypeScript** kullanarak yazmanıza olanak tanır, güncel geliştirme araçları, paketleri ve pratikleriyle bir olasılıklar evreni açar.

Eklenti, bir **Node.js** ortamını doğrudan SA-MP sunucunuza entegre eder. Kullanılan spesifik Node.js sürümü **v22.21.1**'dir ve beraberinde yüksek performanslı bileşenlerin gücünü getirir:
- **V8 Motoru:** **Google Chrome**'un aynı yüksek performanslı JavaScript motoru, kodunuzun etkileyici bir hız ve verimlilikle çalışmasını sağlar.
- **libuv:** I/O'yu (giriş ve çıkış işlemlerini) asenkron olarak yöneten bir destek kütüphanesi, sunucunuzun donmadan birden fazla işlemi aynı anda gerçekleştirmesine olanak tanır, dinamik bir oyun ortamı için idealdir.

Kainure ile Pawn'ın sınırlamalarını bir kenara bırakabilir ve daha karmaşık, organize ve bakımı kolay oyun modları oluşturmak için sağlam bir ekosistemi benimseyebilirsiniz.

## Kurulum ve Kullanım

### 1. İndirme

Eklenti ve gerekli tüm dosyaları içeren Kainure'nin en son sürümü, projenin [**Releases**](https://github.com/aldergrounds/kainure/releases) sayfasında bulunabilir.

### 2. Dosya Yapısı

İndirdikten sonra, dosyaları SA-MP sunucunuzun kök dizininde aşağıdaki gibi düzenlemeniz gerekir:

```
/ (Oyun Modu Kökü)
├── plugins/
│   └── Kainure.dll  (veya Linux'ta Kainure.so)
├── Kainure/
│   ├── core/
│   ├── types/
│   ├── kainure.js
│   └── config.json (yoksa ilk başlatmada oluşturulacaktır)
└── libnode.dll (veya Linux'ta libnode.so)
```

**Önemli:**
- Plugin `Kainure` dosyanız `server.cfg` içerisine eklenmelidir.
- Kütüphane `libnode` (`.dll` veya `.so`) sunucunun **kökünde**, `samp-server` (veya Linux'ta `samp03svr`) yürütülebilir dosyasıyla birlikte bulunmalıdır.

### 3. Yapılandırma (config.json)

`Kainure/` klasörünün içinde `config.json` dosyasını bulacaksınız. Eğer mevcut değilse, eklenti ilk kez çalıştırıldığında onu otomatik olarak oluşturacaktır. Bu dosya, framework'ün davranışını kontrol eder.

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

- **`main_file`**: Bu, gamemode'unuzun giriş noktasıdır. Kainure, kodu bu dosyadan itibaren yürütmeye başlayacaktır. Varsayılan olarak, `.js` veya `.ts` (eğer TypeScript etkinse) olabilir. `./` yolu gamemode'unuzun kök dizinini temsil eder.

- **`encoding.enabled`**: Kainure'un dizeleri UTF-8 formatı (JavaScript varsayılanı) ile sunucunuzun kodlaması arasında dönüştürüp dönüştürmeyeceğini belirler. `false` olarak ayarlanırsa, framework varsayılan `65001` (UTF-8) değerini kullanacaktır.

- **`encoding.target`**: Hedef kodlamayı belirler.
   - **Windows**: **Codepage numarasının** kullanılması zorunludur (örn: `1252`).
   - **Linux**: **Codepage adının** kullanılması zorunludur (örn: `WINDOWS-1252`).

- **`typescript.enabled`**: Kodunuzu TypeScript dilinde yazmak istiyorsanız `true` olarak değiştirin. Kainure, `.ts` dosyalarını otomatik olarak `.js` dosyalarına dönüştürecektir (transpile).
- **`typescript.output_dir`**: Dönüştürülen JavaScript dosyalarının kaydedileceği dizini belirler.

## API ve Özellikler

### Felsefe: Globaller ve Karmaşıklık Yok

Kainure sezgisel olacak şekilde tasarlandı. Dosyalarınızda ana modüllerin (`api.js`, `commands.js`, vb.) hiçbirini **içe aktarmanıza veya `require` etmenize gerek yoktur**. Framework, tüm işlevlerini otomatik olarak global kapsama (`globalThis`) yükler ve enjekte eder, böylece kodunuzun herhangi bir yerinde anında erişilebilir hale getirir.

### Ana Modül: `api.js`

Bu modül, SA-MP sunucusuyla etkileşim kurmak için temel işlevleri içerir.

#### `Float`

Bir sayının, tamsayı olarak yorumlanabilse bile `float` olarak ele alınmasını sağlar. Bu, float gerektiren SA-MP native'leri için çok önemlidir.

**Ne zaman kullanılır?** Sadece değer dinamikse ve derleme zamanında bilinmiyorsa (örneğin bir komuttan veya başka bir işlevden geliyorsa) `Float()` kullanın. Manuel kullanımı yalnızca `Native.` ve `Call_Public.`'e değer geçirirken geçerlidir.

> [!IMPORTANT]
> Kodda `50.0` gibi açık bir değer yazarsanız, Kainure kod ön işlemcisi bunu V8 motoruna göndermeden önce **otomatik olarak** `Float` işlemini uygular. Bu nedenle, manuel kullanım yalnızca belirli durumlarda gereklidir.

```javascript
Command('sethealth', (playerid, params) => {
    const health = { value: 0.1 }; // JavaScript'in tamsayıya optimize etmemesi için "0.1".

    if (!Command_Params(params, "f", health))
        return Native.SendClientMessage(playerid, -1, "Kullanım: /sethealth [can]");

    // `health.value` bilinmiyor, bu yüzden doğru tiplemeyi sağlamak için Float() kullanıyoruz.
    Native.SetPlayerHealth(playerid, Float(health.value));
});
```

#### `Ref`

Bir değişkeni, bir native fonksiyonundan referans yoluyla değer alacak şekilde işaretler.

**Nasıl çalışır?**
- Bir native'den değer **almak** için (çıktı parametreleri), değişkeni geçirirken `.$` sözdizimini kullanın.
- Değişkende bulunan değeri bir native'e **geçirmek** için normal şekilde kullanın.
- `Ref()`'e geçirilen başlangıç değeri beklenen veri tipini belirler.

**Başlatma:**
- **`int`**: `Ref()` veya `Ref(0)`
- **`float`**: `Ref(0.1)` (veya JavaScript tarafından tamsayı optimizasyonunu önlemek için mutlak sıfır ile bitmeyen herhangi bir float)
- **`string`**: `Ref("")`
- **`bool`**: `Ref(true)` veya `Ref(false)`

```javascript
Command('health', (playerid) => {
    const health = Ref(0.1); // Bir float almayı bekliyoruz.

    // Native'in değişkeni oyuncunun canıyla doldurması için health.$ kullanıyoruz.
    Native.GetPlayerHealth(playerid, health.$);

    // Değeri okumak için sadece `health` kullanıyoruz. Ref otomatik olarak değerine dönüştürülür.
    Native.SendClientMessage(playerid, -1, `Canınız: ${health}`);
});
```

#### `Public`

Bir SA-MP callback'ini (public) bildirerek kodunuzun oyun olaylarına tepki vermesini sağlar.

**Parametre İmzaları:** Kainure'un türleri doğru şekilde dönüştürebilmesi için, eğer callback argümanlara sahipse, ikinci parametre olarak bir imza sağlanması zorunludur.
- **`i`**: Tam Sayı (Integer)
- **`f`**: Float
- **`s`**: String
- **`b`**: Boole (Boolean)

**Doğrulamalar ve Hatalar:** Kainure şu durumlarda yürütmeyi engeller:
1. Fonksiyon parametrelere sahipse ancak hiçbir imza sağlanmamışsa.
2. İmzadaki karakter sayısı, fonksiyonda tanımlanan parametre sayısından farklıysa.
3. İmza geçersiz türler içeriyorsa (`i, f, s, b` dışında).

**Geri Dönüş Değerleri:**
- `return 1;` veya `return true;`: Callback'in diğer scriptlerde (varsa) yürütülmeye devam etmesine izin verir. Hiçbir şey döndürülmezse varsayılan davranış budur.
- `return 0;` veya `return false;`: Callback'in diğer scriptlerde yürütülmesini engeller.

```javascript
// Parametresiz basit Public
Public('OnGameModeInit', () => {
    // Mantık
    return true;
});

// İmzalı Public
Public('OnPlayerText', 'is', (playerid, text) => {
    // Mantık
    return true;
});
```

#### `Call_Public`

İster oyun modunun kendisinden, ister bir filterscript'ten veya `Public()` ile oluşturulmuş özel bir public olsun, herhangi bir public'i çağırır.

```javascript
Command('callpublic', (playerid) => {
    // Komutu kullanan oyuncu için OnPlayerSpawn public'ini çağırır.
    Call_Public.OnPlayerSpawn(playerid);
});
```

#### `Native`

SA-MP'nin veya yüklenen herhangi bir eklentinin native fonksiyonunu çağırır. Fonksiyon adının önüne sadece `Native.` ekleyin.

```javascript
Public('OnPlayerConnect', (playerid) => {
    Native.SendClientMessage(playerid, -1, "Sunucuya hoş geldiniz.");
});
```

#### `Native_Hook`

Bir native fonksiyon çağrısını yakalar, davranışını değiştirmeye veya ekstra mantık eklemeye izin verir.

**Yürütme Sırası:** Hook'lar **LIFO (Son Giren, İlk Çıkar)** modelini izler. Bir native için oluşturulan son hook, ilk çalıştırılacak olandır.

**Dönüş Değerleri:**
- `return 1;` veya `return true;`: Native'in çalışmaya devam etmesine izin verir (yığındaki bir sonraki hook'u veya orijinal fonksiyonu çağırarak).
- `return 0;` veya `return false;`: Orijinal native'in çalışmasını engeller.

```javascript
Native_Hook('SetPlayerHealth', (playerid, health) => {
    console.log(`[Hook]: ${playerid} kullanıcısının canı ${health} olarak ayarlandı.`);
    
    // Orijinal SetPlayerHealth native'inin çalışmasına izin verir.
    return true; 
});
```

#### `Include_Storage`

Include'ların (kütüphanelerin) çakışmaları önleyerek kendi verilerini saklayabilmeleri için benzersiz ve güvenli bir dizin yolu sağlar. Klasör her zaman `Kainure/includes_storage/` içinde oluşturulacaktır.

```javascript
// Yolu döndürür: "Kainure/includes_storage/MyInclude"
const my_data_path = Include_Storage("MyInclude"); 
```

### Komut Modülü: `commands.js`

Bu modül, komut oluşturma ve yönetimi için eksiksiz bir sistem sunar.

#### `Command`

Sunucuya yeni bir komut kaydeder. Callback fonksiyonu iki parametreye kadar alabilir: `playerid` ve `params`. Her ikisi de isteğe bağlıdır.

```javascript
// Parametresiz basit komut
Command('pos', (playerid) => {
    const x = Ref(0.1), y = Ref(0.1), z = Ref(0.1);

    Native.GetPlayerPos(playerid, x.$, y.$, z.$);
    Native.SendClientMessage(playerid, -1, `Pozisyon: ${x}, ${y}, ${z}`);
});
```

#### `Command_Params`

Bir komut dizesinden parametreleri `sscanf`'e benzer bir şekilde işler ve ayıklar.

**Nasıl çalışır?**
- Değeri alacak değişken `value` özelliğine sahip bir nesne olmalıdır (örn: `{ value: 0 }`).
- `value` özelliğinin başlangıç değeri beklenen veri tipini tanımlar.
- Fonksiyondan sonra, ayıklanan değere `.value` üzerinden erişilebilir.

```javascript
Command('givemoney', (playerid, params) => {
    const giveid = { value: 0 }; // Bir tamsayı bekler
    const amount = { value: 0 }; // Bir tamsayı bekler

    if (!Command_Params(params, "ii", giveid, amount))
        return Native.SendClientMessage(playerid, -1, "Doğru kullanım: /givemoney [ID] [Miktar]");

    Native.GivePlayerMoney(giveid.value, amount.value);
    // İşlenen değerlere erişmek için .value kullanımına dikkat edin.
});
```

#### `Alias_Command`

Mevcut bir komut için bir veya daha fazla takma ad (alternatif isimler) oluşturur.

```javascript
// Ana komutu oluşturur
Command('showlife', (playerid) => { /* ... */ });

// 'mostrarvida' komutu için takma adlar oluşturur
Alias_Command('showlife', 'life', 'hp', 'health');
```
Şimdi, `/showlife`, `/life`, `/hp` ve `/health` aynı kodu çalıştıracaktır.

#### `Call_Command`

Kodunuzdan programatik olarak bir komut çağırır.

```javascript
// Örnek: 0. oyuncuyu /dargrana 1 500 komutunu çalıştırmaya zorlar, yani oyuncu 1'e 500 para verir.
Call_Command("givemoney", "1 500", 0 /* veya playerid */);

// Sondaki playerid isteğe bağlıdır. Ancak, komutlar oyunculara dayalı olduğu için bu mantıksızdır.
Call_Command("announcement", "Sunucu 1 dakika içinde yeniden başlatılacak!");
```

#### `External_Commands`

Bu fonksiyon, **Kainure ortamında oluşturulmamış** ancak diğer scriptlerde mevcut olan komutları kaydetmek için kullanılır.

Bir komutu harici olarak kaydettiğinizde, Kainure bu komutlar için global komut callback'lerinin (`OnPlayerCommandReceived` ve `OnPlayerCommandPerformed`) doğru şekilde tetiklenmesini sağlayarak onları framework akışına entegre eder.

```javascript
// Bir filterscript içinde var olan komutları kaydetme
External_Commands("login", "register", "help");
```

### Sabitler ve Türler

#### `samp_constants.js`

Bu dosya SA-MP'nin tüm standart sabitlerini ve tanımlarını (MAX_PLAYERS, WEAPON_DEAGLE vb. gibi) içerir. Global olarak yüklenir, bu nedenle bunları bildirmenize gerek kalmadan doğrudan kodunuzda kullanabilirsiniz.

#### `types/` Dizini

Bu dizin, tüm Kainure API'si için tür tanımlama dosyalarını (`.d.ts`) saklar. TypeScript kullanıyorsanız, bu dosyalar kod düzenleyicinizde otomatik tamamlama, tür denetimi ve çok daha zengin ve güvenli bir geliştirme deneyimi sağlayacaktır.

## Resmi Oyun Modu

Öğreniminizi hızlandırmak ve Kainure'yi iş başında görmek için, **AlderGrounds** bu framework ile tamamen oluşturulmuş eksiksiz ve açık kaynaklı bir oyun modu sürdürmektedir. Bu, yaşayan bir belgelemenin temel parçası olarak hizmet eder ve pratik örnekler bulmak için en iyi yerdir.

Kodunuzu nasıl yapılandıracağınız, API'yi nasıl kullanacağınız veya karmaşık işlevleri nasıl uygulayacağınız konusunda sorularınız varsa, bu depo ana referans kaynağınızdır.

➡️ **Resmi Oyun Modunu Keşfedin:** [Gamemode AlderGrounds](https://github.com/aldergrounds/gamemode)

## Derleme

Kainure'yi kaynak kodundan derlemek istiyorsanız, işletim sisteminiz için talimatları izleyin.

### Windows

1. **Gereksinimler**:
   - [Visual Studio](https://visualstudio.microsoft.com/) "C++ ile masaüstü geliştirme" araç seti yüklü.
2. **Adımlar**:
   - `build/Windows/` dizinine gidin.
   - Visual Studio sürümünüz için uygun çözüm dosyasını açın:
      - `Kainure.sln`: Visual Studio 2022 veya daha düşük sürüm için.
      - `Kainure.slnx`: Visual Studio 2026 için.
   - Visual Studio içinde, derleme yapılandırmasının **Release** ve platformun **x86** olarak ayarlandığından emin olun.
   - Çözümü derleyin (genellikle `Ctrl+Shift+B` tuşlarına basarak veya `Build > Build Solution` menüsü aracılığıyla).
   - Derlenen `Kainure.dll` dosyası `compiled/Windows/` klasöründe bulunacaktır.

### Linux

1. **Gereksinimler**:
   - [Docker](https://www.docker.com/) yüklü ve çalışıyor.
2. **Adımlar**:
   - İşlem bir script tarafından otomatikleştirilmiştir. Projenin kök dizininden aşağıdaki komutu çalıştırmanız yeterlidir:
      ```bash
      ./build/Linux/compile.sh
      ```
   - Script şunları yapacaktır:
      1. Gerekli tüm bağımlılıklara (Ubuntu 18.04, g++-9, CMake) sahip bir Docker imajı (`kainure-builder-x86`) oluşturmak.
      2. Projeyi derlemek için imajdan bir konteyner çalıştırmak.
      3. Ortaya çıkan `Kainure.so` dosyasını `compiled/Linux/` klasörüne kopyalamak.
      4. Oluşturulan Docker imajını temizlemek.

## Lisans

Copyright © **AlderGrounds**

Bu yazılım Apache Lisansı, Sürüm 2.0 ("Lisans") koşulları altında lisanslanmıştır; bu yazılımı Lisans'a uygun olmadıkça kullanamazsınız. Lisansın bir kopyası şuradan edinilebilir: [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)

### Kullanım Şartları ve Koşulları

#### 1. Verilen İzinler

İşbu lisans, bu yazılımın ve ilgili dokümantasyon dosyalarının bir kopyasını edinen herkese ücretsiz olarak aşağıdaki hakları verir:
- Yazılımı herhangi bir ortamda veya formatta, ticari veya ticari olmayan herhangi bir amaçla kullanmak, kopyalamak, değiştirmek ve dağıtmak
- Yazılımın kopyalarını birleştirmek, yayınlamak, dağıtmak, alt lisanslamak ve/veya satmak
- Yazılımın sağlandığı kişilerin de aynısını yapmasına izin vermek

#### 2. Zorunlu Koşullar

Yazılımın veya türev çalışmaların tüm dağıtımları şunları yapmalıdır:
- Bu lisansın tam bir kopyasını eklemek
- Orijinal kaynak kodunda yapılan değişiklikleri açıkça belirtmek
- Tüm telif hakkı, patent, ticari marka ve atıf bildirimlerini korumak
- Uygulanan değişikliklerin uygun dokümantasyonunu sağlamak
- Tüm kopyalarda lisans ve garanti bildirimini muhafaza etmek

#### 3. Kısıtlamalar ve Sınırlamalar

- Bu lisans, **AlderGrounds**'un ticari markalarının, logolarının veya ticari adlarının kullanımına izin vermez
- Kaynak koduna yapılan katkılar, bu lisansla aynı koşullar altında lisanslanmalıdır
- Bu yazılımdan türetilen ürünleri onaylamak veya tanıtmak için katkıda bulunanların adlarının kullanılması, önceden özel izin gerektirir

#### 4. Fikri Mülkiyet

Yazılım ve ilgili tüm dokümantasyon, telif hakkı yasaları ve uluslararası anlaşmalarla korunmaktadır. **AlderGrounds**, bu lisansla açıkça verilmeyen tüm hakları, mülkiyetleri ve menfaatleri saklı tutar.

#### 5. Garanti Reddi ve Sorumluluğun Sınırlandırılması

YAZILIM "OLDUĞU GİBİ", TİCARİ ELVERİŞLİLİK, BELİRLİ BİR AMACA UYGUNLUK VE İHLAL ETMEME GARANTİLERİ DAHİL ANCAK BUNLARLA SINIRLI OLMAMAK ÜZERE, AÇIK VEYA ZIMNİ HİÇBİR GARANTİ OLMAKSIZIN SAĞLANIR.

HİÇBİR DURUMDA YAZARLAR VEYA TELİF HAKKI SAHİPLERİ, SÖZLEŞME DAVASI, HAKSIZ FİİL VEYA BAŞKA BİR ŞEKİLDE, YAZILIMDAN VEYA YAZILIMIN KULLANIMINDAN VEYA YAZILIMDAKİ DİĞER İŞLEMLERDEN KAYNAKLANAN VEYA BUNLARLA BAĞLANTILI OLAN HERHANGİ BİR İDDİA, HASAR VEYA DİĞER YÜKÜMLÜLÜKLERDEN SORUMLU OLMAYACAKTIR.

---

Apache Lisansı 2.0 hakkında ayrıntılı bilgi için bakınız: http://www.apache.org/licenses/LICENSE-2.0