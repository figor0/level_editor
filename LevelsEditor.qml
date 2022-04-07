import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
Item {
    property alias model: levels_view.model
    TypesArea
    {
        id: types_view
        height: parent.height/10
        anchors {
            top: parent.top
            left: parent.left
            right: levels_manager.left
        }
    }
    LevelArea{
        id: level_area
        anchors{
            top: types_view.bottom
            left: parent.left
            right: levels_manager.left
            bottom: parent.bottom
        }
    }
    Item{
        id: levels_manager
        width: parent.width/5
        anchors{
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        Button{
            id: addLevelButton
            anchors{
                top: parent.top
                left: parent.left
                bottom: levels_view.top
            }

            text: qsTr("Append Level")
            onClicked: {
                levels_model.appendModel()
            }
        }

        ListView{
            id: levels_view
            model: levels_model
            anchors{
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                top: parent.top
                topMargin: 40
            }
            delegate: Item {
                Rectangle{
                    color: levels_view.currentIndex === index ? "green" : "yellow"
                    anchors.fill: parent
                    Label {
                        id: level_name
                        text: model.name
                        anchors.fill: parent
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            print("level area clicked")
                            if (levels_view.currentIndex != index)
                                levels_view.currentIndex = index
                        }
                    }
                }
                implicitHeight: 30
                implicitWidth: parent.width
            }
            onCurrentItemChanged: {
                print("onCurrentItemChanged")
                level_area.levelModel =
                        levels_model.get(levels_view.currentIndex, 0).data
            }
        }
    }

}

