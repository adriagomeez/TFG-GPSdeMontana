# <img src="https://github.com/adriagomeez/TFG-GPSdeMontana/blob/main/imgs/icono.png" width="35"> TFG - Mountain GPS 

The popularity and use of GPS has grown dramatically in recent years in many areas of human life. One of them is
the realisation of mountain routes to be able to carry out activities such as cycling routes, jogging or hiking in the mountains. In
order to satisfy this need, several applications have emerged that allow the management of these routes, visualising and
sharing them in order to be able to carry out the desired activities. In this project, a multiplatform desktop application is
developed that is capable of unifying several routes loaded by the user in a single graph without repeated or redundant nodes
or edges, in order to subsequently, with a search algorithm, be able to generate routes between two selected points, with
restrictions on distance and accumulated slope and with the possibility of adding intermediate stops in it, with the best possible
relationship between speed of response and accuracy of the results. In addition, it is possible to save this route to be able to
introduce it in mountain GPS and to be able to carry it out without any problem, thus having a route generator which offers an
infinity of them thanks to all its selectable parameters.

![alt text](https://github.com/adriagomeez/TFG-GPSdeMontana/blob/main/imgs/Captura3.PNG)

## Funcionalities
The application allows the user to load an image in GeoTiff format that will be displayed on the screen and on which the user can move freely by applying zoom and dragging on it. It also allows the user to load routes in GPX format which are read and unified in a graph that is displayed on the screen. This graph can be saved in a GPX file if the user wishes. On the graph, it is possible to select start, end and intermediate points to generate a route. In addition, it is also possible to select a distance and acumulative elevation gain to which the generated route should approach. The user can start a route search, which will return the best route found within a maximum of 30 seconds. Finally, this route can be saved in a GPX file which can be read perfectly by other devices in order to be able to follow it.

The two main functionalities are:

### Graph creation
The generation of a single graph from all the routes read is the fundamental pillar of this project, as it allows the routes to be joined and makes it possible to search for paths. Thanks to various strategies, the programme manages to unify the different routes that the user loads, going from having an overpopulated graph with most of them meaningless and without any type of connection between routes, to a graph with the necessary nodes for the representation of all the routes, without repeated nodes or edges and with all the routes joined together, in the event that they intersect.

<img src="https://github.com/adriagomeez/TFG-GPSdeMontana/blob/main/imgs/grafo%20no%20creado.PNG" width="390"> <img src="https://github.com/adriagomeez/TFG-GPSdeMontana/blob/main/imgs/grafo%20creado.PNG" width="419">

The algorithm created is able to convert the unfiltered graph shown in the left image to the graph in the right image. Seen in more detail, the conversion is as follows:

<img src="https://github.com/adriagomeez/TFG-GPSdeMontana/blob/main/imgs/sin%20filtrar%20ampliado.PNG" width="400"> <img src="https://github.com/adriagomeez/TFG-GPSdeMontana/blob/main/imgs/filtrado%20ampliado.PNG" width="400">

### Path search
With an origin and an end of the route selected and with as many parameters and intermediate stops as the user wants, the programme is able to find a route that suits the user's needs as much as possible, and in a reasonable search time. For this purpose, Dijkstra, Greedy and Backtracking algorithms have been used.

The Dijkstra and Greedy algorithms are always executed, but their solution will be the final result only if the user decides not to select any distance or gradient that the route should have. With this, the program is able to generate routes that pass through all points, which will try to be the one with the least difficulty in terms of distance and gradient. As for example the following route found:

<img src=https://github.com/adriagomeez/TFG-GPSdeMontana/blob/main/imgs/ruta%20simple.PNG width="1000">

On the other hand, if the user selects a distance, a slope or both that the route must have, the algorithm used is Backtracking which, with the help of Dijkstra, Greedy and different heuristics, is able to create routes like this one:

<img src=https://github.com/adriagomeez/TFG-GPSdeMontana/blob/main/imgs/ruta%20compleja.PNG width="1000">

In which the user has selected a distance of 28.5km and a gradient of 2310m. The algorithm has managed to return a route with a distance of 28.504km and a gradient of 2557.8m. This indicates that the algorithm is capable of delivering good results.

# 
For more information of the project, with a more detailed explanation of all its functionalities, read the file TFG-GPSdeMontaña.pdf in the repository.

TFG - UAB course 2021/22 - Developed by Adrià Gómez Acosta
