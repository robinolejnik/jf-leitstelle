import QtQuick 2.0
import QtPositioning 5.9
import QtLocation 5.9

Item {
    width: 400; height: 300
    Map {
        id: map
        anchors.fill: parent
        plugin: Plugin { name: "osm" }
        center: QtPositioning.coordinate(51.678225, 7.792301)
        zoomLevel: 17

        MapQuickItem {
            id: marker_hw
            coordinate: QtPositioning.coordinate(51.678225, 7.792301)
            sourceItem: Image {
                id: markerimage_hw
                source: "icon/map-marker-blue.png"
                height: 50
                width: 50
            }
            anchorPoint.x: markerimage.width / 2
            anchorPoint.y: markerimage.height
        }

        MapQuickItem {
            id: marker
            coordinate: QtPositioning.coordinate(51.678225, 7.792301)
            sourceItem: Image {
                id: markerimage
                source: "icon/map-marker-red.png"
                height: 50
                width: 50
            }
            anchorPoint.x: markerimage.width / 2
            anchorPoint.y: markerimage.height
        }

        MapItemView {
            model: routeModel
            delegate: Component {
                MapRoute {
                    route: routeData
                    line.color: "red"
                    line.width: 4
                    smooth: true
                }
            }
        }
    }

    GeocodeModel {
        id: geo
        plugin: map.plugin
        onLocationsChanged: {
            if(count) {
                marker.coordinate = get(0).coordinate
                map.center = get(0).coordinate
                routeQuery.clearWaypoints()
                routeModel.reset()
                routeQuery.addWaypoint(QtPositioning.coordinate(51.678225, 7.792301))
                routeQuery.addWaypoint(get(0).coordinate)
                routeModel.update()
                marker.visible = true
            }
        }
        Component.onCompleted: update()
    }

    RouteModel {
        id: routeModel
        plugin: map.plugin
        query: RouteQuery {
            id: routeQuery
        }
        Component.onCompleted: {
            update()
        }
    }

    function updateGeoData(value) {
        geo.query = value
        routeQuery.clearWaypoints()
        routeModel.update()
        geo.update()
    }

    function signalResetMap() {
        update()
        routeModel.reset()
        marker.visible = false
    }

}
