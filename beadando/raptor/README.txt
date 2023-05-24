A FELADAT

Egy üldözős játék, egy ellenfél van a pályán, amely a játékos felé halad, s ha elkapja, vége a játéknak. A játék alatt érméket lehet gyűjteni pontokért, és tereptárgyakat kell kikerülni, illetve a tereptárgyakkal lehet lassíani az ellenfelet.



IRÁNYÍTÁS

-W, A, S, D billentyűkkel lehet mozogni.
-A kamerát egérgomb nyomvatartása közben az egér mozgatásával lehet forgatni, illetve nyíl gombokkal.
-Space billentyűvel lehet ugrani, az alacsonyabb tereptárgyak átugorhatóak.
-A játékot megállítani az F1 billentyűvel lehet, ezalatt a játék leírása látható.
-A játékot újrakezdeni F5 billentyűvel lehet.
-A ködöt kikapcsolni (csökkenteni)/bekapcsolni az F10 billentyűvel lehet.
-Teljes képernyőre lehet váltani F11 billentyűvel.
-A hangot az F12 billentyűvel lehet ki/bekapcsolni.
-A + és - gombok a fényerőt állíják.



FIZIKA/LOGIKA:

-A kamera fel-le irányban nem forgatható át, nem lehet fejjel lefelé nézni.
-Van ugrás, az alacsonyabb tereptárgyak átugorhatóak.
-Az ellenfél egyenes pályán követi a játékost, valamivel lassabban nála.
-A tereptárgyak gömb-collider-t használnak, az ellenfél és a játékos is "lecsúszik róluk".
-A gömb-collider pozíciója eltolható a modelhez képest, így van megoldva, hogy a magasabb tárgyakat ne lehessen átugrani.
-A radius a fizikális ütközésekért felelős (tereptárgyak), a range az entity-k ütközésérét (kamera-ellenfél; kamera-coin).
-Az érmék random pozíciókban inicializálódnak a pálya méretein belül, mikor felszedjük, új pozíciót keresnek. Ha az érme tereptárgy collider-ébe kerülne, új pozíciót keres, így mindig felszedhető marad.
-A pályát fájlból olvassa be a program, a fájlban kommentben leíás van a használatáról.



FEATURE-ÖK

-A camera egy fényforrás. Eredetileg egy felvehető lámpát terveztem a pályára, erre azonban nem jutott idő.
-2 fényforrás van, a GL_LIGHT0 a játékos, a GL_LIGHT1 egy "globális" fény, melyet +/- billentyűkkel változtathatunk.
-Köd effekt: dinamikusan változik - szinuszosan egy intervallumon belül nagyon lassan.
-Köd effekt: a beépítetten kívül füstöt ábrázoló képfájl mozog a képernyőn (kamerától függetlenül), mely szinuszosan jobbra-balra mozog, illetve szintén szinusosan változik az átlátszósága.
-Az ellenfél "AI"-a igen egyszerű, egyenes vonalban üldözi a játékost, a tereptárgyak kikerülését a gömb-collider-ek teszik. (Emiatt nem használ a program tégla alakú hitbox-okat, az ellenfélnek bonyolultabb AI-ra lenne szüksége, hogy ne akadjon el.)
-Collider: radius és range használata
-"Fizikaként" ugrás és ütközés funkciók
-A pálya fájlban szerkeszthető
-A pálya mérete megadható
-Az érmék száma megadható
-Hanglejátszás (POSIX-os párhuzamosítással)
-Az érmék animálva vannak (forgás, szinuszos értékkel fel-le mozgás)
-Az ablak átmérezhető, akár Full Screne-re állítható
-Játéklogika: szerezhető pontok, Game Over, új játék, Pause
-Pause alatt megjelenik egy rövid játékleírás
-2D-s kirajzolása a pontokszámnak, átlátszó háttérrel



A PROGRAM

A program belépési pontja a main.c fájlban található.

Használt osztályok (.c és .h):
-App: az aplikáció, SDL, OpenGL, window, illetve a játékhoz szükséges Scene és Camera "objektumokat" tartalmazza. Az App felelős a program futásáért, a játék és felhasználói események vezérléséért, valamint a megjelenítésért.
-Camera: a 3D-s tér megjelenítési "helyéért" felelős, az App eseményein keresztül mozgatható, illetve a játék szempontjából a scene-ben entitásként viselkedik, ő a játékos.
-Scene: A pálya elemeit tárolja (pálya, tereptárgyak, érmék, ellenfél), valamint a játéklogika nagyrésze itt van. Itt találhatóak még a pálya fájlból történő beolvasásáért felelős függvények, valamint a 3D-s pályától független, 2D-s render-elések (pl. pontszám).
-ModelWrapper: a pályán lévő objektumokat jelenti, egy Model objektumot "egészít ki" a játékhoz szükséges információkkal (pozíció, irány, méret, stb.)
-Utils: a játék megjelenítéséhez, logikájához használatos "segédfüggvények" tárhelye, a vec3, Color, és Material típus, és a vec3 típus műveleteit, illetve random szám generálását tárolja.
-Texture: a textúra betöltéséhez használt függvényeket tárolja.
-Sound: a játék hangjainak elérési útját, illetve a hanglejátszás függvényét tartalmazza.



FÁJLOK/HASZNÁLAT

A program a Makefile-lal fordítható, szükségesek hozzá a következő könyvtárak:
-stdio.h
-stdlib.h
-stdbool.h
-string.h
-math.h
-time.h
-sys/types.h
-pthread.h
-windows.h
-SDL2/SDL.h
-SDL2/SDL_image.h
-GL/gl.h
-obj/load.h
-obj/draw.h
-obj/model.h

A dll és lib fájlokat az exe fájl mellé kell helyezni fordítás után, valamint a models.csv-t, amely a pálya információit/elemeit tartalmazza. A cpb fájl a CodeBlocks-os szerkesztéshez van. Az exe mellett szüksége van az assets mappára, melyben a következő mappák találhatóak:
-models: az obj fájlokat tartalmazza
-score_img: a pontok megjelenítéséhez használt képeket tartalmazza
-sounds: a hangfájlokat tartalmazza
-textures: a pontszámok textúráin kívüli textúrákat tartalmazza

A pályát a models.csv fájlban szerkeszthetjük.



