[README_conejo_bezier (1).md](https://github.com/user-attachments/files/35358991/README_conejo_bezier.1.md)
# Conejo en movimiento sobre una curva de Bézier - Visor 3D con OpenGL

Proyecto desarrollado en C++ y OpenGL 3.3 que permite visualizar un modelo 3D de un conejo, el cual se desplaza de un lado a otro siguiendo una trayectoria curva de Bézier dentro de un escenario tridimensional.

**Autor(es):** ALEX CAMILO VARELA  
**Curso:** COMPUTACION GRAFICA - Parcial  
**Video explicativo:** ENLACE_DE_YOUTUBE

## Características

- Carga de modelos 3D en formato **glTF** mediante la clase `Model`.
- Uso de **GLM** para realizar transformaciones tridimensionales, incluyendo traslación, rotación y escala.
- Implementación de una **curva de Bézier**, definida mediante cuatro puntos de control que determinan el recorrido del conejo.
- Movimiento automático del modelo a lo largo de la curva utilizando la función seno (`sin()`), permitiendo que el conejo se desplace de ida y vuelta.
- Representación visual de la trayectoria mediante una línea amarilla.
- **Iluminación básica** mediante una luz blanca ubicada en una posición fija del escenario.
- Aplicación de un color rojizo al modelo mediante la variable `objectTint`.
- Uso de una cámara 3D que permite visualizar el escenario desde diferentes posiciones.
- Renderizado del modelo y la trayectoria mediante shaders personalizados.

## Controles

La cámara recibe las entradas del usuario mediante la función `camera.Inputs(window)`.

Los controles específicos de movimiento y orientación dependen de la implementación de la clase `Camera`.

| Entrada | Acción |
|---|---|
| Teclado y mouse | Control de la cámara, según la configuración de `Camera` |
| Esc | Cerrar la ventana mediante el sistema de GLFW |

## Funcionamiento de la animación

El movimiento del conejo se realiza mediante una curva de Bézier cúbica que utiliza cuatro puntos de control:

- **P0:** Punto inicial de la trayectoria.
- **P1:** Primer punto de control, que determina la curvatura.
- **P2:** Segundo punto de control, que también modifica la curvatura.
- **P3:** Punto final de la trayectoria.

La función `bezierPoint()` calcula la posición del modelo en cada instante, utilizando el parámetro `t`, que toma valores entre 0 y 1.

La función `pathPoint()` establece las coordenadas de los cuatro puntos de control y devuelve la posición correspondiente sobre la curva.

Para generar el movimiento automático se utiliza:

```cpp
float t = 0.5f * (std::sin(static_cast<float>(glfwGetTime()) * 0.35f) + 1.0f);
```

Esta expresión permite que el valor de `t` cambie continuamente entre 0 y 1, haciendo que el conejo recorra la trayectoria en ambas direcciones.

## Arquitectura del software

Todo el programa principal se encuentra organizado en diferentes secciones:

| Módulo | Responsabilidad |
|---|---|
| **Shaders** | Utiliza `default.vert` y `default.frag` para representar el modelo 3D, y `path.vert` y `path.frag` para dibujar la trayectoria. |
| **Carga del modelo** (`Model.h`) | Permite cargar el modelo 3D del conejo desde un archivo glTF y preparar su representación en OpenGL. |
| **Curva de Bézier** (`bezierPoint`, `pathPoint`) | Calcula las posiciones de la trayectoria utilizando cuatro puntos de control. |
| **Vértices de la trayectoria** (`pathVertices`) | Genera 201 puntos de la curva para construir la línea que representa el recorrido. |
| **Iluminación** | Configura el color de la luz, su posición y el color aplicado al modelo. |
| **Cámara** (`Camera`) | Controla la visualización del escenario y permite interactuar con la perspectiva de la escena. |
| **Animación** | Actualiza continuamente la posición del conejo utilizando el tiempo transcurrido. |
| **Bucle principal** | Actualiza la cámara, limpia la pantalla, dibuja el modelo y representa la curva de Bézier en cada fotograma. |

### Flujo de cada cuadro

1. Limpiar el fondo y el búfer de profundidad.
2. Recibir las entradas del usuario para controlar la cámara.
3. Actualizar la matriz de visualización y proyección.
4. Calcular el parámetro `t` utilizando el tiempo de ejecución.
5. Obtener la nueva posición del conejo sobre la curva de Bézier.
6. Dibujar el modelo 3D en su nueva posición.
7. Dibujar la trayectoria de Bézier mediante una línea amarilla.
8. Actualizar la ventana y procesar los eventos de GLFW.

## Estructura del proyecto

```text
Proyecto OpenGL/
│
├── Main.cpp
│
├── Model.h
├── Camera.h
├── Shader.h
│
├── default.vert
├── default.frag
├── path.vert
├── path.frag
│
├── Resources/
│   └── YoutubeOpenGL 13 - Model Loading/
│       └── models/
│           └── bunny/
│               ├── scene.gltf
│               ├── scene.bin
│               └── texturas/
│
├── Librerías/
│   ├── GLFW
│   ├── GLAD
│   └── GLM
│
└── Archivos adicionales del proyecto
```

*Nota: La estructura representa los archivos principales y las dependencias utilizadas. Los archivos adicionales pueden variar dependiendo de la organización del proyecto.*

## Cómo compilar

1. Abrir el proyecto en **Visual Studio**.
2. Configurar el lenguaje de C++ en el estándar **C++17**, ya que se utiliza `std::filesystem`.
3. Verificar que las librerías GLFW, GLAD y GLM estén correctamente instaladas y vinculadas.
4. Comprobar que los archivos de shaders estén disponibles en el directorio de ejecución.
5. Verificar que el modelo `scene.gltf` y sus archivos asociados se encuentren dentro de la carpeta de recursos.
6. Compilar y ejecutar el programa.

## Librerías utilizadas

- **GLFW:** creación y administración de la ventana y los eventos del usuario.
- **GLAD:** carga de las funciones de OpenGL.
- **GLM:** operaciones matemáticas y transformaciones tridimensionales.
- **OpenGL 3.3:** renderizado de los objetos y la trayectoria.
- **C++17:** programación del proyecto y manejo de rutas mediante `std::filesystem`.
