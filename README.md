# Proyecto de C++ - La Caballera y la Mazmorra de Leugim (Mazmorra de niveles)

## Descripción del Proyecto

En este juego, el jugador controla a una caballera que debe rescatar a su príncipe, quien ha sido secuestrado por el villano Leugim donde este se fue a resguardar en su mazmorra. La aventura se desarrolla dentro de la mazmorra de Leugim donde tiene 10 pisos, que la protagonista deberá superar para llegar al enfrentamiento final.

**Tematica y ambienticación**
El juego se desarrolla en La Torre de Leugim, una estructura mágica vertical de 10 pisos que flota en los cielos sobre un reino maldito. Fue construida por un poderoso hechicero llamado Leugim, quien encerró al príncipe en la cima para desatar el caos en el mundo.

Cada piso de la torre tiene una ambientación única según el minijuego o evento que aparece:**Mecanica**
Cada sala tendrá tres puertas hacia diferentes salas, donde el usuario se encontrará con diferentes eventos los cuales deberá superar para avanzar, en dado caso no lo supere, perderá una vida y tendrá que empezar el reto de nuevo hasta superarlo, donde las opciones de los eventos será:
**Idea general de la jugabilidad**
La Torre de Leugim es un juego tipo Dungeon Crawler donde una heroína debe ascender 10 niveles de una torre mágica enfrentando enemigos, trampas, y superando 4 minijuegos distintos. A lo largo del camino, podrá encontrar eventos aleatorios con resultados positivos o negativos. El juego combina acción, lógica, estrategia y combate por turnos, culminando en una batalla final contra el jefe Leugim para rescatar al príncipe.

🧱 Estructura del Juego
📦 Niveles
9 niveles aleatorios

1 nivel final con jefe

Cada nivel puede contener:

Minijuego

Evento aleatorio

Combate con enemigo

🧑‍💻 Mecánicas Generales
Comienzas con 3 vidas

Fallar un reto (morir o perder) = -1 vida

Pierdes si llegas a 0 vidas

Puedes ganar vidas en ciertos eventos

Objetivo: llegar al nivel 10 y vencer al jefe

🎮 Minijuegos (4)
Explosivos / "Bomberman"

Rompe escombros para llegar a la salida

Uso limitado de bombas (te pueden dañar)

Mapas tipo laberinto

Laberinto de habitaciones

Encuentra llave y salida

Algunas habitaciones tienen monstruos o nuevas puertas

Shoot n' Run

Pasillo de acción con enemigos y obstáculos

Usa espada y arco

Jefe al final (si pierdes, caes un piso)

Mata al gusano

Gusano con 10 secciones

Combinaciones de teclas por sección

Fallo = perder vida, 3 vidas en total

⚙️ Eventos Aleatorios (6)
Cofre bueno

Puede contener vida extra

Cofre malo (Mimic)

Apariencia engañosa, si lo abres pierdes vida

Elevador lógico

Puzzle o acertijo para avanzar

Esfinge

3 preguntas, si fallas una, mueres

Genio bueno

Elige 1 de 3 deseos aleatorios (ej. +3 vidas, saltar pisos)

Genio malvado

Mismas opciones buenas, pero invertidas tras elegir
**Temas vistos**
1. Arreglos: 
Para almacenar los niveles generados aleatoriamente.
Para manejar el inventario del jugador (vidas, bombas, llaves).
Para guardar preguntas y respuestas de eventos como la esfinge.
2. Condicionales (if, else, else if):
Para determinar qué tipo de nivel se genera en cada piso.
Para controlar qué ocurre cuando el jugador gana o pierde un mini-juego.
Para verificar si el jugador tiene los elementos necesarios para avanzar (ej. llave, vidas).
3. Ciclos for:
Para recorrer las habitaciones en los laberintos.
Para mostrar los niveles, los enemigos o las opciones del jugador.
Para validar múltiples respuestas de la esfinge o verificar impactos a enemigos.
4. Funciones:
Para modularizar cada mini-juego.
Para manejar eventos (genio, cofres, esfinge).
Para calcular vidas, reiniciar niveles, y mostrar mensajes.
5. Instrucción switch:
Para ejecutar acciones según el tipo de evento o mini-juego seleccionado aleatoriamente.
Para decidir los efectos del deseo que ofrece el genio.
6. Ciclo while:
Para repetir un nivel mientras no se haya superado y queden vidas.
Para repetir hasta que el jugador encuentre la llave en un laberinto.
Para controlar bucles de combate o intentos de respuestas en eventos.
7. Matrices (arreglos bidimensionales):
Para representar laberintos o mapas en mini-juegos como “Encuentra el camino con explosivos”.
Para almacenar la estructura de las habitaciones en el minijuego del laberinto.
8. Archivos
Guardado de proceso.

**Consideraciones técnicas del desarrollo**
**Imagenes**



## Equipo

- **Nombre del equipo:** Ctrl+Fe

### Integrantes del equipo

1. **Nombre completo:** Josué Emiliano Valdés  
   **Carnet:** 00003525

2. **Nombre completo:** Anderson José Turcios Flores  
   **Carnet:** 00003925

3. **Nombre completo:** Adriana Paola Mejía Méndez  
   **Carnet:** 00098225
   
4. **Nombre completo:** Karla Reneé Rivas Hernández  
   **Carnet:** 00107225
   
## Instrucciones de Ejecución

1. Clona este repositorio en tu máquina local:
   ```bash
   git clone [URL del repositorio]
