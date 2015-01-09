
//!
//! @author Niklas Damber
//!
//! This is a PhotoShop script for exporting layers as .sprite files 
//! and for using in my Mono1 game engine.
//!
//! Tested on CS5.
//!

var checkFileForAttributes = function(filename, attribute)
{
	var file = new File(filename)
	var success = file.open('r')
	if(!success)
		return null

	var attributes = new Array()
	while(!file.eof)
	{
		var line = file.readln()
		var index = line.search(attribute)
		if(index != -1)
			attributes.push(line)
	}

	file.close()

	return attributes
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
		var animations = checkFileForAttributes(filename, "animations")
		var attributes = checkFileForAttributes(filename, "attributes")

		var file = new File(filename)
		file.lineFeed = "Macintosh"
		file.open('w')
		
		file.writeln("")
		file.writeln("-- This sprite is exported from " + localDocument.name)
		file.writeln("")

		file.writeln("texture = \"" + textureName + "\"")
		file.writeln("rows = " + rows)
		file.writeln("columns = " + columns)
		file.writeln("x = " + (bounds[0].value +1))
		file.writeln("y = " + (bounds[1].value +1))
		file.writeln("u = " + (bounds[2].value -2))
		file.writeln("v = " + (bounds[3].value -2))
		file.writeln("")

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

		// Write previous attributes if there were any
		if(attributes != null)
		{
			file.writeln("")

			for(var attribIndex = 0; attribIndex < attributes.length; ++attribIndex)
				file.writeln(attributes[attribIndex])
		}

		file.close()	
	}
}

var createPathFiles = function(pathItems)
{
	var localPathItems = pathItems
	for(var index = 0; index < localPathItems.length; ++index)
	{
		var pathItem = localPathItems[index]
		var filename = outputPath + pathItem.name + ".path"

		// Create the file
		var file = new File(filename)
		file.lineFeed = "Macintosh";
		file.open('w')

		file.writeln("")
		file.writeln("-- This path is exported from " + localDocument.name)
		file.writeln("-- The path variable contains a collection of x and y pairs")
		file.writeln("")
		file.writeln("path = { }")

		for(var subIndex = 0; subIndex < pathItem.subPathItems.length; ++subIndex)
		{
			var subItem = pathItem.subPathItems[subIndex]
			var points = subItem.pathPoints

			// Let this be the base for the local path coordinate system
			var basePoint = points[0]

			for(var pointIndex = 0; pointIndex < points.length; ++pointIndex)
			{
				var point = points[pointIndex]
				var x = point.anchor[0] - basePoint.anchor[0]
				var y = (point.anchor[1] - basePoint.anchor[1]) * -1
				file.writeln("path[" + pointIndex + "] = { " + x + ", " + y + " }")
			}
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

// Output directory
var outputPath = "/Users/niklasdamberg/Desktop/work/"

var localDocument = app.activeDocument
var dotPsdIndex = localDocument.name.search(".psd")
var textureName = localDocument.name.substr(0, dotPsdIndex) + ".png"

// Start exporting
createSpriteFilesFromLayers(localDocument.layers)
createPathFiles(localDocument.pathItems)
exportDocumentToPNG(localDocument)
 
// Show alert for user that something has been done
alert("Successfully exported sprite sheet!\nOutput folder: "  + outputPath)


