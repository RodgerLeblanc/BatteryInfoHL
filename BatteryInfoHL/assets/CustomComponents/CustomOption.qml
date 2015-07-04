import bb.cascades 1.2

Container {
    property alias optionName: label.text
    property string settingsName
    
    layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
    horizontalAlignment: HorizontalAlignment.Right
    topMargin: 30
    bottomMargin: topMargin
    
    Label { 
        id: label 
        multiline: true
        verticalAlignment: VerticalAlignment.Center
    }
    ToggleButton {
        checked: app.value(settingsName, true)
        onCheckedChanged: { app.setValue(settingsName, checked) }
        verticalAlignment: VerticalAlignment.Center
    }
}
