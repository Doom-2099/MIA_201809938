// --------> Rutas del Servidor
const express = require('express');
const fs = require('fs');
const nodemailer = require('nodemailer');
/* const path = require('path');
const multer = require('multer'); */

const routes = express.Router();
var folders = []
var timeRequest = 0
var codigo = ''

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

routes.get('/getUsers', (req, res) => {
    res.json(JSON.parse(fs.readFileSync('./Files/JUsers.json'))).end()
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
    var tree = JSON.parse(fs.readFileSync('./Files/JFiles.json'))
    folders = []
    recorrerArbol(tree, '')

    for (var i = 0; i < folders.length; i++) {
        folders[i].ruta = folders[i].ruta.slice(1);
    }

    res.json(folders)

})

routes.post('/editFolderAdmin', (req, res) => {
    var folder
    var tree = JSON.parse(fs.readFileSync('./Files/JFiles.json'))

    req.on('data', data => {
        folder = JSON.parse(data)
        tree = editarPropietario(tree, folder)

        fs.writeFileSync('./Files/JFiles.json', JSON.stringify(tree))
        res.json({ valor: 'valor' })
    })
})

routes.post('/editUser', (req, res) => {
    var usuarios = JSON.parse(fs.readFileSync('./Files/JUsers.json'))
    var tree = JSON.parse(fs.writeFileSync('./Files/JFiles.json'))

    req.on('data', data => {
        var user = JSON.parse(data)['user']


        if (user.user != JSON.parse(data)['valor']) {
            tree = editarFolderUser(tree, user)
            fs.writeFileSync('./Files/JFiles.json', JSON.stringify(tree))
        }

        for (var i = 0; i < usuarios.length; i++) {
            if (user.email == usuarios[i].email) {
                usuarios[i] = user
                break
            }
        }

        fs.writeFileSync('./Files/JUsers.json', JSON.stringify(usuarios))
        res.json({ valor: 'valor' })
    })
})

