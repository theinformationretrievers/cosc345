/*!
* @brief Opens a file. 
* @details Places text from the file into the html element with the tag "fileContent"
*          The GetFile function is globally defined in MyApp.cpp. This is how ultralight
*          interacts with JS
*/
function onClickOpenFile() {
  var content = GetFile();
  document.getElementById("fileContent").innerHTML = content;
}