
var exportDocumentToPNG = function(document, output_file)
{
	var exportFile = new File(output_file)
	var exportOptions = new ExportOptionsSaveForWeb()
	exportOptions.format = SaveDocumentType.PNG
	exportOptions.PNG8 = false
	document.exportDocument(exportFile, ExportType.SAVEFORWEB, exportOptions)
}

// Output directory
var outputPath = "/Users/niklasdamberg/Desktop/work/"

var localDocument = app.activeDocument
var dotPsdIndex = localDocument.name.search(".jpg")
var textureName = localDocument.name.substr(0, dotPsdIndex) + ".png"

var output_file = outputPath + textureName

exportDocumentToPNG(localDocument, output_file)

// Show alert for user that something has been done
alert("Successfully exported image!\nOutput folder: " + output_file)
