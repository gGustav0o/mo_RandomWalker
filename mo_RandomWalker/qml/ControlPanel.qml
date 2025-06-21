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
    signal runSegmentation()
    signal labelChanged(int value)

    property bool hasUserImage
    property var fileDialog

    Button {
        text: "Load Image"
        onClicked: fileDialog.open()
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
        enabled: hasUserImage

        onCurrentIndexChanged: {
            let newValue = labelModel.get(currentIndex).value
            labelChanged(newValue)
        }
	}
    Button {
        text: "Run Algorithm"
        enabled: canRun
        onClicked: runSegmentation()
    }
}