const fs = require('fs')

text = fs.readFile('Input.txt', 'utf-8', (err, data) => {
    if (err) throw err;
 
    // Converting Raw Buffer to text
    // data using tostring function.
    return data
})