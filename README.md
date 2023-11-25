# Minecraft-RayTracing

![image](https://github.com/adrianRFlores/Minecraft-RayTracing/assets/84111818/ce954c56-3594-4d01-8f22-b13f541d2d7e)

## Características

### Rotación del diorama y acercamiento/alejamiento de la cámara

<img src="https://github.com/adrianRFlores/Minecraft-RayTracing/assets/84111818/63947490-4f43-4181-984d-213312cba185" width="600">

<img src="https://github.com/adrianRFlores/Minecraft-RayTracing/assets/84111818/66cf72bc-f858-46f0-9e8a-ad0c5e64fce1" width="600">

### Refracción y reflexión de rayos de luz en los materiales

![Vídeo sin título ‐ Hecho con Clipchamp (5)](https://github.com/adrianRFlores/Minecraft-RayTracing/assets/84111818/ed42a112-b71e-4733-8467-1cd310815ca9)

### Colores de luz

<img src="https://github.com/adrianRFlores/Minecraft-RayTracing/assets/84111818/c5b5808b-e3e4-4d35-95ac-2afc8f6afe08" width="600">

## Materiales

Hay un total de 16 materiales distintos, de los cuales se utilizan 15 en la escena principal. Cada material cuenta con sus propias características físicas y 2 texturas: una para los lados y la parte inferior, y la otra exclusivamente para la cara superior del cubo.

Las características físicas de los materiales incluyen:

- Albedo
- Albedo especular
- Coeficiente especular
- Reflectividad
- Transparencia
- Índice de refracción
  
Materiales como el agua y la lámpara están configurados con un índice de refracción realista, correspondiente a sus contrapartes en la vida real.

![adobe](https://github.com/adrianRFlores/Minecraft-RayTracing/assets/84111818/c363ac3f-7706-48d3-8194-9f709c706f67)

## Compilar
Utilizar MinGW con ``mingw32-make.exe`` dentro del directorio del Makefile. SDL, SDL_image, SDL_Mixer y GLM vienen empacados dentro de la carpeta src.
``mingw32-pthreads-w32`` debe estar instalado en MinGW para poder utilizar threading.
