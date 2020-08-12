import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

Item {
    property alias loadDialog: loadDialog
    property alias saveDialog: saveDialog
    FileDialog{
        id: loadDialog
        visible: false
        title: qsTr("Выбор уровня")
        nameFilters: ["Xml files (*.xml)"]
        folder: shortcuts.home
        onAccepted: {
            close()
            folder = shortcuts.home
            if (!level_model.load_level(fileUrl)){
                print("Can't load level file")
            }
        }
        onRejected: {
            close()
            folder = shortcuts.home
        }
    }
    FileDialog{
        id: saveDialog
        visible: false
        title: "Сохранение уровня"
        folder: shortcuts.home
        onAccepted: {
            close()
            folder = shortcuts.home
            if (level_model.save_level(fileUrl) === false){
                print("Can't save level file")
            }
        }
        onRejected: {
            close()
            folder = shortcuts.home
        }
    }
}
