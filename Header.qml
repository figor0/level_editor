import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
Item{
    id: root
    MenuBar{
        id: model_action
        anchors{
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        Menu{
            title: qsTr("Уровень")
            Action {
                text: qsTr("Создать")
                onTriggered: {
                    level_model.create();
                }
            }
            Action {
                text: qsTr("Загрузить")
                onTriggered: {
                    loader_saver.loadDialog.open();
                }
            }
            Action {
                text: qsTr("Сохранить")
                onTriggered: {
                    if (level_model.level_path() !== ""){
                        level_model.save_level()
                    } else {
                        loader_saver.saveDialog.open()
                    }
                }
            }
            Action {
                text: qsTr("Сохранить как...")
                onTriggered: {
                    loader_saver.saveDialog.open()
                }
            }
            Action {
                text: qsTr("Очистить")
                onTriggered: {
                    level_model.clear()
                }
            }
        }
    }
    CheckBox{
        anchors.left: model_action.right
        anchors.top: parent.top
        width: 200
        height: 30
        checked: true
        text: "Показывать сетку"
        onCheckStateChanged: {
            level_area.greed_visibility = checked
        }
    }
}
