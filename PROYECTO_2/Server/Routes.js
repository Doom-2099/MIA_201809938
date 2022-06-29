// --------> Rutas del Servidor
const express = require('express');
const path = require('path');
const fs = require('fs');
const multer = require('multer');

const routes = express.Router();

const storage = multer.diskStorage({
    destination: path.resolve(__dirname, '../Server/Files/'),
    filename: function(req, file, cb) {
        cb("", file.originalname);
    }
})

const upload = multer({
    storage: storage
})


// ------------------------------------ RUTAS PETICIONES ------------------------------------------


// ------------------------------------------------------------------------------------------------

module.exports = routes;