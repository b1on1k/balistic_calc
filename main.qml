import QtQuick 2.0
import QtPositioning 5.2
import QtLocation 5.3
import QtQuick.Controls 1.3


Rectangle {
    width: 1330
    height: 650
    radius: 0

    Plugin {
        id : myPlugin
        name: "nokia"
        PluginParameter { name: "app_id"; value: "tlEL3oHG88T550enP3C2" }
        PluginParameter { name: "token"; value: "i1qk5yBJHfcBXmqsxjr1Tw" }
        PluginParameter { name: "proxy"; value: "system" }
    }

//    ListView {
//        x: 131
//        y: 8
//        anchors.leftMargin: 0
//        anchors.topMargin: 0
//        anchors.rightMargin: 500
//        anchors.bottomMargin: 90
//        anchors.fill: parent
//        model: searchModel
//        delegate: Component {
//            Column {
//                Text { text: title }
//                Text { text: place.location.address.text }
//            }
//        }
//        spacing: 10

//        Button {
//            id: button2
//            objectName: "button"
//            x: 38
//            y: 537
//            text: qsTr("Подтвердить")
//            //Действие мыши
//                    MouseArea {
//                        anchors.fill: parent
//                        id: mouseArea
//                        onClicked: textField2.text = MyClass.run();
//                    }
//        }

//        TextField {
//            id: textField2
//            objectName: "textinput"
//            x: 8
//            y: 511
//            placeholderText: qsTr("Введите адрес")
//        }

//        TextArea {
//            id: textArea1
//            objectName: "memo"
//            readOnly:true
//            x: 14
//            y: 448
//            width: 115
//            height: 51
//        }
//        //        MouseArea {
////                        anchors.fill: parent
////                        onClicked: textField1.text="sssss"
////                    }
//    }

//    PlaceSearchModel {
//        id: searchModel

//        plugin: myPlugin

//        property double lat: 50.27
//        property double lon: 30.3

//        searchTerm: textField2.getText(0,textField2.text.length)
//        //Kiev
//        searchArea: QtPositioning.circle(QtPositioning.coordinate(lat, lon))

//        Component.onCompleted: update()
//    }
//    Marker{
//        id: cur_mark
        

//    }

    Map {
        id: map
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.fill: parent
        plugin: myPlugin;
        center {
            latitude: 50.27
            longitude: 30.3
        }
        zoomLevel: 13

        property variant markers
        property variant mapItems
        property int markerCounter: 0 // counter for total amount of markers. Resets to 0 when number of markers = 0
        property int currentMarker

        MapQuickItem {
                 id: point1
                 sourceItem: Image {
                   id: image1
                   width: 20
                   height: 20
                   fillMode: Image.PreserveAspectCrop
                   source: "marker.png"
                 }
//                 coordinate{
//                     latitude: 50.27
//                     longitude: 30.3
//                 }

                 opacity:1
                 anchorPoint.x: sourceItem.width/2
                 anchorPoint.y: sourceItem.height/2
        }
        MapQuickItem {
                 id: point2
                 sourceItem: Image {
                   id: image2
                   width: 20
                   height: 20
                   fillMode: Image.PreserveAspectCrop
                   source: "marker.png"
                 }
//                 coordinate{
//                     latitude: 50.27
//                     longitude: 30.3
//                 }

                 opacity:1
                 anchorPoint.x: sourceItem.width/2
                 anchorPoint.y: sourceItem.height/2
        }

             Button {
                 id: button1
                 x: 1214
                 y: 594
                 width: 81
                 height: 26
                 text: qsTr("Ready?")
                 onClicked: {
                     var f1 = false
                     var f2 = false
                     if(fromLat.text.length!=0&&fromLong.text.length!=0)
                     {
                         map.addMarker1(fromLat.text,fromLong.text)
                         f1=true
                     }
                     if(whereLat.text.length!=0&&whereLong.text.length!=0)
                     {
                         map.addMarker2(whereLat.text,whereLong.text)
                         f2=3
                     }
                     if(f1&&f2)
                     {
                         var coord1 = QtPositioning.coordinate(p1.coordinate.latitude,p1.coordinate.longitude)
                         var coord2 = QtPositioning.coordinate(p2.coordinate.latitude,p2.coordinate.longitude)
                         text1.text = coord1.distanceTo(coord2)
                         var dist = coord1.distanceTo(coord2)
                         if(textField1.text.length!=0&&textField2.text.length!=0&&textField3.length!=0&&
                            textField4.text.length!=0&&textField5.text.length!=0&&textField6.length!=0)
                            {
                                if(dist>=14000)
                                {
                                    textArea2.text="Занадто велика дистанція"
                                    textArea1.text="0"
                                }
                                else
                                {
                                    if(dist<=600)
                                    {
                                         textArea2.text="Занадто мала дистанція"
                                         textArea1.text="0"
                                    }
                                    else
                                    {

                                        console.log("Hi man")

//                                        var a=mCl.getResA(textField1.text,textField2.text,textField3.text,
//                                                              textField4.text,textField5.text,
//                                                           textField6.text,dist);
                                        textArea2.text=mCl.getResA(textField1.text,textField2.text,textField3.text,
                                                        textField4.text,textField5.text,
                                                     textField6.text, dist,p1.coordinate.latitude,p1.coordinate.longitude,
                                                                   p2.coordinate.latitude,p2.coordinate.longitude);

                                        textArea1.text=mCl.getResB(textField1.text,textField2.text,textField3.text,
                                                          textField4.text,textField5.text,
                                                     textField6.text, dist,p1.coordinate.latitude,p1.coordinate.longitude,
                                                                   p2.coordinate.latitude,p2.coordinate.longitude);
//                                        var b=mCl.getResB(textField1.text,textField2.text,textField3.text,
//                                                                textField4.text,textField5.text,
//                                                           textField6.text,dist);

//                                        console.log("A:",a,"B:",b)
                                }
                            }
                         }

                     }
                 }
             }
             Location {
                 id:p1
//                 coordinate {
//                     latitude: -27.5
//                     longitude: 153.1
//                 }
             }
             Location {
                 id:p2
//                 coordinate {
//                     latitude: -27.5
//                     longitude: 153.1
//                 }
             }

             TextField {
                 id: fromLong
                 x: 909
                 y: 599
                 width: 82
                 height: 20
                 placeholderText: qsTr("longitude")
             }


             TextField {
                 id: fromLat
                 x: 815
                 y: 599
                 width: 81
                 height: 20
                 placeholderText: qsTr("latitude")
             }

             TextField {
                 id: whereLong
                 x: 1099
                 y: 599
                 width: 82
                 height: 20
                 placeholderText: qsTr("longitude")
             }

             TextField {
                 id: whereLat
                 x: 1005
                 y: 599
                 width: 81
                 height: 20
                 placeholderText: qsTr("latitude")
             }

             Label {
                 id: label1
                 x: 815
                 y: 580
                 width: 176
                 height: 13
                 text: qsTr("From:")
                 verticalAlignment: Text.AlignVCenter
                 horizontalAlignment: Text.AlignHCenter
             }

             Label {
                 id: label2
                 x: 1005
                 y: 580
                 width: 176
                 height: 13
                 text: qsTr("Where to:")
                 verticalAlignment: Text.AlignVCenter
                 horizontalAlignment: Text.AlignHCenter
             }

             TextArea {
                 id: textArea1
                 objectName: "area1"
                 text: "0"
                 x: 545
                 y: 478
             }

             TextField {
                 id: textField1
                 objectName: "hM"
                 x: 19
                 y: 148
                 placeholderText: qsTr("висота метеостанції")
             }

             TextField {
                 id: textField2
                 objectName: "deltaHM"
                 x: 19
                 y: 185
                 placeholderText: qsTr("відхилення наземного тиску")
             }

             TextField {
                 id: textField3
                 objectName: "deltaTM"
                 x: 19
                 y: 227
                 placeholderText: qsTr("відхилення наземної температури")
             }

             TextField {
                 id: textField4
                 objectName: "hVP"
                 x: 19
                 y: 264
                 placeholderText: qsTr("висота вогневої позиції")
             }

             TextField {
                 id: textField5
                 objectName: "TZ"
                 x: 19
                 y: 303
                 placeholderText: qsTr("температура заряда")
             }

             TextField {
                 id: textField6
                 objectName: "deltaV0"
                 x: 19
                 y: 341
                 placeholderText: qsTr("відхилення початкової швидкості")
             }

             TextArea {
                 id: textArea2
                 objectName: "area2"
                 text: "0"
                 x: 298
                 y: 478
             }

             Text {
                 id: text1
                 x: 167
                 y: 580
                 width: 99
                 height: 20
                 text: qsTr("0")
                 visible: true
                 font.pixelSize: 12
             }

             Label {
                 id: label3
                 x: 298
                 y: 459
                 width: 241
                 height: 13
                 text: qsTr("Поправка а")
             }

             Label {
                 id: label4
                 x: 545
                 y: 459
                 width: 240
                 height: 13
                 text: qsTr("Поправка b")
             }


//             function deleteMarkers(){
//                      var count = map.markers.length
//                      for (var i = 0; i<count; i++){
//                          map.removeMapItem(map.markers[i])
//                          map.markers[i].destroy()
//                      }
//                      map.markers = []
//                      markerCounter = 0
//                  }

//                  function deleteMapItems(){
//                      var count = map.mapItems.length
//                      for (var i = 0; i<count; i++){
//                          map.removeMapItem(map.mapItems[i])
//                          map.mapItems[i].destroy()
//                      }
//                      map.mapItems = []

//                  }

                  function addMarker1(lat,lon){
                      var count = markerCounter
                      markerCounter++
//                      var marker = Qt.createQmlObject ('Marker {}', map)
//                      map.addMapItem(marker)
//                      marker.x = lon
//                      marker.y = lat
//                      var marker = Qt.createComponent(point)
//                      var coord = Qt.createComponent(coordinate)
                      point1.coordinate.latitude = lat
                      point1.coordinate.longitude = lon
                      p1.coordinate.latitude = lat
                      p1.coordinate.longitude = lon

                      map.addMapItem(point1)

                      //update list of markers
                      var myArray = new Array()
                      for (var i = 0; i<count; i++){
                          myArray.push(markers[i])
                      }
                      myArray.push(point1)
                      markers = myArray
                  }

                  function addMarker2(lat,lon){
                      var count = markerCounter
                      markerCounter++
//                      var marker = Qt.createQmlObject ('Marker {}', map)
//                      map.addMapItem(marker)
//                      marker.x = lon
//                      marker.y = lat
//                      var marker = Qt.createComponent(point)
//                      var coord = Qt.createComponent(coordinate)
                      point2.coordinate.latitude = lat
                      point2.coordinate.longitude = lon
                      p2.coordinate.latitude = lat
                      p2.coordinate.longitude = lon

                      map.addMapItem(point2)

                      //update list of markers
                      var myArray = new Array()
                      for (var i = 0; i<count; i++){
                          myArray.push(markers[i])
                      }
                      myArray.push(point2)
                      markers = myArray
                  }

//                  function addGeoItem(item){
//                      var count = map.mapItems.length
//                      var co = Qt.createComponent(item+'.qml')
//                      if (co.status === Component.Ready) {
//                          var o = co.createObject(map)
//                          o.setGeometry(map.markers, currentMarker)
//                          map.addMapItem(o)
//                          //update list of items
//                          var myArray = new Array()
//                          for (var i = 0; i<count; i++){
//                              myArray.push(mapItems[i])
//                          }
//                          myArray.push(o)
//                          mapItems = myArray

//                      } else {
//                          console.log(item + " is not supported right now, please call us later.")
//                      }
//                  }

//                  function deleteMarker(index){
//                      //update list of markers
//                      var myArray = new Array()
//                      var count = map.markers.length
//                      for (var i = 0; i<count; i++){
//                          if (index !== i) myArray.push(map.markers[i])
//                      }

//                      map.removeMapItem(map.markers[index])
//                      map.markers[index].destroy()
//                      map.markers = myArray
//                      if (markers.length === 0) markerCounter = 0
//                  }




//        MapItemView {
//            model: searchModel
//            delegate: MapQuickItem {
//                coordinate: place.location.coordinate

//                anchorPoint.x: image.width * 0.5
//                anchorPoint.y: image.height


//                sourceItem: Image {
//                    id: image
//                    width: 20
//                    height: 20
//                    fillMode: Image.PreserveAspectCrop
//                    source: "marker.png"
//                }
//            }
//        }
    }

//    Connections {
//        target: searchModel
//        onStatusChanged: {
//            if (searchModel.status == PlaceSearchModel.Error)
//                console.log(searchModel.errorString());
//        }
//    }

}
