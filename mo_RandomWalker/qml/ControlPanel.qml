import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs 
import App.Enums

RowLayout {
    id: controlPanel
    spacing: 10
    Layout.alignment: Qt.AlignLeft

    signal clearImage()
    signal clearSeeds()
    signal loadImage()
    signal runSegmentation()
    signal labelChanged(int label)

    function setLabelComboEnabled(enabled) {
        labelCombo.enabled = enabled
    }
    function setSegmentationButtonEnabled(enabled) {
        segmentationButton.enabled = enabled
    }
    function currentLabelValue() {
        return labelModel.get(labelCombo.currentIndex).value
    }

    Button {
        text: "Load Image"
        onClicked: loadImage()
    }

    Button {
        text: "Clear"
        onClicked: clearImage()
    }

    Button {
        text: "Clear seeds"
        onClicked: clearSeeds()
    }

    ListModel {
        id: labelModel
        ListElement { text: "Background"; value: SeedLabel.Background }
        ListElement { text: "Object"; value: SeedLabel.Object }
    }

    ComboBox {
        id: labelCombo
        model: labelModel
        textRole: "text"
        Binding {
            target: labelCombo
            property: "enabled"
            value: hasUserImage
        }

        onCurrentIndexChanged: {
            labelChanged(labelModel.get(currentIndex).value)
        }
	}
    Button {
        id: segmentationButton
        text: "Run Algorithm"
        onClicked: runSegmentation()
    }
}