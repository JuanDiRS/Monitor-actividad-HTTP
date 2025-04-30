# Monitor-actividad-HTTP
Un monitor que muestra la actividad HTTP de un equipo

Este programa está diseñado para capturar y analizar los paquetes de red que circulan en el dispositivo donde se ejecuta. Su funcionalidad principal es filtrar y mostrar únicamente aquellos paquetes que utilizan el protocolo HTTP, permitiendo observar el tráfico HTTP durante un período específico de tiempo.

## Funcionalidades
- Captura en tiempo real de trafico de red.
- Filtrado especifico de datos HTTP.
- Muestra la cantidad de peticiones agrupadas por URL.

## Como esta contruido
- **WSL:** Este programa fue desarrollado y ejecutado en WSL (Windows Subsystem for Linux).

- **C++:** El programa se construye en el lenguaje C++ el cual se puede consultar en el archivo "ProyectoLu.cpp", donde se ve el codigo fuente del programa.

## Requisitos
- **libpcap**: Libreria para la captura de paquetes en sistemas Unix/Linux. PcapPlusPlus depende de libpcap para acceder a la interfaz de red y realizar la captura.

- **CMake:** Herramienta utilizada para la compilación y gestión del proyecto.
Puedes descargarla desde: https://cmake.org/ CMake desde: https://cmake.org/

- **PcapPlusPlus:** Esta libreria es la que hace posible la captura y analisis de paquetes de red de forma eficiente. Simplifica el desarrollo de aplicaciones para el análisis de tráfico, gracias a su enfoque orientado a objetos y su soporte para múltiples protocolos de red.
Puedes descargar PcapPlusPlus desde: https://pcapplusplus.github.io/

## Instalacion en Linux
1. Asegúrate de tener instaladas las dependencias mencionadas anteriormente.
2. Clona el repositorio con el siguiente comando:
git clone https://github.com/JuanDiRS/Monitor-actividad-HTTP.git.
4. Accede a la carpeta del repositorio clonado y dirígete a la subcarpeta build, donde encontrarás el ejecutable del proyecto llamado ProyectoLu.
5. Otorga permisos de ejecucion al programa con el comando:  chmod +x ProyectoLu
6. Ejecuta el programa con: "./ProyectoLu <Tiempo>". Reemplaza <Tiempo> por la cantidad de segundos que deseas que se ejecute el programa.

## Limitaciones
- El programa solo es capaz de hacer una consulta a la vez.
- No mide el tiempo entre consultas.
- Si una pagina falla el programa la salta.
- Solo muestra actividad HTTP.
- Si una pagina se demora en responder el programa no tomara mas.
- Toma toda la actividad que hay en la red del dispositivo.

