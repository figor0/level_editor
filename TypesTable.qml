import QtQuick 2.0
import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

Item {
    property int cellheight: (height - height/20)/15
    property int cellwidth: width/25
    property bool greed_visibility: true
    property bool changeState: false
    property alias levelModel: level_view.model
    TableView{
        id: types_model
    }
}
