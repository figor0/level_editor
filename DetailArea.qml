import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

Item {
    property alias setting_type_label: setting_type_label
    property alias big_type_image: big_type_image
    id: root
    Label{
        id: setting_type_label
        width: parent.width
        height: parent.height*0.1
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 10
    }
    ListView{
        id: input_list
        anchors.top: setting_type_label.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: parent.width*0.4
        height: parent.height*0.7
        model: field_model
        delegate: Item{
        id: values_input
        implicitHeight: 20
        implicitWidth: 200
        Text{
            id: type_input_name
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width/2
            text: model.type_name
        }
        Rectangle{
            anchors.left: type_input_name.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: 2
            color: "grey"
            width: parent.width/2
            TextInput{
                    id: input
                    anchors.fill: parent
                    text: model.field_value
                    color: "white"
                    onLengthChanged: {
                    model.field_value = text.toString()
                }
            }
        }
        }
    }
    Image{
        id: big_type_image
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: setting_type_label.top
        width: parent.width*0.3
        height: parent.height*0.5
    }
    Button{
        id: accepButton
        text: "Вставить"
        width: 100
        height: 40
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        onClicked: {
            blocker.visible = false
            setting_window.right_input =
                level_model.response(field_model.getData())
            if (setting_window.right_input === false){
                print("Wrong writing")
            } else{
                setting_window.visible = false
            }
        }
    }
}
