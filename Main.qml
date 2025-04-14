import QtQuick 6.9
import QtQuick.Controls 6.9  // Unified controls in Qt 6.9 (no "2" suffix)
import QtLocation 6.9        // Maps
import QtPositioning 6.9     // GPS coordinates
import jpo 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Air Quality Stations"

    DataFetcher {
        id: fetcher
        onStationsReady: mapComponent.addStations(stations)
    }

    MapComponent {
        id: mapComponent
        anchors.fill: parent
    }

    Button {
        text: "Load Stations"
        onClicked: fetcher.fetchStations()
    }
}
