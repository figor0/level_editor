import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3


Item {
    id: root
    property int cellheight: (height - height/20)/15
    property int cellwidth: width/25
    property bool greed_visibility: true
    property bool changeState: false
    property alias levelModel: level_view.model
    TableView{
        id: level_view
        interactive: false
        anchors.fill: parent
        rowSpacing: 0
        columnSpacing: 0
        onModelChanged: {
            print("model changed")
            model = levelModel
        }

        delegate: Rectangle{
            id: cell
            border.color: "black"
            border.width: greed_visibility === true ? 1 : 0
            implicitHeight: root.cellheight
            implicitWidth: root.cellwidth
            color: cell_image.source === "" ? "black" : "white"
            Image{
                id: cell_image
                anchors.fill: parent
                source: model.image_path
            }
            MouseArea{
                id: click_area
                anchors.fill: parent
                acceptedButtons: Qt.RightButton
                onClicked: {
                    if ( mouse.button === Qt.RightButton)
                        contextMenu.popup();
                }
                Menu{
                    id: contextMenu
                    width: parent.width*2
                    property bool empty: model.type_name === "" ? false : true
                    MenuItem {
                        text: "Изменить"
                        visible: contextMenu.empty
                        height: contextMenu.empty === true ? implicitHeight : 0
                        onTriggered: {
                            changeState = true
                            blocker.visible = true
                            model.setDstType(model.type_name)
                            model.setDst(index)
                            field_model.setField(model.data_types,
                                                 model.data_members)
                            detail_area.setting_type_label.text = model.type_name
                            detail_area.big_type_image.source = model.image_path
                            setting_window.visible = true;
                        }
                    }
                    MenuItem {
                        text: "Вставить"
                        height: !contextMenu.empty === true ? implicitHeight : 0
                        visible: !contextMenu.empty
                        onTriggered: {
                            blocker.visible = true
                            types_window.visible = true
                            model.setDst(index)
                        }
                    }
                    MenuItem {
                        text: "Очистить"
                        visible: contextMenu.empty
                        height: contextMenu.empty === true ? implicitHeight : 0
                        onTriggered: {
                            model.clear(index)
                        }
                    }
                }
            }
        }
    }
}
