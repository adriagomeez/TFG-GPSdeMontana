import QtQuick
import QtQuick.Window
import QtQuick.Controls
import graph_object 1.0
import loader_object 1.0
import QtQuick.Dialogs

Rectangle {
    id: ventana
    anchors.fill: parent

    color: "white"

    Graph {
        id: graph
    }

    Loader{
        id: mapLoader
    }

    Image {
        id: imagen
        x:0
        y:0
        fillMode: Image.PreserveAspectFit
        smooth: true
        asynchronous: false
    }

    Canvas {
        id: map
        anchors.fill: imagen

        property int radius: 1
        property int marker: 7
        property var arredges : []
        property var arrtrack : []

        onPaint: {
            var context = getContext("2d");
            if(arredges.length > 0){
                context.beginPath()
                for(var j=0; j < arredges.length; j++){
                    var edge = arredges[j]
                    context.moveTo(edge["x1"], edge["y1"])
                    context.lineTo(edge["x2"], edge["y2"])
                 }
                 context.moveTo(edge["x2"], edge["y2"])
                 context.closePath()
                 context.strokeStyle ="orange"
                 context.lineWidth = 3;
                 context.stroke()

                context.beginPath()
                for(j=0; j < arrtrack.length; j++){
                    edge = arrtrack[j]
                    context.moveTo(edge["x1"], edge["y1"])
                    context.lineTo(edge["x2"], edge["y2"])
                 }
                 context.moveTo(edge["x2"], edge["y2"])
                 context.closePath()
                 context.strokeStyle ="green"
                 context.lineWidth = 5;
                 context.stroke()

//                context.beginPath()
//                for(var i=0; i < arredges.length; i++){
//                    var node= arredges[i]
//                    context.ellipse(node["x1"]-radius, node["y1"]-radius, 2*radius, 2*radius)
//                    context.ellipse(node["x2"]-radius, node["y2"]-radius, 2*radius, 2*radius)
//                }
//                context.strokeStyle = "black"
//                context.fill()
//                context.stroke()

                for(var x=0; x < mymouse.arrpoints.length; x++){
                    context.beginPath()
                    var select= mymouse.arrpoints[x]

                    context.moveTo(select["x"], select["y"]-15)
                    context.lineTo(select["x"], select["y"])
                    context.moveTo(select["x"]-marker, select["y"]-marker-15)
                    context.closePath()
                    context.strokeStyle = "black"
                    context.lineWidth = 2;
                    context.stroke()

                    context.beginPath()
                    context.ellipse(select["x"]-marker, select["y"]-marker-15, 2*marker, 2*marker)
                    context.closePath()
                    context.strokeStyle = "black"
                    if(x==0){
                        context.fillStyle = "green"
                    }
                    else{
                        context.fillStyle = "red"
                    }
                    context.lineWidth = 2;
                    context.fill()
                    context.stroke()
                }

                for(x=0; x < mymouse.arrparadas.length; x++){
                    context.beginPath()
                    var parada= mymouse.arrparadas[x]

                    context.moveTo(parada["x"], parada["y"]-15)
                    context.lineTo(parada["x"], parada["y"])
                    context.moveTo(parada["x"]-marker, parada["y"]-marker-15)
                    context.closePath()
                    context.strokeStyle = "black"
                    context.lineWidth = 2;
                    context.stroke()

                    context.beginPath()
                    context.ellipse(parada["x"]-marker, parada["y"]-marker-15, 2*marker, 2*marker)
                    context.closePath()
                    context.strokeStyle = "black"
                    context.fillStyle = "yellow"
                    context.lineWidth = 2;
                    context.fill()
                    context.stroke()
                }
             }
             context.restore()
        }

        function clear() {
            var ctx = getContext("2d");
            ctx.reset();
            map.requestPaint();
        }

        function redrawPoints() {
            var xCoor
            var yCoor
            var l = mymouse.arrpoints.length
            mymouse.arrpoints.length=0
            for(var x = 0; x<l; x++){
                if(x==0){
                    xCoor = graph.getX(0)
                    yCoor = graph.getY(0)
                    mymouse.arrpoints.push({"x": xCoor, "y": yCoor})
                }
                else{
                    xCoor = graph.getX(1)
                    yCoor = graph.getY(1)
                    mymouse.arrpoints.push({"x": xCoor, "y": yCoor})
                }

            }

            l = mymouse.arrparadas.length
            mymouse.arrparadas.length=0
            for(x = 0; x<l; x++){
                xCoor = graph.getXParada(x)
                yCoor = graph.getYParada(x)
                mymouse.arrparadas.push({"x": xCoor, "y": yCoor})
            }
        }


        Connections {
            target: graph
            onDoCreateEdge: {
                map.arredges.push({"x1": x1, "y1": y1, "x2": x2, "y2": y2})
            }
            onDoCreateTrack: {
                map.arrtrack.push({"x1": x1, "y1": y1, "x2": x2, "y2": y2})
            }
            onDoCreateMap: {
                map.requestPaint()
                loaderBackground.stop()
                mymouse.enabled = true
            }
            onDoTrackSearched: {
                distanciaText.text = "Distancia: " + graph.getTrackDistance() + " km"
                desnivelText.text = "Desnivel: " + graph.getTrackElevation() + " m"
                map.arredges.length = 0
                map.arrtrack.length = 0
                map.clear()
                map.requestPaint()
                map.redrawPoints()
                graph.show()
            }
            onDoCalculate: {
                graph.calculateGraphParams()
                userDist.to = graph.getDistMax()
                userElev.to = graph.getDesnivelMax()
            }
        }
    }

    Connections{
        target: mapLoader
        onDoMapLoaded: {
            imagen.source = ""
            imagen.source = "image://MapProvider/-1"
            cargarMapa.charged = true;
            loaderBackground.stop()
            mymouse.enabled = true
        }
    }

    MouseArea {
        id: mymouse
        anchors.fill: imagen
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        property var arrpoints : []
        property var arrparadas : []
        property bool c: true
        property int zoom: 0
        property bool presionado: false
        property int antX: 0
        property int antY: 0
        onClicked: {
            if((cargarRutas.buscados==1 && arrpoints.length<2) || añadirParadas.paradas){

                var xCoor
                var yCoor
                if(arrpoints.length==0){
                    c = graph.isInPath(mouseX, mouseY, 0)
                    if(c){
                        xCoor = graph.getX(0)
                        yCoor = graph.getY(0)
                        arrpoints.push({"x": xCoor, "y": yCoor})
                        map.requestPaint()
                        origenText.text = "Origen: ("+graph.getLatInicio()+", "+graph.getLonInicio()+")"
                    }

                }
                else{
                    if(arrpoints.length==1){
                        c = graph.isInPath(mouseX, mouseY, 1)
                        if(c){
                            xCoor = graph.getX(1)
                            yCoor = graph.getY(1)
                            arrpoints.push({"x": xCoor, "y": yCoor})
                            map.requestPaint()
                            destinoText.text = "Destino: ("+graph.getLatFin()+", "+graph.getLonFin()+")"
                        }
                    }
                    else{
                        c = graph.isInPath(mouseX, mouseY, 2)
                        if(c){
                            xCoor = graph.getX(2)
                            yCoor = graph.getY(2)
                            arrparadas.push({"x": xCoor, "y": yCoor})
                            map.requestPaint()
                            destinoText.text = "Destino: ("+graph.getLatParada()+", "+graph.getLonParada()+")"
                        }
                    }
                }
            }
        }
        onWheel: {
            var delta = wheel.angleDelta.y

            if(delta > 0 && zoom < 18){
                zoom++
                mapLoader.mapAction(0)
                map.arredges.length = 0
                map.arrtrack.length = 0
                map.clear()
                map.requestPaint()
                imagen.source = "image://MapProvider/" + zoom
                map.redrawPoints()
                graph.show()


            }
            else{
                if(delta < 0 && zoom > 0){
                    zoom--
                    mapLoader.mapAction(1)
                    map.arredges.length = 0
                    map.arrtrack.length = 0
                    map.clear()
                    map.requestPaint()
                    imagen.source = "image://MapProvider/" + zoom
                    map.redrawPoints()
                    graph.show()
                }
            }
        }
        onPressed: {
            mymouse.cursorShape = Qt.ClosedHandCursor
            presionado = true
            antX = mouseX
            antY = mouseY
        }
        onReleased: {
            mymouse.cursorShape = Qt.PointingHandCursor
            presionado = false
        }
        onPositionChanged: {
            if(presionado){
                var desplacamientoX = mouseX - antX
                var despalacmientoY = mouseY - antY
                imagen.source = "image://MapProvider/-2/" + desplacamientoX + "/" + despalacmientoY
                antX = mouseX
                antY = mouseY
                map.arredges.length = 0
                map.arrtrack.length = 0
                map.clear()
                map.requestPaint()
                map.redrawPoints()
                graph.show()
            }
        }

    }



    Rectangle{
        id: loaderBackground
        anchors.fill: parent
        color: "white"
        opacity: 0.7
        visible: false
        BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            running: false
            width: 200
            height: 200
            opacity: 1
        }

        function start(){
            loaderBackground.visible = true;
            busyIndicator.running = true;
        }

        function stop(){
            loaderBackground.visible = false;
            busyIndicator.running = false;
        }
    }

    Rectangle{
        id: datos
        x:20
        y:10
        width: 230
        height: 118

        Rectangle{
            id: destino
            height: 40
            width: 230
            border.color: "grey"
            color: "lightgrey"
            Canvas{
                x: 10
                height: 40
                width: 30
                onPaint: {
                    var context = getContext("2d");
                    context.beginPath()

                    context.beginPath()
                    context.ellipse(15-6, 30-16, 2*6, 2*6)
                    context.closePath()
                    context.strokeStyle = "black"
                    context.fillStyle = "green"
                    context.lineWidth = 2;
                    context.fill()
                    context.stroke()
                    context.restore()
                }
            }
            Text{
                id:origenText
                x:45
                anchors.verticalCenter: destino.verticalCenter
                text: "Origen: -"
            }
        }

        Rectangle{
            id: origen
            y:39
            height: 40
            width: 230
            border.color: "grey"
            color: "lightgrey"
            Canvas{
                id: arreglar
                x: 10
                height: 40
                width: 30
                onPaint: {
                    var context = getContext("2d");
                    context.beginPath()

                    context.beginPath()
                    context.ellipse(15-6, 30-16, 2*6, 2*6)
                    context.closePath()
                    context.strokeStyle = "black"
                    context.fillStyle = "red"
                    context.lineWidth = 2;
                    context.fill()
                    context.stroke()
                    context.restore()

                }
            }
            Text{
                id:destinoText
                x:45
                anchors.verticalCenter: origen.verticalCenter
                text: "Destino: -"
            }
        }

        Rectangle{
            id: datosCamino
            y:78
            height: 40
            width: 230
            border.color: "grey"
            color: "lightgrey"

            Text{
                id:distanciaText
                x:10
                anchors.verticalCenter: datosCamino.verticalCenter
                text: "Distancia: -"
            }

            Text{
                id:desnivelText
                x:130
                anchors.verticalCenter: datosCamino.verticalCenter
                text: "Desnivel: -"
            }

        }

        MouseArea{
            anchors.fill: parent
            cursorShape: Qt.ArrowCursor
        }
    }


    Button{
        id: reset
        x:19
        y:127
        height: 41
        width: 117

        Text{
            anchors.centerIn: reset
            text: "Reelegir"
        }
        onClicked: {
            añadirParadas.paradas = false;
            mymouse.arrpoints.length = 0;
            mymouse.arrparadas.length = 0;
            map.arrtrack.length = 0;
            crearRuta.searched = false;
            graph.resetTrack();
            var ctx = map.getContext("2d");
            ctx.reset();
            map.requestPaint();
            origenText.text = "Origen: -"
            destinoText.text = "Destino: -"
            distanciaText.text = "Distancia: -"
            desnivelText.text = "Desnivel: -"
        }
    }

    Button{
        id: crearRuta
        x:134
        y:127
        height: 41
        width: 117
        property bool searched: false

        Text{
            anchors.centerIn: crearRuta
            text: "Crear Ruta"
        }
        onClicked: {
            if(mymouse.arrpoints.length >= 2 && !searched){
                mymouse.enabled = false
                searched = true
                loaderBackground.start()
                graph.searchTrack(userDist.value, userElev.value)
            }
        }
    }

    Rectangle{
        id: parametros
        x:20
        y:178
        width: 230
        height: 80

        Rectangle{
            id: datosUserDist
            width: 230
            height: 40
            border.color: "grey"
            color: "lightgrey"

            Text{
                x:10
                anchors.verticalCenter: datosUserDist.verticalCenter
                text: "Distancia: "
            }

            Text{
                id: userDistText
                x:65
                anchors.verticalCenter: datosUserDist.verticalCenter
                text: "0 km"
            }

            Slider {
                id: userDist
                x:120
                anchors.verticalCenter: datosUserDist.verticalCenter
                from: 0
                value: 0
                to: 0
                width: 100
                stepSize:0.5
                onMoved: {
                    userDistText.text = value + " km"
                    crearRuta.searched = false;
                }
            }
        }

        Rectangle{
            id: datosUserElev
            y:40
            width: 230
            height: 40
            border.color: "grey"
            color: "lightgrey"

            Text{
                x:10
                anchors.verticalCenter: datosUserElev.verticalCenter
                text: "Desnivel: "
            }

            Text{
                id: userElevText
                x:65
                anchors.verticalCenter: datosUserElev.verticalCenter
                text: "0 m"

            }

            Slider {
                id: userElev
                x:120
                anchors.verticalCenter: datosUserElev.verticalCenter
                from: 0
                value: 0
                to: 0
                stepSize:10
                width: 100
                onMoved: {
                    userElevText.text = value + " m"
                    crearRuta.searched = false;
                }
            }
        }
    }


    Button{
        id: cargarMapa
        text: "Cargar mapa"
        width: 100
        height: 41
        x:270
        y:10
        property bool charged: false
        onClicked: {
            openMap.open()
        }

        FileDialog{
            id: openMap
            nameFilters: ["TIFF files (*.tif)"]
            onAccepted: {
                mymouse.zoom = 0
                mymouse.enabled = false
                loaderBackground.start()
                mapLoader.loadMap(2, currentFile);
                if(cargarMapa.charged){
                    cargarRutas.buscados = 0
                    mymouse.arrpoints.length = 0;
                    mymouse.arrparadas.length = 0;
                    crearRuta.searched = false;
                    origenText.text = "Origen: -"
                    destinoText.text = "Destino: -"
                    graph.clear()
                    map.arredges.length = 0
                    map.arrtrack.length = 0
                    map.clear()
                    map.requestPaint()
                }
            }
        }
    }

    Button{
        id: cargarRutas
        text: "Cargar rutas"
        width: 100
        height: 41
        x:380
        y:10
        property int buscados: 0
        onClicked: {
            if(cargarMapa.charged){
                mymouse.enabled = false
                cargarRutas.buscados = 1
                openTracks.open()
            }
        }

        FileDialog{
            id: openTracks
            nameFilters: ["GPX files (*.gpx)"]
            fileMode: FileDialog.OpenFiles
            onAccepted: {
                loaderBackground.start()
                graph.load(currentFiles);
            }
        }
    }

    Button{
        id: eliminarRutas
        text: "Eliminar rutas"
        width: 100
        height: 41
        x:380
        y:56
        onClicked: {
            if(cargarMapa.charged){
                if(cargarRutas.buscados == 1){
                    userDist.to = 0
                    userElev.to = 0
                    userDistText.text = "0 km"
                    userElevText.text = "0 m"
                    cargarRutas.buscados = 0
                    mymouse.arrpoints.length = 0;
                    mymouse.arrparadas.length = 0;
                    crearRuta.searched = false;
                    origenText.text = "Origen: -"
                    destinoText.text = "Destino: -"
                    graph.clear()
                    map.arredges.length = 0
                    map.arrtrack.length = 0
                    map.clear()
                    map.requestPaint()
                }
            }
        }


    }

    Button{
        id: gurdarRutas
        text: "Guardar ruta"
        width: 100
        height: 41
        x:490
        y:10
        onClicked: {
            if(crearRuta.searched){
                saveTrack.open()
            }
        }

        FileDialog{
            id: saveTrack
            defaultSuffix: "gpx"
            nameFilters: ["GPX files (*.gpx)"]
            fileMode: FileDialog.SaveFile
            onAccepted: {
                graph.saveTrack(currentFile)
            }
        }
    }

    Button{
        id: gurdarGrafo
        text: "Guardar grafo"
        width: 100
        height: 41
        x:490
        y:56
        onClicked: {
            if(cargarRutas.buscados == 1){
                saveGraph.open()
            }
        }

        FileDialog{
            id: saveGraph
            defaultSuffix: "gpx"
            nameFilters: ["GPX files (*.gpx)"]
            fileMode: FileDialog.SaveFile
            onAccepted: {
                graph.saveGraph(currentFile)
            }
        }
    }

    Button{
        id: añadirParadas
        text: "Añadir paradas"
        width: 100
        height: 41
        x:600
        y:10
        property bool paradas: false
        onClicked: {
            if(mymouse.arrpoints.length == 2 && !crearRuta.searched){
                añadirParadas.paradas = true;
            }
        }
    }

    Button{
        id: eliminarParadas
        text: "Eliminar paradas"
        width: 100
        height: 41
        x:600
        y:56
        property bool paradas: false
        onClicked: {
            if(añadirParadas.paradas){
                crearRuta.searched = false
                map.arredges.length = 0
                map.arrtrack.length = 0
                mymouse.arrparadas.length = 0
                map.clear()
                map.requestPaint()
                graph.clearParadas()
                map.redrawPoints()
                graph.show()
            }
        }
    }


    Rectangle{
        id: bienvenida
        anchors.fill: parent
        color: "black"
        opacity: 0.8
        visible: true
        Text{
            id:bienvenidaText1
            anchors.horizontalCenter: parent.horizontalCenter
            y: parent.height/2 - 25
            text: "Bienvenido a GPS de Monataña"
            font.pointSize: 22
            font.family: "sans-serif"
            color: "white"
        }
        Text{
            id:bienvenidaText2
            anchors.horizontalCenter: parent.horizontalCenter
            y: parent.height/2 + 25
            text: "Haz click en la pantalla para empezar"
            font.pointSize: 18
            font.family: "sans-serif"
            color: "white"
        }
        MouseArea{
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                bienvenida.visible = false;
            }
        }
    }
}
