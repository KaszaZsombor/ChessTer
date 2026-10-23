# ChessTer

## 2026/27 őszi félév, sakk program dokumentáció

Ez a dokumentum a Sakk program telepítését, használatát és főbb funkcióit ismerteti.

A dokumentáció célja, hogy a felhasználók megismerjék a program működését, a rendelkezésre álló lehetőségeket és a használat lépéseit.

---

## Linkek

### [Github](https://github.com/KaszaZsombor/SAKK)

### [Dokumentáció](https://docs.kaszazsombor.hu)

---

## Funkciók

### 1. Sakktábla megjelenítése

A program megjeleníti a sakktáblát és a sakkbábukat.

A felhasználó a táblán követheti az aktuális játékállást, és kiválaszthatja a mozgatni kívánt bábut.

### Lépések megadása

A lépéseket a következő formátumok egyikében kell megadni:

- **Normál lépés:** `e2e4` – az `e2` mezőről az `e4` mezőre történő lépés.
- **Gyalog átváltoztatása:** `f7f8Q` – a gyalog az `f7` mezőről az `f8` mezőre lép, majd vezérré változik.

A megadott formátum felépítése:
    - Az első két karakter a kiinduló mezőt jelöli.
    - A következő két karakter a célmezőt jelöli.
    - Átváltoztatás esetén az utolsó karakter a választott bábu jele, például `Q` (vezér).
    - Ha átváltozás esetén nincs megadva bábu, automatikusan vezérré alakul.

**Példák:**
    - `e2e4` – lépés az e2 mezőről az e4 mezőre.
    - `g1f3` – lépés a g1 mezőről az f3 mezőre.
    - `f7f8Q` – gyaloglépés vezérré változtatással.

### 3. Szabályos sakk-lépések ellenőrzése

A program a sakk szabályai alapján ellenőrzi a lehetséges lépéseket.

### 4. További funkciók

- Sakk és sakk-matt felismerése.
- Patthelyzet felismerése.
- Gyalogátváltozás kezelése.
- Lépések és játékállások megjelenítése.
- 50 lépéses szabály ellenőrzése.

---

## Telepítési útmutató

### 1. A bináris fájl letöltése

A releases oldal megnyitása:

![Release image](docs/images/relese1.png)

Válasszon egy verziót, majd töltse le a .exe fájlt.

---

### 2. Projekt forráskód letöltése

**Az egyik lehetőség a zip-fájl letöltése:**

![source 1](docs/images/source1.png)

![source 2](docs/images/source2.png)

**Vagy klónozás gitről:**

```bash
git clone https://github.com/KaszaZsombor/ChessTer.git
```

### Környezet ellenőrzése

**gcc ellenőrzése:**

```bash
gcc --version
```

**Make ellenőrzése:**

```bash
make --version
```

### Projekt építése forráskódból

Lépjen a kicsomagolt projekt mappájába, majd futtassa:

```bash
make
```

Ha make nem található:

```bash
mkdir bin
gcc -Iinclude -c source/board.c -o bin/board.o
gcc -Iinclude -c source/main.c -o bin/main.o
gcc -Iinclude -c source/move.c -o bin/move.o
gcc -Iinclude -c source/movegenerator.c -o bin/movegenerator.o
gcc -Iinclude -c source/ui.c -o bin/ui.o
gcc  bin/board.o  bin/main.o  bin/move.o  bin/movegenerator.o  bin/ui.o -o sakk.exe
```
