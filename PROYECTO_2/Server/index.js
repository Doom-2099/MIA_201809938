// -------------> SERVIDOR EXPRESS
const express = require('express');
const app = express();

app.use(require('./Routes'));

app.listen(3000, () => {
    console.log('Server On Port 3000');
})

