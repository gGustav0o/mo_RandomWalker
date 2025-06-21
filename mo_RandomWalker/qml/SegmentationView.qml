import QtQuick 
import QtQuick.Window
import QtQuick.Controls 
import QtQuick.Layouts 
import QtQuick.Dialogs 
import App.Enums

Rectangle {
    Layout.fillWidth: true
    Layout.fillHeight: true
    color: "#222"
    border.color: "#555"
    radius: 4

    property int selectedLabel: SeedLabel.Background
    property bool hasUserImage: false

    onSelectedLabelChanged: {
        drawOverlay.updateLabel(selectedLabel)
        mouseArea.updateLabel(selectedLabel)
    }
    
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
        drawOverlay.updateLabel(label)  
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

    SeedLayer {
        id: backgroundSeedLayer
        model: backgroundRects
        fillColor: "#440000FF"
        strokeColor: "blue"
        coordinateItem: imageDisplay
    }
    
    SeedLayer {
        id: objectSeedLayer
        model: objectRects
        fillColor: "#44FF0000"
        strokeColor: "red"
        coordinateItem: imageDisplay
    }

    RectSeedMouseArea {
        id: mouseArea
        hasUserImage: hasUserImage
        imageItem: imageDisplay
        imageWidth: ImageLoader.imageWidth
        imageHeight: ImageLoader.imageHeight
        selectedLabel: selectedLabel
    
        onRectDrawn: (x, y, w, h, label) => {
            const rect = { x, y, width: w, height: h }
    
            if (label === SeedLabel.Background) {
                backgroundRects.append(rect)
            } else {
                objectRects.append(rect)
            }
    
            addRectSeedArea(x, y, w, h, label)
        }
    }
    
    DrawOverlay {
        id: drawOverlay
        startX: mouseArea.startX
        startY: mouseArea.startY
        currentX: mouseArea.currentX
        currentY: mouseArea.currentY
        drawing: mouseArea.drawing
    }
}