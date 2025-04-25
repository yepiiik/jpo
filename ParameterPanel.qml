import QtQuick 2.15
import QtQuick.Controls 2.15
import jpo

Rectangle {
    id: root
    color: "transparent"
    width: listView.width
    height: listView.height
    signal parameterSelected(string paramCode)
    property ListModel parameterModel: ListModel {}

    function buildParameterList(paramMap) {
        parameterModel.clear()
        const params = Object.keys(paramMap)
        console.log(paramMap)
        for (let i = 0; i < params.length; i++) {
            parameterModel.append({
                code: params[i],
                data: paramMap[params[i]],
                count: paramMap[params[i]].length
            })
            console.log(parameterModel.get(i).data)
        }
    }

    function modelToArray(model) {
        var array = [];
        for (var i = 0; i < model.count; i++) {
            array.push(model.get(i));
        }
        return array;
    }

    ListView {
        id: listView
        height: parent.height
        width: parent.width
        model: root.parameterModel

        delegate: Item {
            anchors.margins: 16
            id: delegatedItem
            width: name.width
            height: name.height
            Button {
                width: 70
                height: 30
                id: name
                text: model.code
                onClicked: {
                    mapComponent.addStations(
                        modelToArray(model.data)
                    );
                }
            }
        }

        Item {
            anchors.margins: 16
            id: all
            width: allname.width
            height: allname.height
            anchors.bottom: parent.bottom
            Button {
                width: 70
                height: 30
                id: allname
                text: "All"
                onClicked: {
                    dataFetcher.fetchStations()
                }
            }
        }
    }



    DataFetcher {
        id: fetcher
        onParametersReady: (parameters) => {
            parameterPanel.buildParameterList(parameters)
            parameterPanel.visible = true
        }
    }
}
