# <img src="https://github.com/adriagomeez/TFG-GPSdeMontana/blob/main/imgs/icono.png" width="48"> TFG - GPS de Montana 

La popularidad y la utilización de los GPS ha crecido radicalmente en los últimos años en muchos ámbitos de la
vida humana. Uno de ellos es la realización de rutas por la montaña para poder llevar a cabo actividades como rutas en
bicicleta, salir a correr o caminatas por la montaña. Para satisfacer esa necesidad, han surgido varias aplicaciones que
permiten el manejo de estas rutas pudiendo visualizarlas y compartirlas para poder realizar las actividades deseadas. En este
proyecto, se desarrolla una aplicación de escritorio multiplataforma capaz de unificar varias rutas cargadas por el usuario en un
único grafo sin nodos ni aristas repetidas ni redundantes, para posteriormente, con un algoritmo de búsqueda, poder generar
rutas entre dos puntos seleccionados, con restricciones de distancia y desnivel acumulado y con la posibilidad de añadir
paradas intermedias en ella, con la mejor relación entre velocidad de respuesta y precisión de los resultados posible. Además,
es posible guardar esta ruta para poder introducirla en GPSs de montaña y poder realizarla sin problema, teniendo así
un generador de rutas el cual ofrece una infinidad de ellas gracias a todos sus parámetros seleccionables.

![alt text](https://github.com/adriagomeez/TFG-GPSdeMontana/blob/main/imgs/Captura3.PNG)

## Funcionalidades
La aplicación permite al usuario cargar una imagen en formato GeoTiff que será mostrada por pantalla y sobre la cual se puede desplazar con total libertad aplicando zum y arrastrando sobre ella. También permite cargar rutas en formato GPX las cuales son leidas y unificadas en un grafo que se muestra por pantalla. Este grafo puede ser guardado en un fichero GPX si el usuario lo desea. Sobre el grafo se pueden seleccionar puntos de origen, fin y puntos intermedios con los que poder generar una ruta. Además, también se permite seleccionar una distancia y un desnivel acumulado a los que la ruta que se genere se debe acercar. El usuario puede iniciar la busqueda de la ruta la cual devolverá la mejor ruta encontrada en como máximo 30 segundos. Finalmete esta ruta se puede guardar en un fichero GPX el cual es leido perfectamente por otros dispositivos para poder realizarla.

## Creación del grafo
La generación de un grafo único a partir de todas las rutas leídas es el pilar fundamental de este proyecto ya que, permite unir las rutas y hace posible la búsqueda de caminos. Gracias a varias estrategias, el programa consigue unificar las diferentes rutas que el usuario carga, pasando de tener un grafo superpoblado de nodos con la mayoria de ellos sin sentido y sin ningun tipo de conexión entre rutas, a un grafo con los nodos necesarios para la representación de todas las rutas, sin nodos ni aristas repetidas y con todas las rutas unidas entre si, en caso de que interseccionen.
