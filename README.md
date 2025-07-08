<h1 align="center">Proyecto de C++ - La Caballera y la Mazmorra de Leugim (Mazmorra de niveles)</h1>
<br>
![logo](https://github.com/FDP-01-2025/project-ctrl-fe/blob/be1bf8a6bf43f5fde54722cd4aecb38f63bd46e6/assets/image/logo.png)

<h3 align="center">🎮 Descripción del Proyecto</h3>

#### _🏦 Contexto hitstórico_

En un universo hiperincreíble y fantástico de fantasía se desarrolla la historia que dará vida al título: "La Caballera y la Mazmorra de Leugim".

Un día como cualquier otro, en la villa Kirby, se encuentra nuestra protagonista, la caballera Arlak, quien se encontraba pasando el rato con su novio, el príncipe de la villa, Onailiem. De repente y sin avisar, el dueño de una temible mazmorra cercana a la villa, celoso de su amor, decide capturar al guapo príncipe y llevarlo a sus aposentos... Arlak, furiosa y roja de rabia, decide ir y ayudar al malvado Leugim a acercarse a Dios para recuperar a su amado y vivir felices para siempre.

#### _🙍‍♀️ Papel del jugador_

El jugador controla a la caballera, que está en su camino para rescatar a su príncipe, quien ha sido secuestrado por el villano Leugim, resguardado en su mazmorra. La aventura se desarrolla dentro de la mazmorra de distintos pisos, que la protagonista deberá superar para llegar hasta Leugim, enfrentarlo en un enfrentamiento final y recuperar a su pushamon de sus malvadas garras.

- **🖼️ Tematica y ambienticación:**
  El juego se desarrolla en La Torre de Leugim, una estructura mágica vertical de distintos pisos que flota en los cielos sobre una villa. Fue construida por un poderoso hechicero llamado Leugim, quien encerró al príncipe en la cima por SIMPatico y quería compañia Leugim.

> Cada piso de la torre tiene una ambientación única según el minijuego o evento que aparece:

- 👾 **Idea general de la jugabilidad:**
  La Torre de Leugim es un juego tipo Dungeon Crawler donde una heroína debe ascender 10 niveles de una torre mágica enfrentando enemigos, trampas, y superando 4 minijuegos distintos. A lo largo del camino, podrá encontrar eventos aleatorios con resultados positivos o negativos. El juego combina acción, lógica, estrategia y combate por turnos, culminando en una batalla final contra el jefe Leugim para rescatar al príncipe.

- 🔩 **Mecanica:**
  Cada sala tendrá tres puertas hacia diferentes salas, donde el usuario se encontrará con diferentes eventos los cuales deberá superar para avanzar, en dado caso no lo supere, perderá una vida y tendrá que empezar el reto de nuevo hasta superarlo, la cantidad de retos que debera enfrentar va a depender de la dificultad que elija, por ejemplo en la facil será 3 eventos/retos con la batalla contra el jefe, normal con 5 y dificil con 6, de ahi podra tener dos posibles batallas finales contra el jefe, pero esto sera aleatorio, de ahí donde las opciones de los eventos/jefes será:

---

| Categoría                 | Subcategoría              | Detalles                                                                                                           |
| ------------------------- | ------------------------- | ------------------------------------------------------------------------------------------------------------------ |
| 🧱 Estructura del Juego   | 📦 Niveles                | 9 niveles (incluyendo jefes) aleatorios                                                                            |
|                           | Contenido por nivel       | - Minijuego<br>- Evento aleatorio<br>- Combate con Jefe                                                            |
| 🧑‍💻 Mecánicas Generales    | Vidas                     | Comienzas con 3 vidas                                                                                              |
|                           |                           | Fallar un reto (morir o perder) = -1 vida                                                                          |
|                           |                           | Pierdes si llegas a 0 vidas                                                                                        |
|                           |                           | Puedes ganar vidas en ciertos eventos                                                                              |
|                           | Objetivo                  | Completar x niveles dependiendo de la dificultad y vencer al jefe                                                  |
| 🎮 Minijuegos (4)         | Explosivos / "Bomberman"  | - Rompe escombros para llegar a la salida<br>- Uso limitado de bombas (te pueden dañar)<br>- Mapas tipo laberinto  |
|                           | Laberinto de habitaciones | - Encuentra llave y salida<br>- Algunas habitaciones tienen monstruos o nuevas puertas                             |
|                           | Shoot n' Run              | - Pasillo de acción con enemigos y obstáculos<br>- Usa espada y arco<br>- Jefe al final (si pierdes, caes un piso) |
|                           | Mata al gusano            | - Gusano con 10 secciones<br>- Combinaciones de teclas por sección<br>- Fallo = perder vida, 3 vidas en total      |
| ⚙️ Eventos Aleatorios (6) | Cofre bueno               | Puede contener vida extra                                                                                          |
|                           | Cofre malo (Mimic)        | Apariencia engañosa, si lo abres pierdes vida                                                                      |
|                           | Elevador lógico           | Puzzle o acertijo para avanzar                                                                                     |
|                           | Genio bueno               | Elige 1 de 3 deseos aleatorios (ej. +3 vidas, saltar pisos)                                                        |
|                           | Genio malvado             | Mismas opciones buenas, pero invertidas tras elegir                                                                |
| 💀 Jefes                  | Jefe tipo Mario Bross     | Se tiene que saltar para esquivar al jefe, y dispararle                                                            |
|                           | Jefe tipo Zelda           | Se tiene que esquivar los ataques de la estatua y posicionarse para ponerle una bomba para hacerle daño            |
||

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

- El proyecto fue desarrollado en C++, con el compilador de g++.exe (MinGW.org GCC-6.3.0-1) 6.3.0.

- Window 11, debido a caracteres especiales del unicode pertenecientes.

- Peso del ejecutable 550 KB.

- Uso exclusivo de gráficos ASCII con UNICODE.

- Modularización completa mediante archivos .h y un único .cpp principal.

- El juego incluye música de fondo mediante integración de sonido de consola y de archivos .wav.

- El juego se compila como aplicación de escritorio (-mwindows) para evitar mostrar consola negra adicional.

- El juego está organizado usando programación orientada a objetos (OOP), dividiendo funcionalidades en clases específicas, lo cual permite modularidad, reutilización de código y una mejor organización, donde facilita el mantenimiento, se puede añadir más contenido después de forma más fácil y cada clase tiene una responsabilidad clara, lo que mejora la legibilidad del proyecto.

## **Imagenes**

https://www.figma.com/design/n5lZId7IIKfvx0tRVgtP1N/Cntr-Fe?node-id=0-1&t=tDheFUCTkofCvffx-1

## **Diagrama de flujo**

https://drive.google.com/drive/folders/1ICve8q_SMPjpHk987wgam8xRdul9jr3r?usp=sharing

## Equipo

```bash
Ctrl+Fe
```

### Integrantes del equipo

1. **Nombre completo:** Josué Emiliano Valdés  
   **Carnet:** 00003525

2. **Nombre completo:** Anderson José Turcios Flores  
   **Carnet:** 00003925

3. **Nombre completo:** Adriana Paola Mejía Méndez  
   **Carnet:** 00098225
4. **Nombre completo:** Karla Reneé Rivas Hernández  
   **Carnet:** 00107225

## 📦 Compilación y Ejecución del Proyecto

### 🧩 Instrucciones de Ejecución

1. Clona este repositorio en tu máquina local:
   ```bash
   git clone https://github.com/FDP-01-2025/project-ctrl-fe.git
   ```
2. Entra a la carpeta del proyecto y compílalo con el siguiente comando:
   ```bash
   g++ main.cpp -o dungeonLeugim -I./src -mwindows -lwinmm
   ```
3. Una vez compilado, **ejecuta el archivo `main.exe` desde el explorador de archivos de Windows** (doble clic), no desde la terminal, para evitar la apertura de una consola negra adicional.

---

### ❓ ¿Por qué se utiliza la opción `-mwindows`?

La bandera `-mwindows` le indica al compilador que el programa se ejecutará como una **aplicación de escritorio (GUI)** y no como una **aplicación de consola**. Esto evita que se abra una ventana de terminal negra al ejecutar el `.exe`, lo cual puede ser útil para mejorar la presentación o evitar conflictos visuales, especialmente si se desea una experiencia más limpia para el usuario final.

---

### ❓ ¿Por qué se utiliza la opción `-I`?

La opción `-I` (mayúscula i) le indica al compilador la **ruta de las carpetas donde debe buscar archivos `.h` o archivos de cabecera personalizados**. Es esencial cuando el código fuente está dividido en múltiples carpetas, como `src`, `core`, `utils`, etc.

> Por ejemplo, en el comando:
>
> ```bash
> g++ main.cpp -o dungeonLeugim -I./src
> ```
>
> el compilador buscará dentro de la carpeta `src` cualquier archivo que esté siendo incluido con `#include`.

Esto evita errores como `fatal error: archivo.h: No such file or directory`, y permite una estructura modular del proyecto.

---

### ❓ ¿Por qué se utiliza la opción `-lwinmm`?

La opción `-lwinmm` le indica al compilador que debe **enlazar la biblioteca `winmm.lib` de Windows**, la cual permite usar funciones relacionadas con el **control de audio y multimedia**, como reproducir música o sonidos con `PlaySound()`.

> Esto es necesario porque el juego incluye **música de fondo o efectos de sonido**, y para que esas funciones funcionen correctamente al compilar con `g++`, se debe enlazar manualmente esa librería del sistema operativo.

> Es parte de la API de Windows llamada **Windows Multimedia API**, utilizada para reproducir archivos `.wav` dentro del juego.

---

## ✅ Reglas para realizar commits

Para mantener un historial de cambios claro, profesional y consistente, se deben seguir las siguientes reglas al momento de hacer commits:

1. **Evita commits con una sola palabra.**
2. **Sigue este formato obligatorio:**

   ```
   [Emoji] [Verbo en infinitivo] [Descripción del cambio]
   ```

   Ejemplos válidos:

   - ✨ Agregar minijuego tipo Bomberman
   - 🐛 Corregir bug en lógica de explosión

3. **Solo se permite un (1) emoji por mensaje de commit.**
4. **No se permite el uso de palabras ofensivas o vulgares**, ni en español ni en inglés.
5. **Utiliza siempre verbos en infinitivo** para estandarizar la forma del mensaje.

---

## 📚 Nomenclatura y Convenciones de Código

| Elemento      | Formato    | Ejemplo              | Notas                                           |
| ------------- | ---------- | -------------------- | ----------------------------------------------- |
| **Carpetas**  | kebab-case | `nombre-ejemplo`     | Usar guiones para separar palabras              |
| **Archivos**  | camelCase  | `nombreEjemplo.cpp`  | Iniciar con minúscula, sin espacios             |
| **Variables** | camelCase  | `int nombreVariable` | Variables y atributos en minúscula inicial      |
| **Funciones** | PascalCase | `NombreFuncion()`    | Cada palabra inicia con mayúscula, sin espacios |

---

g++ .\main.cpp -I./src -o TorreDeLeugim.exe -mwindows

## Logica de carpetas

- **Assets**: Carpeta con todos los recursos del juego que no son código, como imágenes, sonidos, mapas y textos cargados en tiempo de ejecución.

- **Build**: Archivos generados automáticamente durante la compilación, como ejecutables y archivos temporales. No contiene código fuente ni recursos originales.

- **src**: Carpeta que contiene todo el código fuente escrito para el juego.

- **Data**: Información estructurada o de soporte que el juego usa pero que no es código ni recursos multimedia.

- **Sounds**: Archivos de audio utilizados en el juego.

- **Maps**: Archivos de texto que representan los mapas del juego. (Evitar nombrar subcarpetas como "map")

- **Core**: Código que no es visible para el jugador pero que hace funcionar el juego internamente.

- **Engine**: Configuraciones generales, sistemas base de renderizado y audio. Controla la apariencia y comportamiento general del juego.

- **Interface**: Código para mostrar la interfaz gráfica principal del juego.

- **Utils**: Código reutilizable en distintas partes del juego.

- **Startup**: Archivos necesarios para el inicio del juego, como logo, pantalla de carga, intro y configuración inicial.

- **Modules**: Módulos independientes con funcionalidades específicas para cada minijuego o evento.

"En programación, un módulo es una unidad o bloque independiente de código que agrupa funciones, procedimientos, clases, variables y datos relacionados para cumplir con una tarea específica o un conjunto de funcionalidades dentro de un programa."
