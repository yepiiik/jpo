import QtQuick 6.9
import QtQuick.Controls 6.9
import QtLocation 6.9
import QtPositioning 6.9


Item {
    id: root

    property var markers: []
    property var highlighedtMarkers: []
    property var selectedMarker

    property alias map: map

    // Update marker creation:
    function addStations(stations) {
        clearMarkers()
        for (var i = 0; i < stations.length; i++) {
            var station = stations[i];
            var component = Qt.createComponent("StationMarker.qml");
            if (component.status === Component.Ready) {
                var marker = component.createObject(map, {
                    coordinate: QtPositioning.coordinate(
                        station.latitude,
                        station.longitude
                    ),
                    stationId: station.id,
                    stationName: station.name
                });
                markers.push(marker);
                map.addMapItem(marker);
            }
        }
    }

    function stationClicked(stationId, stationName) {
        sensorPanel.showSensors(stationId, stationName)
    }

    function highlightStations(stations) {
        clearHighlights();
        for (var i = 0; i < stations.length; i++) {
            var station = stations[i];
            addHighlightMarker(station.latitude, station.longitude, station.name);
        }
        console.log(highlightMarkers);
        fitToHighlights();
    }

    function clearHighlights() {
        // Remove all highlight markers from map
        for (var i = 0; i < highlightMarkers.length; i++) {
            map.removeMapItem(highlightMarkers[i])
            highlightMarkers[i].destroy()
        }
        highlightMarkers = []
    }

    function addHighlightMarker(latitude, longitude, stationName) {
        // Create a special marker for highlighted stations
        var component = Qt.createComponent("HighlightMarker.qml")
        if (component.status === Component.Ready) {
            var marker = component.createObject(map, {
                coordinate: QtPositioning.coordinate(latitude, longitude),
                stationName: stationName
            })
            map.addMapItem(marker)
            highlightMarkers.push(marker)
        }
    }

    function fitToHighlights() {
        // Zoom map to show all highlighted stations
        if (highlightMarkers.length === 0) return
        
        var minLat = 90, maxLat = -90
        var minLon = 180, maxLon = -180
        
        for (var i = 0; i < highlightMarkers.length; i++) {
            var coord = highlightMarkers[i].coordinate
            minLat = Math.min(minLat, coord.latitude)
            maxLat = Math.max(maxLat, coord.latitude)
            minLon = Math.min(minLon, coord.longitude)
            maxLon = Math.max(maxLon, coord.longitude)
        }
        
        // Add some padding
        var padding = 0.1
        map.fitViewportToMapItems(highlightMarkers)
        map.zoomLevel = map.zoomLevel - 0.5 // Zoom out slightly
    }

    function clearMarkers() {
        for (var i = 0; i < markers.length; i++) {
            markers[i].destroy();
        }
        markers = [];

    }

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: map
        plugin: mapPlugin
        anchors.fill: parent
        center: QtPositioning.coordinate(52.0, 19.0) // Center on Poland
        zoomLevel: 10
        focus: true

        // Store the last mouse position
        property point lastMousePos

        // Mouse area to track cursor position
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onPositionChanged: map.lastMousePos = Qt.point(mouse.x, mouse.y)
        }

        // Improved PinchHandler for touch gestures
        PinchHandler {
            id: pinch
            target: null
            onActiveChanged: if (active) {
                map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
            }
            onScaleChanged: (delta) => {
                map.zoomLevel += Math.log2(delta)
                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
            }
        }

        // Enhanced WheelHandler for cursor-centric zoom
        WheelHandler {
            id: wheel
            acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
            rotationScale: 1/120
            
            onWheel: (event) => {
                // Get current coordinate under cursor
                var coordBefore = map.toCoordinate(map.lastMousePos)
                
                // Apply zoom change
                var zoomChange = event.angleDelta.y / 120 * 0.5 // Adjust sensitivity
                map.zoomLevel += zoomChange
                
                // Get new coordinate for same screen position
                var coordAfter = map.toCoordinate(map.lastMousePos)
                
                // Adjust center to maintain cursor position
                map.center = QtPositioning.coordinate(
                    map.center.latitude + (coordBefore.latitude - coordAfter.latitude),
                    map.center.longitude + (coordBefore.longitude - coordAfter.longitude)
                )
            }
        }

        // Smooth dragging
        DragHandler {
            id: drag
            target: null
            onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
        }
    }

    // Zoom controls
    Column {
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        spacing: 5

        Button {
            text: "+"
            width: 40
            height: 40
            onClicked: map.zoomLevel = Math.min(map.zoomLevel + 1, 20) // Max zoom
        }

        Button {
            text: "-"
            width: 40
            height: 40
            onClicked: map.zoomLevel = Math.max(map.zoomLevel - 1, 3) // Min zoom
        }
    }
}
