import QtQuick 2.15

Rectangle {
    id:root
    implicitWidth: parent?parent.width:800
    implicitHeight: parent?parent.height:600
    color: "purple"

    property int pid: 0
    property string fullName: ""
    property int gender: 2
    property string birthDay: "0000-00-00"
    property int age: 0


    Column{

        Text {
            text: "ID:"+ root.pid|| "/"
        }

        Text {
            text:"FULL_NAME:"+ root.fullName|| "/"
        }

        Text {
            text: "GENDER:"+root.gender|| "/"
        }

        Text {
            text: "BIRTH:"+root.birthDay|| "/"
        }

        Text {
            text: "AGE:"+root.age || "/"
        }
    }

}



