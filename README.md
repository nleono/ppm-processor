# ppm-processor
Trabajo final del Seminario de Lenguajes C

Se implementa la funcionalidad faltante del repositorio https://github.com/chrodriguez/ppm-ejercicio-c.git
donde se puede ver la consigna completa del trabajo.

El programa es un gestor de imagenes PPM de tipo P6, que recibe un archivo ppm de entrada y genera uno nuevo de salida
modificando el original según las opciones ingresadas:

OPCIONES:

      -i <INPUT PPM FILE>        archivo origen. OPCION REQUERIDA

      -o <OUTPUT PPM FILE>       archivo salida. OPCION REQUERIDA

      -r                         rota la imagen 90º hacia la derecha 

      -n                         genera el negativo de la imagen

      -h                         espeja horizontalmente la imagen

      -v                         espeja verticalmente la imagen

      -b <NUM>                   genera desenfoque con radio NUM

      -?                         muestra esta ayuda y finaliza

Para compilar el código:

    gcc -Wall -o ppm-processor main.c ppm.c ppm-operations.c


