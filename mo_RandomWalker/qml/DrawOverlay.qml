import QtQuick
import App.Enums

Rectangle {
	property int startX: 0
    property int startY: 0
    property int currentX: 0
    property int currentY: 0
    property bool drawing: false

    function updateLabel(label) {
        color = label === SeedLabel.Background ? "#440000FF" : "#44FF0000"
        border.color = label === SeedLabel.Background ? "blue" : "red"
    }

    visible: drawing
    border.width: 1
    x: Math.min(startX, currentX)
    y: Math.min(startY, currentY)
    width: Math.abs(currentX - startX)
    height: Math.abs(currentY - startY)
}