
//!
//! @author Niklas Damber
//!
//! This is a PhotoShop script for exporting layers as .sprite files 
//! for using in my Mono1 game engine.
//!
//! I've only tested this on CS3.
//!

var checkCurrentFileForAnimations = function(filename)
{	
	var file = new File(filename)
	var success = file.open('r')
	if(!success)
		return null

	var animations = new Array()
	while(!file.eof)
	{
		var line = file.readln()
		var index = line.search("animations")
		if(index != -1)
			animations.push(line)
	}
	
	file.close()
	
	return animations
}

var createSpriteFilesFromLayers = function(layers)
{
	var animationsText = "animations = { }"
	var defaultAnimationText = "animations[0] = { 0, -1 }"
	
	var localLayers = layers
	
	for(var index = 0; index < localLayers.length; ++index)
	{
		var layer = localLayers[index]
		var bounds = layer.bounds

		var properties = layer.name.split(" ")
		var spriteName = null
		var rows = null
		var columns = null

		for(var propIndex = 0; propIndex < properties.length; ++propIndex)
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

		var filename = outputPath + spriteName + ".sprite"
		var animations = checkCurrentFileForAnimations(filename)

		var file = new File(filename)
		file.open('w')

		file.writeln("")
		file.writeln("texture = \"" + textureName + "\"")
		file.writeln("rows = " + rows)
		file.writeln("columns = " + columns)
		file.writeln("x = " + (bounds[0].value +1))
		file.writeln("y = " + (bounds[1].value +1))
		file.writeln("u = " + (bounds[2].value -2))
		file.writeln("v = " + (bounds[3].value -2))

		if(animations == null)
		{
			file.writeln(animationsText)
			file.writeln(defaultAnimationText)			
		}
		else
		{
			for(var animIndex = 0; animIndex < animations.length; ++animIndex)
				file.writeln(animations[animIndex])
		}

		file.close()	
	}
}

var exportDocumentToPNG = function(document)
{
	var exportFile = new File(outputPath + textureName)
	var exportOptions = new ExportOptionsSaveForWeb()
	exportOptions.format = SaveDocumentType.PNG
	exportOptions.PNG8 = false
	document.exportDocument(exportFile, ExportType.SAVEFORWEB, exportOptions)	
}

var outputPath = "/Users/Niblit/Desktop/"

var localDocument = app.activeDocument
var dotPsdIndex = localDocument.name.search(".psd")
var textureName = localDocument.name.substr(0, dotPsdIndex) + ".png"

createSpriteFilesFromLayers(localDocument.layers)
exportDocumentToPNG(localDocument)
 