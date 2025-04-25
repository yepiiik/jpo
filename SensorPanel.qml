import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import jpo

Rectangle {
    id: root
    width: dragArea.x
    height: parent ? parent.height : 400
    radius: 16
    x: -width  // Start hidden off-screen
    visible: false  // Always visible (position handles visibility)
    antialiasing: true
    smooth: true

    property ListModel sensorModel: ListModel {}
    property string stationName: ""

    function showSensors(stationId, name) {
        stationName = name
        fetcher.fetchSensors(stationId)
        root.visible = true
    }

    function hide() {
        mapComponent.selectedMarker = false
        root.visible = false
    }

    // Animations
    NumberAnimation on x {
        id: slideIn
        to: 0  // Slide in to x=0 (fully visible)
        duration: 200
    }

    NumberAnimation on x {
        id: slideOut
        to: -width  // Slide out to x=-width (fully hidden)
        duration: 200
    }

    Rectangle {
        id: dragArea
        width: 10
        height: 40
        anchors.verticalCenter: parent.verticalCenter
        x: 300
        color: "gray"

        MouseArea {
            anchors.fill: parent
            drag.target: dragArea
            drag.axis: Drag.XAxis
            drag.minimumX: 300
            drag.maximumX: 600
        }
    }

    // Header with close button
    Item {
        id: header
        width: parent.width
        height: 50

        Text {
            text: stationName
            font.bold: true
            font.pixelSize: 16
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
                margins: 10
            }
        }

        Button {
            text: "✕"
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
                margins: 10
            }
            onClicked: root.hide()
        }
    }

    // Sensor List
    ListView {
        id: sensorList
        width: parent.width
        anchors {
            top: header.bottom
            bottom: parent.bottom
        }
        model: root.sensorModel
        clip: true
        interactive: true

        delegate: Item {
            id: delegatedItem
            width: sensorList.width
            height: expanded ? 250 : 60
            property bool expanded: model.expanded
            property var sensorData: model.data
            property var minValue: 0
            property var maxValue: 40
            property var minDate: new Date()
            property var maxDate: new Date()

            Rectangle {
                anchors.fill: parent
                color: expanded ? "#eef" : "#fff"
                border.color: "#ccc"
                border.width: 1

                Column {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 8

                    MouseArea {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 40
                        onClicked: {
                            expanded = !expanded
                            model.expanded = expanded
                            if (expanded) {
                                console.log("456")
                                delegatedItem.updateChart(sensorData)
                            }
                        }

                        Row {
                            spacing: 8
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.width

                            Text {
                                text: model.paramCode
                                font.bold: true
                                font.pixelSize: 16
                            }

                            Text {
                                color: "#AA000000"
                                text: model.data.get(0).value
                                font.bold: true
                                font.pixelSize: 16
                            }

                            Text {
                                text: expanded ? "▲" : "▼"
                                font.pixelSize: 14
                                color: "#888"
                            }
                        }
                    }

                    ChartView {
                        visible: expanded
                        width: parent.width
                        height: 180
                        antialiasing: true

                        DateTimeAxis {
                            id: xAxis
                            format: "dd MMM hh:mm"
                            titleText: "Date/Time"
                            min: minDate
                            max: maxDate
                        }

                        ValueAxis {
                            id: yAxis
                            titleText: model.paramCode
                            min: minValue
                            max: maxValue
                        }

                        LineSeries {
                            id: lineSeries
                            name: model.paramName
                            axisX: xAxis
                            axisY: yAxis
                        }
                    }
                }
            }

            function updateChart(data) {
                // Clear previous data
                lineSeries.clear();

                if (data.count === 0) return;

                // Find min and max values for scaling
                var minVal = Number.MAX_VALUE;
                var maxVal = Number.MIN_VALUE;
                var minDateVal = new Date(data.get(0).dateTime);
                var maxDateVal = new Date(data.get(data.count - 1).dateTime);

                for (var i = 0; i < data.count; i++) {
                    var value = data.get(i).value;
                    var date = new Date(data.get(i).dateTime);

                    if (value < minVal) minVal = value;
                    if (value > maxVal) maxVal = value;
                    if (date < minDateVal) minDateVal = date;
                    if (date > maxDateVal) maxDateVal = date;
                }

                // Add some padding to the range
                var range = maxVal - minVal;
                minValue = Math.max(0, minVal - range * 0.1);
                maxValue = maxVal + range * 0.1;

                // Add padding to date range (1 hour before/after)
                minDate = new Date(minDateVal.getTime() - 3600000);
                maxDate = new Date(maxDateVal.getTime() + 3600000);

                // Update the axes ranges
                yAxis.min = minValue;
                yAxis.max = maxValue;
                xAxis.min = minDate;
                xAxis.max = maxDate;

                // Add points to the series
                for (var j = 0; j < data.count; j++) {
                    var point = data.get(j);
                    lineSeries.append(new Date(point.dateTime).getTime(), point.value);
                }
            }
        }
    }

    DataFetcher {
        id: fetcher
        onSensorsReady: (sensors) => {
            root.sensorModel.clear()
            for (let i = 0; i < sensors.length; i++) {
                root.sensorModel.append({
                    sensorId: sensors[i].id,
                    paramName: sensors[i].paramName,
                    paramCode: sensors[i].paramCode,
                    expanded: false,
                    data: []
                })
                fetcher.fetchSensorData(sensors[i].id)
            }
        }

        onSensorDataReady: (sensorId, readings) => {
            // Find the item with this sensorId and update its chart
            for (var i = 0; i < sensorModel.count; i++) {
                if (sensorModel.get(i).sensorId === sensorId) {
                    var item = sensorList.itemAtIndex(i);

                    // Convert QVariantList to JS array
                    var data = [];
                    for (var j = 0; j < readings.length; j++) {
                        // Ensure date is properly converted
                        var dateTime = new Date(readings[j].dateTime);
                        if (isNaN(dateTime.getTime())) {
                            console.error("Invalid date:", readings[j].dateTime);
                            continue;
                        }
                        data.push({
                            dateTime: dateTime,
                            value: readings[j].value
                        });
                    }
                    root.sensorModel.get(i).data = data
                    break;
                }
            }
        }

        onParametersReady: (parameters) => {
            parameterPanel.buildParameterList(parameters)
            parameterPanel.visible = true
        }
    }
    border {
        color: "#11000000"
        width: 2
    }

    Component.onCompleted: {
        dataFetcher.fetchStations()
        dataFetcher.fetchAllParameters()
    }
}
