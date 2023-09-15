/*!
* @brief Opens a file. 
* @details Places text from the file into the html element with the tag "fileContent"
*          The GetFile function is globally defined in MyApp.cpp. This is how ultralight
*          interacts with JS
*/
function onClickOpenFile() {
  const filePath = GetFilePath();
  const translatedText = GetTranslatedText(filePath)
  document.getElementById("fileContent").innerHTML = translatedText;
}