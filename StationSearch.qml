import QtQuick 2.15
import QtQuick.Controls 2.15
import QtPositioning 5.15
import QtLocation 5.15

FocusScope {
    id: root
    width: 300
    height: 40

    property var map
    property var stationsModel: []
    property string selectedStationId: ""

    TextField {
        id: searchField
        placeholderText: "Search station..."
        color: "#000000"
        font.pixelSize: 14
        background: Rectangle {
            color: "#ffffff"
            radius: 16
            border.color: "#11000000";
            border.width: searchField.activeFocus ? 2 : 0
        }
        width: parent.width
        height: parent.height
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter

        onTextChanged: {
            if (text.length > 0) {
                suggestionList.visible = true
                suggestionList.model = filterStations(text)
            } else {
                suggestionList.visible = false
                suggestionList.model = []
            }
        }
    }

    function filterStations(searchText) {
        var filtered = []
        for (var i = 0; i < stationsModel.length; i++) {
            var station = stationsModel[i]
            if (station.name && station.name.toLowerCase().includes(searchText.toLowerCase())) {
                filtered.push(station)
            }
        }
        return filtered
    }

    ListView {
        id: suggestionList
        width: parent.width
        height: Math.min(contentHeight, 200)
        anchors.top: searchField.bottom
        clip: true
        visible: false
        model: []

        delegate: Rectangle {
            width: parent.width
            height: 40
            color: "#333333"

            Text {
                anchors.fill: parent
                anchors.leftMargin: 10
                verticalAlignment: Text.AlignVCenter
                text: modelData.name
                color:  "#ffffff"
                font.pixelSize: 14
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    selectedStationId = modelData.id
                    searchField.text = modelData.name
                    suggestionList.visible = false
                    focusMapOnStation(modelData)
                }
            }
        }

        ScrollIndicator.vertical: ScrollIndicator {}
    }

    function focusMapOnStation(station) {
        console.log(map)
        if (root.map && station.latitude && station.longitude) {
            root.map.center = QtPositioning.coordinate(station.latitude, station.longitude)
            root.map.zoomLevel = 14
        }
    }
}
