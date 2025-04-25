import QtQuick 6.9
import QtQuick.Effects
import QtQuick.VectorImage
import QtLocation 6.9

MapQuickItem {
    id: marker
    property int stationId
    property string stationName
    
    anchorPoint.x: image.width/2
    anchorPoint.y: image.height
    
    sourceItem: Item {
        VectorImage {
            id: image
            source: "resources/marker.svg"
            width: mapComponent.selectedMarker === marker ? 48 : 32
            height: mapComponent.selectedMarker === marker ? 48 : 32
            preferredRendererType: VectorImage.CurveRenderer

            // Animate width and height changes
            Behavior on width {
                NumberAnimation {
                    duration: 200
                    easing.type: Easing.InOutQuad
                }
            }
            Behavior on height {
                NumberAnimation {
                    duration: 200
                    easing.type: Easing.InOutQuad
                }
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    mapComponent.selectedMarker = marker
                    sensorPanel.showSensors(stationId, stationName)
                }
            }
        }
    }
}
