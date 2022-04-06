import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
Item {
    TypesTable{
        anchors{
            top: parent.top
            left:parent.left
        }

        id: typesView
    }
    LevelArea{
        id: level_area
        anchors{
            top: header.bottom
            left: parent.left
        }
        width: native_width
        height: native_height*19/20
    }
    ListView{
        id: levels_view
        model: levels_model
        delegate: Item {
            Label {
                id: levels_model.name
            }
        }
    }
}

