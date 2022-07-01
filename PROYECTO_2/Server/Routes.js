// --------> Rutas del Servidor
const express = require('express');
const fs = require('fs');
const nodemailer = require('nodemailer');
/* const path = require('path');
const multer = require('multer'); */

const routes = express.Router();
var folders = []

// ------------------------------------ RUTAS PETICIONES ------------------------------------------
routes.post('/registerUser', (req, res) => {
    var usuarios = []

    req.on('data', data => {
        usuarios = JSON.parse(fs.readFileSync('./Files/JUsers.json'))
        usuarios.push(JSON.parse(data))
        fs.writeFileSync('./Files/JUsers.json', JSON.stringify(usuarios))

        var correo = nodemailer.createTransport({
            host: "smtp.gmail.com",
            port: 465,
            auth: {
                user: 'gerencia.archivos2022@gmail.com',
                pass: 'flyycuqnuoghzopz'
            }
        })

        var msg = '\t' + data.nombre + `Su Cuenta Se Ha Registrado Correctamente,
        Queda Pendiente La Activacion De La Misma Por Parte 
        Del Area De Administracion. Se Le Notificara Cuando
        Su Cuenta Sea Activada. 

        Para Cualquier Informacion Escribir A Este Correo: gerencia.archivos2022@gmail.com

        Manejo E Implementacion De Archivos
        Junio 2022
        `;

        var mailOptions = {
            from: 'FUBOX',
            to: data.email,
            subject: 'Registro De Cuenta',
            text: msg
        }

        correo.sendMail(mailOptions, function (error, info) {
            if (error) {
                console.log(error)
            } else {
                console.log('Email Enviado: ' + info.response);
            }
        })
    })
})

routes.post('/login', (req, res) => {
    req.on('data', data => {
        var usuarios = JSON.parse(fs.readFileSync('./Files/JUsers.json'))
        var userLog = JSON.parse(data)
        var i;

        for (i = 0; i < usuarios.length; i++) {
            if ((usuarios[i].email == userLog.email) && (usuarios[i].password == userLog.password)) {
                res.json({
                    flagLog: true,
                    user: usuarios[i].user,
                    email: usuarios[i].email,
                    habilitada: usuarios[i].habilitado,
                    activada: usuarios[i].activador
                }).end()

                break
            }
        }

        if (i == usuarios.length) {
            res.json({
                flagLog: false,
                user: '',
                email: '',
                habilitada: false,
                activada: false
            }).end()
        }
    })
})

routes.get('/getUsers', (res, req) => {
    req.json(JSON.parse(fs.readFileSync('./Files/JUsers.json'))).end()
})

routes.post('/habilitarCuenta', (req, res) => {
    var usuarios = []
    var usuario

    req.on('data', data => {
        usuarios = JSON.parse(fs.readFileSync('./Files/JUsers.json'))
        usuario = JSON.parse(data)

        for (var i = 0; i < usuarios.length; i++) {
            if (usuarios[i].user == usuario.user || usuarios[i].nombre == usuario.nombre) {
                usuarios[i] = usuario
                break
            }
        }

        fs.writeFileSync('./Files/JUsers.json', JSON.stringify(usuarios))
        res.json({ valor: 'valor' });

        var correo = nodemailer.createTransport({
            host: "smtp.gmail.com",
            port: 465,
            auth: {
                user: 'gerencia.archivos2022@gmail.com',
                pass: 'flyycuqnuoghzopz'
            }
        })

        var msg = '\t' + usuario.nombre + `Su Cuenta Se Ha Inhabilitado Debido A Superar
        El Intento Establecido De Inicio De Sesion,
        Debera De Esperar A Que Un Encargado De Administracion 
        Vuelva A Habilitar Su Cuenta.

        Para Cualquier Informacion Escribir A Este Correo: gerencia.archivos2022@gmail.com

        Manejo E Implementacion De Archivos
        Junio 2022
        `;

        var mailOptions = {
            from: 'FUBOX',
            to: usuario.email,
            subject: 'Habilitacion De Cuenta',
            text: msg
        }

        correo.sendMail(mailOptions, function (error, info) {
            if (error) {
                console.log(error)
            } else {
                console.log('Email Enviado: ' + info.response);
            }
        })
    })
})

