import QtQuick
import App.Enums

MouseArea {
    id: mouseArea
    property alias hasUserImage: mouseArea.enabled
    property int selectedLabel: SeedLabel.Background
    property Item imageItem
    property int imageWidth: 0
    property int imageHeight: 0

    property int startX: 0
    property int startY: 0
    property int currentX: 0
    property int currentY: 0
    property bool drawing: false

    signal rectDrawn(double rx, double ry, double rw, double rh, int label)

    anchors.fill: parent

    property int imageLeft: imageItem.x
    property int imageTop: imageItem.y
    property int imageRight: imageItem.x + imageItem.width
    property int imageBottom: imageItem.y + imageItem.height

    function updateLabel(label) {
        selectedLabel = label
    }

    onPressed: function(mouse) {
        startX = Math.max(imageLeft, Math.min(mouse.x, imageRight))
        startY = Math.max(imageTop, Math.min(mouse.y, imageBottom))
        currentX = startX
        currentY = startY
        drawing = true
    }

    onPositionChanged: function(mouse) {
        if (drawing) {
            currentX = Math.max(imageLeft, Math.min(mouse.x, imageRight))
            currentY = Math.max(imageTop, Math.min(mouse.y, imageBottom))
        }
    }

    onReleased: {
        if (!drawing) return
        drawing = false

        const x1 = Math.max(imageLeft, Math.min(startX, currentX));
        const y1 = Math.max(imageTop, Math.min(startY, currentY));
        const x2 = Math.min(imageRight, Math.max(startX, currentX));
        const y2 = Math.min(imageBottom, Math.max(startY, currentY));

        const topLeft = imageItem.mapFromItem(mouseArea, x1, y1)
        const bottomRight = imageItem.mapFromItem(mouseArea, x2, y2)

        const finalX = topLeft.x
        const finalY = topLeft.y
        const finalWidth = bottomRight.x - topLeft.x
        const finalHeight = bottomRight.y - topLeft.y

        const scaleX = imageWidth / imageItem.width
        const scaleY = imageHeight / imageItem.height

        const rx = (x1 - imageLeft) * scaleX
        const ry = (y1 - imageTop) * scaleY
        const rw = (x2 - x1) * scaleX
        const rh = (y2 - y1) * scaleY

        rectDrawn(rx, ry, rw, rh, selectedLabel)
    }
}
