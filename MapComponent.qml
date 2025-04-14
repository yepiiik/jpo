import QtQuick 6.9
import QtQuick.Controls 6.9
import QtLocation 6.9
import QtPositioning 6.9


Item {
    id: root

    property var markers: []

    function addStations(stations) {
        clearMarkers();

        for (var i = 0; i < stations.length; i++) {
            var station = stations[i];
            var component = Qt.createComponent("StationMarker.qml");
            if (component.status === Component.Ready) {
                var marker = component.createObject(map, {
                    coordinate: QtPositioning.coordinate(
                        station.latitude,
                        station.longitude
                    ),
                    stationName: station.name
                });
                map.addMapItem(marker);
            }
        }

        markers.push(marker);
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
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(52.0, 19.0) // Center on Poland
        zoomLevel: 10 // Default zoom (1-20 typically)
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

    // Current position indicator (optional)
    MapQuickItem {
        coordinate: map.center
        anchorPoint.x: image.width/2
        anchorPoint.y: image.height
        
        sourceItem: Image {
            id: image
            source: "qrc:/crosshair.png"
            width: 32
            height: 32
        }
    }
}
