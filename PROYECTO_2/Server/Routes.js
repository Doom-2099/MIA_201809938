// --------> Rutas del Servidor
const express = require('express');
const fs = require('fs');
/* const path = require('path');
const multer = require('multer'); */

const routes = express.Router();

// ------------------------------------ RUTAS PETICIONES ------------------------------------------
routes.post('/registerUser', (req, res) => {
    var usuarios = []

    req.on('data', data => {
        usuarios = JSON.parse(fs.readFileSync('./Files/JUsers.json'))
        usuarios.push(JSON.parse(data))
        fs.writeFileSync('./Files/JUsers.json', JSON.stringify(usuarios))
    })
})

routes.post('/login', (req, res) => {
    var usuarios = []
    

    req.on('data', data => {
        var usuarios = JSON.parse(fs.readFileSync('./Files/JUsers.json'))
        var userLog = JSON.parse(data)
        var i;

        for(i = 0; i < usuarios.length; i++) {
            if((usuarios[i].email == userLog.email) && (usuarios[i].password == userLog.password)) {
                console.log('logueado')
                res.json({
                    flagLog: true,
                    user: userLog,
                    email: usuarios[i].email
                })

                break
            }
        }

        if(i == usuarios.length) {
            console.log('no logueado')
            res.json({
                flagLog: false,
                user: '',
                email: ''
            })
        }
    })
})

// ------------------------------------------------------------------------------------------------

module.exports = routes;