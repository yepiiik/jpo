import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import jpo

ApplicationWindow {
    id: root
    width: 1200
    height: 800
    visible: true

    MouseArea {
        anchors.fill: parent
        onClicked: {
            // This will force any focused item to lose focus
            forceActiveFocus()
        }
    }

    DataFetcher {
        id: dataFetcher
        onStationsReady: (stations) => {
            mapComponent.addStations(stations)
            stationSearch.stationsModel = dataFetcher.getCachedStations()
        }
        onParametersReady: (paramMap) => {
            parameterPanel.buildParameterList(paramMap)
        }
    }

    MapComponent {
        id: mapComponent
        anchors.fill: parent
    }

    SensorPanel {
        id: sensorPanel
        height: parent.height
        anchors {
            top: stationSearch.bottom
            bottom: parent.bottom
            left: parent.left
            margins: 16
        }
    }

    StationSearch {
        id: stationSearch
        anchors {
            top: parent.top
            left: parent.left
            margins: 16
        }

        map: mapComponent.map
    }

    ParameterPanel {
        id: parameterPanel
        width: 70
        height: 300

        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        anchors.margins: 16

        onParameterSelected: (paramCode) => {
            const stations = dataFetcher.getStationsForParameter(paramCode)
        }
    }


    Component.onCompleted: {
        dataFetcher.fetchStations()
    }
}
