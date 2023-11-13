import QtQuick 2.15

import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

import ScreenSaver 1.0

ApplicationWindow {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Test Yaware")

    Material.theme: Material.Dark

    GridView {
        anchors.fill: parent
        model: ScreenSaver.database.imageModel.sortModel

        cellHeight: 220
        cellWidth: 210

        ScrollBar.vertical: ScrollBar {}

        delegate: ColumnLayout {

            width: GridView.view.cellWidth
            height: GridView.view.cellHeight

            Image {

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.topMargin: 5
                Layout.leftMargin: 5
                Layout.rightMargin: 5

                source: image
//                sourceSize.height: 200
//                sourceSize.width: 200
                fillMode: Image.PreserveAspectFit

            }

            Label {
                Layout.alignment: Qt.AlignHCenter
                text: diff < 0 ? "" : "Similarity: %1%".arg(diff)
                Layout.bottomMargin: 5
                Material.theme: Material.White
            }

        }
    }

    Button {
        id: button
        text: {
            switch(ScreenSaver.state) {
            case ScreenSaver.Stopped:
                return "Start"
            case ScreenSaver.Running:
                return "Stop"
            }
        }

        Material.background: {
            switch(ScreenSaver.state) {
            case ScreenSaver.Stopped:
                return Material.BlueGrey
            case ScreenSaver.Running:
                return Material.Red
            }
        }

//        Material.onAccentChanged: {
//            console.log(button.Material.accent)
//        }

//        Material.background: Material.Red

        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 16
        }

        onClicked: {
            switch(ScreenSaver.state) {
            case ScreenSaver.Stopped:
                ScreenSaver.start()
                break
            case ScreenSaver.Running:
                ScreenSaver.stop()
                break
            }
        }
    }

//    ComboBox {

//        anchors {
//            right: root.right
//            top: root.top
//        }

//        model: [
//            "Як в системі",
//            "Темна",
//            "Світла",
//        ]

//        onActivated: {
//            switch(index) {
//            case 0:
//                root.Material.theme = Material.System
//                break
//            case 1:
//                root.Material.theme = Material.Dark
//                break
//            case 2:
//                root.Material.theme = Material.Light
//            }
//        }
//    }

}
