import QtQuick 6.9
import QtLocation 6.9

MapQuickItem {
    property string stationName

    anchorPoint.x: image.width/2
    anchorPoint.y: image.height

    sourceItem: Column {
        Image {
            id: image
            source: "marker.png"
            width: 32
            height: 32
        }
        Text {
            text: stationName
            font.bold: true
            color: "black"
            style: Text.Outline
            styleColor: "white"
        }
    }
}
