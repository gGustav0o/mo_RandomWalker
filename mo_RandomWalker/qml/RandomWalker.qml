import QtQuick 
import QtQuick.Window
import QtQuick.Controls 
import QtQuick.Layouts 
import QtQuick.Dialogs 
import App.Enums

Window {
    id: mainWindow
    width: 1000
    height: 700
    visible: true
    title: "Random Walker Segmentation"

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        RowLayout {
            spacing: 10
            Layout.alignment: Qt.AlignLeft

            Button {
                text: "Load Image"
                onClicked: fileDialog.open()
            }

            Button {
                text: "Clear"
                onClicked: {
                    ImageLoader.clear_image()
                    hasUserImage = false
                    objectRect = Qt.rect(0, 0, 0, 0)
                    backgroundRect = Qt.rect(0, 0, 0, 0)
                    segmentationOverlay.source = ""
                }
            }
            ListModel {
                id: labelModel
                ListElement { text: "Background"; value: 0 }  // == SeedLabel.Background
                ListElement { text: "Object"; value: 1 }      // == SeedLabel.Object
            }
            ComboBox {
                id: labelCombo
                model: labelModel
                textRole: "text"
                enabled: hasUserImage
            
                Component.onCompleted: {
                    selectedLabel = labelModel.get(currentIndex).value
                }
            
                onCurrentIndexChanged: {
                    selectedLabel = labelModel.get(currentIndex).value
                }
            }

            Button {
                text: "Run Algorithm"
                enabled: canRun
                onClicked: {
                    if (canRun)
                        onClicked: randomWalkerRunner.start_segmentation()
                }
            }
        }
        Rectangle {
            id: imageContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#222"
            border.color: "#555"
            radius: 4

            Image {
                id: imageDisplay
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
                cache: false
                visible: true
                source: loaderSource
            }
            Image {
                id: segmentationOverlay
                //anchors.fill: imageDisplay
                //fillMode: Image.Stretch
                anchors.fill: imageDisplay
                source: "image://segmentation/mask"
                //visible: SceneManager.segmentationResult.width > 0
            }
            Connections {
                target: SceneManager
                onSegmentationResultChanged: {
                    // принудительно обновляем
                    segmentationOverlay.source = ""
                    segmentationOverlay.source = "image://segmentation/mask?" + Date.now()
                    segmentationOverlay.visible = true
                    backgroundSeedRect.visible = false
                    objectSeedRect.visible = false
                }
            }
            Rectangle {
                id: backgroundSeedRect
                z: 1
                color: "#440000FF"
                border.color: "blue"
                border.width: 1
                visible: backgroundRect.width > 0
                x: backgroundRect.x
                y: backgroundRect.y
                width: backgroundRect.width
                height: backgroundRect.height
            }
            
            Rectangle {
                id: objectSeedRect
                z: 1
                color: "#44FF0000"
                border.color: "red"
                border.width: 1
                visible: objectRect.width > 0
                x: objectRect.x
                y: objectRect.y
                width: objectRect.width
                height: objectRect.height
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

                    if (selectedLabel === SeedLabel.Background) {
                        backgroundRect = Qt.rect(startX, startY, 0, 0)
                    } else {
                        objectRect = Qt.rect(startX, startY, 0, 0)
                    }
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

                    const currentRect = Qt.rect(x1, y1, x2 - x1, y2 - y1)
                    if (selectedLabel === SeedLabel.Background)
                        backgroundRect = currentRect
                    else
                        objectRect = currentRect
                
                    const rx = (x1 - imageX) * scaleX;
                    const ry = (y1 - imageY) * scaleY;
                    const rw = (x2 - x1) * scaleX;
                    const rh = (y2 - y1) * scaleY;
                
                    const newRect = Qt.rect(rx, ry, rw, rh);
                    SceneManager.add_rect_seed_area(newRect, selectedLabel)
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
    }
    FileDialog {
        id: fileDialog
        title: "Choose an image"
        //currentFolder: StandardPaths.pictures
        nameFilters: ["Images (*.png *.jpg *.jpeg *.bmp)"]
        onAccepted: {
            ImageLoader.load_image(selectedFile)
            hasUserImage = true
        }
    }

    property bool hasUserImage: false
    property int selectedLabel: SeedLabel.Object
    property string loaderSource: "image://loader/preview"
    property rect objectRect: Qt.rect(0, 0, 0, 0)
    property rect backgroundRect: Qt.rect(0, 0, 0, 0)
    readonly property bool canRun: hasUserImage &&
                               backgroundRect.width > 0 && backgroundRect.height > 0 &&
                               objectRect.width > 0 && objectRect.height > 0


    Connections {
        target: ImageLoader
        function onImage_changed() {
            loaderSource = "image://loader/preview?" + Date.now()
        }
    }
}
