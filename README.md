<h1 align="center">Proyecto de C++ - La Caballera y la Mazmorra de Leugim (Mazmorra de niveles)</h1>
<br><br>

<h2 align="center">🎮 Descripción del Proyecto</h6>

### **_🏦 Contexto historico_**

En un universo hiperincreíble y fantástico de fantasía se desarrolla la historia que dará vida al título: "La Caballera y la Mazmorra de Leugim".

Un día como cualquier otro, en la villa Kirby, se encuentra nuestra protagonista, la caballera Arlak, quien se encontraba pasando el rato con su novio, el príncipe de la villa, Onailiem. De repente y sin avisar, el dueño de una temible mazmorra cercana a la villa, celoso de su amor, decide capturar al guapo príncipe y llevarlo a sus aposentos... Arlak, furiosa y roja de rabia, decide ir y ayudar al malvado Leugim a acercarse a Dios para recuperar a su amado y vivir felices para siempre.

### **_🙍‍♀️ Papel del jugador_**

El jugador controla a la caballera, que está en su camino para rescatar a su príncipe, quien ha sido secuestrado por el villano Leugim, resguardado en su mazmorra. La aventura se desarrolla dentro de la mazmorra de 10 pisos, que la protagonista deberá superar para llegar hasta Leugim, enfrentarlo en un enfrentamiento final y recuperar a su pushamon de sus malvadas garras.

- **🖼️ Tematica y ambienticación:**
  El juego se desarrolla en La Torre de Leugim, una estructura mágica vertical de 10 pisos que flota en los cielos sobre una villa. Fue construida por un poderoso hechicero llamado Leugim, quien encerró al príncipe en la cima por Simp.

> Cada piso de la torre tiene una ambientación única según el minijuego o evento que aparece:

- 🔩 **Mecanica:**
  Cada sala tendrá tres puertas hacia diferentes salas, donde el usuario se encontrará con diferentes eventos los cuales deberá superar para avanzar, en dado caso no lo supere, perderá una vida y tendrá que empezar el reto de nuevo hasta superarlo, donde las opciones de los eventos será:

- 👾 **Idea general de la jugabilidad:**
  La Torre de Leugim es un juego tipo Dungeon Crawler donde una heroína debe ascender 10 niveles de una torre mágica enfrentando enemigos, trampas, y superando 4 minijuegos distintos. A lo largo del camino, podrá encontrar eventos aleatorios con resultados positivos o negativos. El juego combina acción, lógica, estrategia y combate por turnos, culminando en una batalla final contra el jefe Leugim para rescatar al príncipe.

---

| Categoría                 | Subcategoría              | Detalles                                                                                                           |
| ------------------------- | ------------------------- | ------------------------------------------------------------------------------------------------------------------ |
| 🧱 Estructura del Juego   | 📦 Niveles                | 9 niveles aleatorios                                                                                               |
|                           |                           | 1 nivel final con jefe                                                                                             |
|                           | Contenido por nivel       | - Minijuego<br>- Evento aleatorio<br>- Combate con enemigo                                                         |
| 🧑‍💻 Mecánicas Generales    | Vidas                     | Comienzas con 3 vidas                                                                                              |
|                           |                           | Fallar un reto (morir o perder) = -1 vida                                                                          |
|                           |                           | Pierdes si llegas a 0 vidas                                                                                        |
|                           |                           | Puedes ganar vidas en ciertos eventos                                                                              |
|                           | Objetivo                  | Llegar al nivel 10 y vencer al jefe                                                                                |
| 🎮 Minijuegos (4)         | Explosivos / "Bomberman"  | - Rompe escombros para llegar a la salida<br>- Uso limitado de bombas (te pueden dañar)<br>- Mapas tipo laberinto  |
|                           | Laberinto de habitaciones | - Encuentra llave y salida<br>- Algunas habitaciones tienen monstruos o nuevas puertas                             |
|                           | Shoot n' Run              | - Pasillo de acción con enemigos y obstáculos<br>- Usa espada y arco<br>- Jefe al final (si pierdes, caes un piso) |
|                           | Mata al gusano            | - Gusano con 10 secciones<br>- Combinaciones de teclas por sección<br>- Fallo = perder vida, 3 vidas en total      |
| ⚙️ Eventos Aleatorios (6) | Cofre bueno               | Puede contener vida extra                                                                                          |
|                           | Cofre malo (Mimic)        | Apariencia engañosa, si lo abres pierdes vida                                                                      |
|                           | Elevador lógico           | Puzzle o acertijo para avanzar                                                                                     |
|                           | Esfinge                   | 3 preguntas, si fallas una, mueres                                                                                 |
|                           | Genio bueno               | Elige 1 de 3 deseos aleatorios (ej. +3 vidas, saltar pisos)                                                        |
|                           | Genio malvado             | Mismas opciones buenas, pero invertidas tras elegir                                                                |

## **Temas vistos**

1. **Arreglos**:
   Para almacenar los niveles generados aleatoriamente.
   Para manejar el inventario del jugador (vidas, bombas, llaves).
   Para guardar preguntas y respuestas de eventos como la esfinge.

2. **Condicionales (if, else, else if)**:
   Para determinar qué tipo de nivel se genera en cada piso.
   Para controlar qué ocurre cuando el jugador gana o pierde un mini-juego.
   Para verificar si el jugador tiene los elementos necesarios para avanzar (ej. llave, vidas).

3. **Ciclos for**:
   Para recorrer las habitaciones en los laberintos.
   Para mostrar los niveles, los enemigos o las opciones del jugador.
   Para validar múltiples respuestas de la esfinge o verificar impactos a enemigos.

4. **Funciones**:
   Para modularizar cada mini-juego.
   Para manejar eventos (genio, cofres, esfinge).
   Para calcular vidas, reiniciar niveles, y mostrar mensajes.

5. **Instrucción switch**:
   Para ejecutar acciones según el tipo de evento o mini-juego seleccionado aleatoriamente.
   Para decidir los efectos del deseo que ofrece el genio.

6. **Ciclo while**:
   Para repetir un nivel mientras no se haya superado y queden vidas.
   Para repetir hasta que el jugador encuentre la llave en un laberinto.
   Para controlar bucles de combate o intentos de respuestas en eventos.

7. **Matrices (arreglos bidimensionales)**:
   Para representar laberintos o mapas en mini-juegos como “Encuentra el camino con explosivos”.
   Para almacenar la estructura de las habitaciones en el minijuego del laberinto.

8. **Archivos**
   Guardado de proceso.

## **Consideraciones técnicas del desarrollo**

**Imagenes**

## Equipo

#### Ctrl+Fe

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
   ```
