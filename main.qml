import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

ApplicationWindow {
    id: main_window
    title: qsTr("Редактор уровней")
    visible: true
    property int native_width: 1280
    property int native_height: 960
    width: native_width
    height: native_height
    LoaderSaver{
        id: loader_saver
    }
    Header{
        id: header
        width: parent.width
        height: parent.height/20
        anchors.top: parent.top
        anchors.left: parent.left
    }
    LevelArea{
        id: level_area
        levelModel: level_model
        anchors{
            top: header.bottom
            left: parent.left
        }
        width: native_width
        height: native_height*19/20
    }
    ApplicationWindow {
        id: types_window
        title: qsTr("Доступные типы")
        visible: false
        width: 960
        height: 620
        TypesArea{
            id: types_view
            anchors.fill: parent
            anchors.margins: 8
        }
        onClosing: {
            blocker.visible = false
        }
    }
    ApplicationWindow{
        id: setting_window
        title: qsTr("Детальное описание")
        visible: false
        width: 640
        height: 480
        property bool right_input: true
        DetailArea{
            id: detail_area
            anchors.fill: parent
        }
        onClosing: {
            if (level_area.changeState === true){
                blocker.visible = false
            } else{
                types_window.visible = true
            }
        }
    }
    Rectangle{
        id: blocker
        color: Qt.rgba(0.0, 0.0, 0.0, 0.5)
        anchors.fill:parent
        visible: false
        MouseArea{
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {

            }
        }
    }
}