routes.post('/addColaborador', (req, res) => {
    req.on('data', data => {
        var folder = JSON.parse(data);
        var tree = JSON.parse(fs.readFileSync('./Files/JFiles.json'))

        tree = actualizarColaborador(tree, folder)

        fs.writeFileSync('./Files/JFiles.json', JSON.stringify(tree))
        res.json({ valor: 'valor' })

        var usuario = buscarUsuario(folder.colaboradores[folder.colaboradores.length - 1])

        if (usuario != null) {
            var correo = nodemailer.createTransport({
                host: "smtp.gmail.com",
                port: 465,
                auth: {
                    user: 'gerencia.archivos2022@gmail.com',
                    pass: 'flyycuqnuoghzopz'
                }
            })

            var msg = usuario.nombre + `Se Le Ha Agregado Como Colaborador En La
            Carpeta` + folder.nombre + ` Esta Carpeta Aparecera En Su Inicio La Proxima
            Vez Que Inicie Sesion En La Plataforma
    
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
        }
    })
})

routes.post('/inhabilitarCuenta', (req, res) => {
    var usuarios = []
    var email

    req.on('data', data => {
        usuarios = JSON.parse(fs.readFileSync('./Files/JUsers.json'))
        email = JSON.parse(data)['email']

        var i
        for (i = 0; i < usuarios.length; i++) {
            if (usuarios[i].email == email) {
                usuarios[i].habilitada = false;
            }
        }

        fs.writeFileSync('./Files/JUsers.json', JSON.stringify(usuarios))
        res.json({ valor: 'valor' })


        if (i >= usuarios.length) {
            var correo = nodemailer.createTransport({
                host: "smtp.gmail.com",
                port: 465,
                auth: {
                    user: 'gerencia.archivos2022@gmail.com',
                    pass: 'flyycuqnuoghzopz'
                }
            })

            var msg = `Se Ha Inhabilitado La Cuenta Correspondiente 
            Al Correo ` + email + ` Debido A Que Ha Cometido Varios Intentos
            Erroneos De Iniciar Sesion, Para Habilitar La Cuenta, Esta Se Remarcara 
            Como Inhabilitada En La Pagina De Inicio Del Administrador.
            
            Para Cualquier Informacion Escribir A Este Correo: gerencia.archivos2022@gmail.com

            Manejo E Implementacion De Archivos
            Junio 2022
            `;

            var mailOptions = {
                from: 'FUBOX',
                to: 'gerencia.archivos2022@gmail.com',
                subject: 'Inhabilitacion De Cuenta',
                text: msg
            }

            correo.sendMail(mailOptions, function (error, info) {
                if (error) {
                    console.log(error)
                } else {
                    console.log('Email Enviado: ' + info.response);
                }
            })
        }
    })
})

routes.post('/codigoNewPass', (req, res) => {
    var startDate = new Date()
    timeRequest = startDate.getMinutes()
    codigo = Math.round(Math.random() * 999999).toString();

    req.on('data', data => {
        var correo = nodemailer.createTransport({
            host: "smtp.gmail.com",
            port: 465,
            auth: {
                user: 'gerencia.archivos2022@gmail.com',
                pass: 'flyycuqnuoghzopz'
            }
        })

        var msg = `Para Reestablecer Su Password Debera De Ingresar
        El Siguiente Codigo: ` + codigo + ` En El Siguiente Enlace
        http://localhost:4200/Reset En Los Proximos 5 Minutos Antes De 
        Que Acabe La Validez Del Enlace 
        
        Para Cualquier Informacion Escribir A Este Correo: gerencia.archivos2022@gmail.com
        
        Manejo E Implementacion De Archivos
        Junio 2022
        `;

        var mailOptions = {
            from: 'FUBOX',
            to: JSON.parse(data)['email'],
            subject: 'Inhabilitacion De Cuenta',
            text: msg
        }

        correo.sendMail(mailOptions, function (error, info) {
            if (error) {
                console.log(error)
            } else {
                console.log('Email Enviado: ' + info.response);
            }
        })

        res.json({ valor: 'valor' })
    })
})

routes.get('/getFlagLink', (req, res) => {
    var start = new Date()
    if (((parseInt(start.getMinutes()) - parseInt(timeRequest)) < 5) && (timeRequest > 0)) {
        res.json({ flag: true })
        timeRequest = 0
    } else {
        res.json({ flag: false })
        timeRequest = 0
    }
})

routes.post('/resetPass', (req, res) => {
    var usuarios = []
    var datos 

    req.on('data', data => {
        usuarios = JSON.parse(fs.readFileSync('./Files/JUsers.json'))
        datos = JSON.parse(data)

        if (datos.key == codigo) {
            for (var i = 0; i < usuarios.length; i++) {
                if (usuarios[i].email == datos.email) {
                    usuarios[i].password = datos.pass
                    break
                }
            }

            fs.writeFileSync('./Files/JUsers.json', JSON.stringify(usuarios))
            res.json({ flag: true })
            codigo = ''
        } else {
            res.json({ flag: false })
            codigo = ''
        }
    })
})

// ------------------------------------------------------------------------------------------------
function recorrerArbol(root, ruta) {

    if (root.nombre == '/') {
        var folder = {
            nombre: root.name,
            type: root.type,
            propietario: root.propietario,
            fecha: root.fecha,
            ruta: ruta,
            colaboradores: root.colaboradores
        }
    } else {
        var folder = {
            nombre: root.name,
            type: root.type,
            propietario: root.propietario,
            fecha: root.fecha,
            ruta: ruta + root.name + '/',
            colaboradores: root.colaboradores
        }
    }


    if (root.hijos.length != 0) {
        for (var i = 0; i < root.hijos.length; i++) {
            recorrerArbol(root.hijos[i], folder.ruta);
        }
    }

    folders.push(folder)
    return;
}

function editarPropietario(root, folder) {
    if (root.name == folder.nombre) {
        root.propietario = folder.propietario
        return root
    } else {
        if (root.hijos.length != 0) {
            for (var i = 0; i < root.hijos.length; i++) {
                root.hijos[i] = editarPropietario(root.hijos[i], folder)
            }
        }

        return root;
    }
}

function editarFolderUser(root, oldUser, user) {
    if (root.propietario == oldUser) {
        root.propietario = user.user
        return root
    } else {
        if (root.hijos.length != 0) {
            for (var i = 0; i < root.hijos.length; i++) {
                root.hijos[i] = editarFolderUser(root.hijos[i], oldUser, user)
            }
        }

        return root
    }
}

function actualizarColaborador(root, folder) {
    if (root.name == folder.nombre) {
        root.colaboradores = folder.colaboradores
        return root
    } else {
        if (root.hijos.length != 0) {
            for (var i = 0; i < root.hijos.length; i++) {
                root.hijos[i] = actualizarColaborador(root.hijos[i], folder)
            }
        }

        return root
    }
}

function buscarUsuario(usuario) {
    var usuarios = JSON.parse(fs.readFileSync('./Files/JUsers.json'))

    for (var i = 0; i < usuarios.length; i++) {
        if (usuarios[i].user == usuario) {
            return usuarios[i];
        }
    }

    return null
}
// ------------------------------------------------------------------------------------------------

module.exports = routes;