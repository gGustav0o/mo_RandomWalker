import QtQuick

Item {
    id: seedLayer

    property var model
    property color fillColor: "#44FFFFFF"
    property color strokeColor: "white"
    property Item coordinateItem

    anchors.fill: parent
    z: 10

    Repeater {
        model: seedLayer.model

        delegate: Rectangle {
            color: seedLayer.fillColor
            border.color: seedLayer.strokeColor
            border.width: 1

            property point localPos: Qt.point(model.x, model.y)

            x: coordinateItem.mapToItem(seedLayer, localPos.x, localPos.y).x
            y: coordinateItem.mapToItem(seedLayer, localPos.x, localPos.y).y
            width: model.width
            height: model.height
        }
    }
}