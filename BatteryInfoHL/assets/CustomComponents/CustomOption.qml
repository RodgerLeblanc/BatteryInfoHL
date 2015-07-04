import bb.cascades 1.2

Container {
    property alias optionName: label.text
    
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
        checked: app.value(label.text.trim(), true)
        onCheckedChanged: { app.setValue(label.text.trim(), checked) }
        verticalAlignment: VerticalAlignment.Center
    }
}
