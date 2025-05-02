import QtQuick 
import QtQuick.Window
import QtQuick.Controls 
import QtQuick.Layouts 
import QtQuick.Dialogs 


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
                onClicked: ImageLoader.clear_image()
            }

            Button {
                text: "Run Algorithm"
                onClicked: {
                    if (imagePath !== "")
                        coreController.run_algorithm()
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
        }
    }
    FileDialog {
        id: fileDialog
        title: "Choose an image"
        currentFolder: StandardPaths.pictures
        nameFilters: ["Images (*.png *.jpg *.jpeg *.bmp)"]
        onAccepted: {
            ImageLoader.load_image(selectedFile)
        }
    }

    property string loaderSource: "image://loader/preview"

    Connections {
        target: ImageLoader
        function onImage_changed() {
            loaderSource = "image://loader/preview?" + Date.now()
        }
    }
}
