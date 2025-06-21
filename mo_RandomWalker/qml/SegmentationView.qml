import QtQuick 
import QtQuick.Window
import QtQuick.Controls 
import QtQuick.Layouts 
import QtQuick.Dialogs 
import App.Enums

Rectangle {
    id: segmentationView
    Layout.fillWidth: true
    Layout.fillHeight: true
    color: "#222"
    border.color: "#555"
    radius: 4

    property int selectedLabel: SeedLabel.Background
    
    function updateSegmentationOverlay(source) {
        segmentationOverlay.source = source
        segmentationOverlay.visible = true
    }
    function updateImageDisplay(source) {
        imageDisplay.source = source
    }
    function clearSeeds() {
        backgroundRects.clear()
        objectRects.clear()
    }
    function clearImage() {
        clearSeeds()
        imageDisplay.source = ""
        segmentationOverlay.source = ""
    }
    function tryEmitReady() {
        if (backgroundRects.count > 0 && objectRects.count > 0) {
            allSeedsCreated()
        }
    }
    function setSelectedLabel(label) {
        selectedLabel = label
    }

    signal addRectSeedArea(double x, double y, double width, double height, int label)
    signal allSeedsCreated()

    ListModel { id: backgroundRects }
    ListModel { id: objectRects }

    Component.onCompleted: {
        backgroundRects.onCountChanged.connect(tryEmitReady)
        objectRects.onCountChanged.connect(tryEmitReady)
    }

    Image {
        id: imageDisplay
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit
        cache: false
        visible: true
    }

    Image {
        id: segmentationOverlay
        anchors.fill: imageDisplay
    }

    Item {
        id: backgroundSeedLayer
        anchors.fill: imageDisplay
        z: 10
    
        Repeater {
            model: backgroundRects
            delegate: Rectangle {
                color: "#440000FF"
                border.color: "blue"
                border.width: 1
                x: model.x
                y: model.y
                width: model.width
                height: model.height
            }
        }
    }
    
    Item {
        id: objectSeedLayer
        anchors.fill: imageDisplay
        z: 10
    
        Repeater {
            model: objectRects
            delegate: Rectangle {
                color: "#44FF0000"
                border.color: "red"
                border.width: 1
                x: model.x
                y: model.y
                width: model.width
                height: model.height
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        enabled: hasUserImage

        property int startX
        property int startY
        property int currentX
        property int currentY
        property bool drawing: false
        property int imageLeft: imageDisplay.x
        property int imageTop: imageDisplay.y
        property int imageRight: imageDisplay.x + imageDisplay.width
        property int imageBottom: imageDisplay.y + imageDisplay.height

        onPressed: function(mouse) {
            startX = Math.max(imageLeft, Math.min(mouse.x, imageRight))
            startY = Math.max(imageTop, Math.min(mouse.y, imageBottom))
            currentX = startX
            currentY = startY
            drawing = true
        }

        onPositionChanged: function(mouse) {
            if (drawing) {
                currentX = mouse.x
                currentY = mouse.y
                currentX = Math.max(imageLeft, Math.min(mouse.x, imageRight))
                currentY = Math.max(imageTop, Math.min(mouse.y, imageBottom))
            }
        }

        onReleased: {
            if (!drawing) return
            drawing = false

            const imageX = imageDisplay.x;
            const imageY = imageDisplay.y;
            const imageW = imageDisplay.width;
            const imageH = imageDisplay.height;
        
            const scaleX = ImageLoader.imageWidth / imageW;
            const scaleY = ImageLoader.imageHeight / imageH;
        
            const x1 = Math.max(imageX, Math.min(startX, currentX));
            const y1 = Math.max(imageY, Math.min(startY, currentY));
            const x2 = Math.min(imageX + imageW, Math.max(startX, currentX));
            const y2 = Math.min(imageY + imageH, Math.max(startY, currentY));

            const topLeft = imageDisplay.mapFromItem(mouseArea, x1, y1)
            const bottomRight = imageDisplay.mapFromItem(mouseArea, x2, y2)

            const finalX = topLeft.x
            const finalY = topLeft.y
            const finalWidth = bottomRight.x - topLeft.x
            const finalHeight = bottomRight.y - topLeft.y

            if (selectedLabel === SeedLabel.Background) {
                backgroundRects.append({
                    "x": finalX,
                    "y": finalY,
                    "width": finalWidth,
                    "height": finalHeight
                })
            } else {
                objectRects.append({
                    "x": finalX,
                    "y": finalY,
                    "width": finalWidth,
                    "height": finalHeight
                })
            }
        
            const rx = (x1 - imageX) * scaleX;
            const ry = (y1 - imageY) * scaleY;
            const rw = (x2 - x1) * scaleX;
            const rh = (y2 - y1) * scaleY;
        
            addRectSeedArea(rx, ry, rw, rh, selectedLabel)
        }
    }
    
    Rectangle {
        id: drawOverlay
        z: 2
        visible: mouseArea.drawing
        color: selectedLabel === SeedLabel.Background ? "#440000FF" : "#44FF0000"
        border.color: selectedLabel === SeedLabel.Background ? "blue" : "red"
        border.width: 1
        x: Math.min(mouseArea.startX, mouseArea.currentX)
        y: Math.min(mouseArea.startY, mouseArea.currentY)
        width: Math.abs(mouseArea.currentX - mouseArea.startX)
        height: Math.abs(mouseArea.currentY - mouseArea.startY)
    }
}