routes.post('/activarCuenta', (req, res) => {
    var usuarios = []
    var usuario

    req.on('data', data => {
        usuarios = JSON.parse(fs.readFileSync('./Files/JUsers.json'))
        usuario = JSON.parse(data)

        for (var i = 0; i < usuarios.length; i++) {
            if (usuarios[i].user == usuario.user || usuarios[i].nombre == usuario.nombre) {
                usuarios[i] = usuario
                break
            }
        }

        fs.writeFileSync('./Files/JUsers.json', JSON.stringify(usuarios))
        res.json({ valor: 'valor' });

        var correo = nodemailer.createTransport({
            host: "smtp.gmail.com",
            port: 465,
            auth: {
                user: 'gerencia.archivos2022@gmail.com',
                pass: 'flyycuqnuoghzopz'
            }
        })

        var msg = '\t' + usuario.nombre + `Su Cuenta Ha Sido Activada, Ahora Tiene 
        Accesso A La Plataforma Con Sus Credenciales Correspondientes,
        Las Cuales Son Email y Contraseña.

        Para Cualquier Informacion Escribir A Este Correo: gerencia.archivos2022@gmail.com

        Manejo E Implementacion De Archivos
        Junio 2022
        `;

        var mailOptions = {
            from: 'FUBOX',
            to: usuario.email,
            subject: 'Activacion De Cuenta',
            text: msg
        }

        correo.sendMail(mailOptions, function (error, info) {
            if (error) {
                console.log(error)
            } else {
                console.log('Email Enviado: ' + info.response);
            }
        })
    })
})

routes.post('/desactivarCuenta', (req, res) => {
    var usuarios = []
    var usuario

    req.on('data', data => {
        usuarios = JSON.parse(fs.readFileSync('./Files/JUsers.json'))
        usuario = JSON.parse(data)

        for (var i = 0; i < usuarios.length; i++) {
            if (usuarios[i].user == usuario.user || usuarios[i].nombre == usuario.nombre) {
                usuarios[i] = usuario
                break
            }
        }

        fs.writeFileSync('./Files/JUsers.json', JSON.stringify(usuarios))
        res.json({ valor: 'valor' });

        var correo = nodemailer.createTransport({
            host: "smtp.gmail.com",
            port: 465,
            auth: {
                user: 'gerencia.archivos2022@gmail.com',
                pass: 'flyycuqnuoghzopz'
            }
        })

        var msg = '\t' + usuario.nombre + `Su Cuenta Se Ha Dado De Baja Por Ordenes De Administracion,
        Ya No Cuenta Con Acceso A La Plataforma.

        Para Cualquier Informacion Escribir A Este Correo: gerencia.archivos2022@gmail.com

        Manejo E Implementacion De Archivos
        Junio 2022
        `;

        var mailOptions = {
            from: 'FUBOX',
            to: usuario.email,
            subject: 'Desactivacion De Cuenta',
            text: msg
        }

        correo.sendMail(mailOptions, function (error, info) {
            if (error) {
                console.log(error)
            } else {
                console.log('Email Enviado: ' + info.response);
            }
        })
    })
})

routes.get('/getFoldersAdmin', (req, res) => {
    tree = JSON.parse(fs.readFileSync('./Files/JFiles.json'))
    folders = []
    recorrerArbol(tree, '')
    console.log(folders)
    res.json(folders)
    
})



// ------------------------------------------------------------------------------------------------
function recorrerArbol(root, ruta) {

    if(root.nombre == '/') {
        var folder = { 
            nombre: root.name, 
            type: root.type,
            propietario: root.propietario,
            ruta: ruta
        }
    } else {
        var folder = { 
            nombre: root.name, 
            type: root.type,
            propietario: root.propietario,
            ruta: ruta + root.name + '/'
        }
    }
    

    if(root.hijos.length != 0){
        for(var i = 0; i < root.hijos.length; i++) {
            recorrerArbol(root.hijos[i], folder.ruta);
        }
    }
    
    folders.push(folder)
    return;
}

// ------------------------------------------------------------------------------------------------

module.exports = routes;