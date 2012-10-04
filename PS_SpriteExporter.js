
//!
//! @author Niklas Damber
//!
//! This is a PhotoShop script for exporting layers as .sprite files 
//! for using in my Mono1 game engine.
//!
//! I've only tested this on CS3.
//!

var outputPath = "/Users/Niblit/Desktop/"

var document = app.activeDocument

var dotPsdIndex = document.name.search(".psd")
var textureName = document.name.substr(0, dotPsdIndex) + ".png"

var animationsText = "animations = { }"
var defaultAnimationText = "animations[0] = { 0, -1 }"

for(index = 0; index < document.layers.length; ++index)
{
	var layer = document.layers[index]
	var bounds = layer.bounds
	
	var properties = layer.name.split(" ")
	var spriteName = null
	var rows = null
	var columns = null
	
	for(propIndex = 0; propIndex < properties.length; ++propIndex)
	{
		var propString = properties[propIndex]
		var equalsIndex = propString.search("=")
		var type = propString.substr(0, equalsIndex)
		var value = propString.substr(equalsIndex +1)
		
		if(type == "name")
			spriteName = value
		else if(type == "rows")
			rows = value
		else if(type == "columns")
			columns = value
	}
			
	var file = new File(outputPath + spriteName + ".sprite")
	file.open('w')
	
	file.writeln("")
	file.writeln("texture = \"" + textureName + "\"")
	file.writeln("rows = " + rows)
	file.writeln("columns = " + columns)
	file.writeln("x = " + (bounds[0].value +1))
	file.writeln("y = " + (bounds[1].value +1))
	file.writeln("u = " + (bounds[2].value -2))
	file.writeln("v = " + (bounds[3].value -2))
	file.writeln(animationsText)
	file.writeln(defaultAnimationText)
		
	file.close()	
}

var exportFile = new File(outputPath + textureName)
var exportOptions = new ExportOptionsSaveForWeb()
exportOptions.format = SaveDocumentType.PNG
exportOptions.PNG8 = false
document.exportDocument(exportFile, ExportType.SAVEFORWEB, exportOptions)

