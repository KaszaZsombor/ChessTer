# Főoldal

## Sakk program - 2026/27 őszi félév

Ez a projekt a programozás alapjai 1 tantárgy nagy házifeladatra készült. A projekt egy sakkjáték. A játék a stdin-ről lépéseket olvas be, lefordítja, majd a szabályosság ellenőrzése után a stdout -ra lerajzolja a lépés utáni sakktábla állását. 

### Mit tud:

- A program figyel az éppen következő játékosra, kétszer ugyanaz a szín nem léphet.
- A program figyel a sáncolási szabályokra és a sakkokra. Lépés után nem maradhat sakkban a lépő szín királya.
- A szabályos lépések betartatásán kívül az átalakulás során meg lehet adni, milyen bábuvá alakuljon át a gyalog. Amennyiben nem volt ilyen megadva, automatikusan vezérré alakul.
- A játszmát egyszerűen meg lehet szakítani a stdin EOF karakterrel. Windowson ez Crtl + z.

### Telepítés és használat:

- A telepítése lehetséges a sakk.exe fájl letöltésével és futtatásával.
- A projekt letöltésével source-ból is építhető a projekt, a github oldalról letöltve.
- A futtatás után az alábbi formátumban lehet a lépéseket megadni: <file><row><file><row><(optional) promotion piece>

### Projekt elérhetősége:

- https://github.com/KaszaZsombor/SAKK


### A dokumentáció:

- https://docs.kaszazsombor.hu/