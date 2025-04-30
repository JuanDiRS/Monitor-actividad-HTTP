# Monitor-actividad-HTTP
Un monitor que muestra la actividad HTTP de un equipo

Este programa permite capturar y analizar el tráfico de red HTTP en un dispositivo en tiempo real, filtrando únicamente las solicitudes HTTP y mostrando estadísticas sobre las peticiones agrupadas por URL durante un período específico de tiempo. Está diseñado para funcionar en sistemas Linux y WSL (Windows Subsystem for Linux).

## Funcionalidades
- Captura de tráfico de red en tiempo real.
- Filtrado especifico de datos HTTP.
- Agrupación y conteo de solicitudes HTTP por URL.
- Estadísticas sobre el número total de peticiones HTTP realizadas durante la ejecución.

## Como esta contruido
- **WSL(Windows Subsystem for Linux):** Este programa fue desarrollado y ejecutado en WSL .

- **C++:** El programa se construye en el lenguaje C++ el cual se puede consultar en el archivo "ProyectoLu.cpp", donde se ve el codigo fuente del programa.

- **libpcap y PcapPlusPlus:** Estas bibliotecas permiten la captura y el análisis de paquetes de red. PcapPlusPlus proporciona una interfaz más amigable para trabajar con los datos capturados.

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

