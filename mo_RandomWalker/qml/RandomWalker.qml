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

    property bool hasUserImage: false

    onHasUserImageChanged: {
        if (hasUserImage) {
            controlPanel.setLabelComboEnabled(true)
            controlPanel.setSegmentationButtonEnabled(false)
        } else {
            controlPanel.setLabelComboEnabled(false)
            controlPanel.setSegmentationButtonEnabled(false)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        ControlPanel {
            id: controlPanel

            onClearImage: {
                hasUserImage = false
                controlPanel.setLabelComboEnabled(false)
                controlPanel.setSegmentationButtonEnabled(false)
                segmentationView.clearImage()
                ImageLoader.clear_image()
                SceneManager.clear_seeds()
            }
            onClearSeeds: {
                controlPanel.setSegmentationButtonEnabled(false)
                segmentationView.clearSeeds()
                SceneManager.clear_seeds()
            }
            onRunSegmentation: {
                randomWalkerRunner.start_segmentation()
            }
            onLoadImage: {
                fileDialog.open()
            }
            onLabelChanged: function(label) {
                segmentationView.setSelectedLabel(label)
            }
        }
        SegmentationView {
            id: segmentationView
            onAddRectSeedArea: function(x, y, width, height, type) {
                SceneManager.add_rect_seed_area(Qt.rect(x, y, width, height), type)
            }
            onAllSeedsCreated: function() {
                if (mainWindow.hasUserImage) {
                    controlPanel.setSegmentationButtonEnabled(true)
                }
            }
        }
        Connections {
            target: SceneManager
            onSegmentationResultChanged: {
                segmentationView.updateSegmentationOverlay("image://segmentation/mask?" + Date.now())
            }
        }
        Connections {
            target: ImageLoader
            function onImage_changed() {
                const url = "image://loader/preview?" + Date.now()
                segmentationView.updateImageDisplay(url)
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
            controlPanel.setLabelComboEnabled(true)
            mainWindow.hasUserImage = true
        }
    }
}
