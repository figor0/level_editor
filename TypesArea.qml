import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3


Item {
    GridView{
        id: types_view
        anchors.fill: parent
        anchors.margins: 10
        model:	types_model
        cellHeight: 150
        cellWidth: 150
        delegate: Rectangle{
            id: type_cell
            color: (model.unique === true || model.amount === 0) ? "yellow" : "grey"
            implicitHeight: 140
            implicitWidth: 140
            Label{
                id: type_name
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                text: model.type_name
                color: "black"
            }
            Image{
                id: type_image
                anchors.top: type_name.bottom
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width: parent.width/2
                source: model.image_path
            }
            MouseArea{
                id: types_mouse_area
                anchors.fill: parent
                property int changeType: 0
                enabled: model.unique === true ? model.amount === 0 : true
                onClicked: {
                    if (model.unique === false || model.amount === 0){
                        types_window.visible = false
                        level_model.setDstType(model.type_name)
                        field_model.setFieldTypes(model.type_fields)
                        detail_area.setting_type_label.text = model.type_name
                        detail_area.big_type_image.source = model.image_path
                        setting_window.visible = true
                    }
                }
            }
        }
    }
}
