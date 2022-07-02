import { Component, OnInit, AfterViewInit } from '@angular/core';
import * as M from 'materialize-css';
import { Session } from '../Models/Session';
import { Router } from '@angular/router';
import { Users } from '../Interfaces/UserInterface';
import { UserList } from '../Models/UserList';
import { APIService } from '../API.service';
import { FolderList } from '../Models/FolderList';
import { Folders } from '../Interfaces/FoldersInterface';

@Component({
  selector: 'app-PageUser',
  templateUrl: './PageUser.component.html',
  styleUrls: ['./PageUser.component.css']
})
export class PageUserComponent implements OnInit, AfterViewInit {

  private flagAccount: boolean = false;
  private flagCollaborator: boolean = false;
  private flagFiles: boolean = false;


  user: Users = {
    nombre: "",
    user: "",
    password: "",
    email: "",
    telefono: 0,
    fechaRegistro: "",
    fechaNacimiento: "",
    activador: false,
    habilitado: false
  }

  userOld: string = "";
  actualFolder: Folders = {
    nombre: "",
    type: "",
    propietario: "",
    ruta: "",
    fecha: "",
    colaboradores: []
  };

  usuarios: Users[] = [];
  foldersUser: Folders[] = [];

  propietarioDatos = "";
  colaboradoresDatos = "";
  nombreFolder = "";
  colaborador = "";

  constructor(private apiTest: APIService, private router: Router) { }

  ngOnInit() {
    if (!Session.getInstance().getFlagLog()) {
      this.router.navigate(['Denied']);
    } else {
      UserList.getInstance().clearList();
      FolderList.getInstance().clearList();
      this.foldersUser = [];

      this.apiTest.getUsers()
        .subscribe(resp => {
          resp.forEach(function (usr) {
            if (usr.user != 'Administrador') {
              UserList.getInstance().addUser(usr);
            }
          });
          this.usuarios = UserList.getInstance().getList();

          for (var i = 0; i < this.usuarios.length; i++) {
            if (this.usuarios[i].email == Session.getInstance().getEmail()) {
              this.user = this.usuarios[i];
              this.userOld = this.user.user;
              break;
            }
          }
        });

      this.apiTest.getFolders()
        .subscribe(resp => {
          resp.forEach(function (folder) {
            FolderList.getInstance().addFolder(folder);
          })

          this.foldersUser = FolderList.getInstance().getFoldersUser(this.user.user);
        });
    }

    M.AutoInit();
  }

  ngAfterViewInit(): void {
    /* setInterval(() => {
      M.AutoInit();
    }, 5000) */
  }

  getFlagAccount() {
    return this.flagAccount;
  }

  getFlagCollaborator() {
    return this.flagCollaborator;
  }

  getFlagFiles() {
    return this.flagFiles;
  }

  flagAccountView() {
    this.flagAccount = true;
    this.flagCollaborator = false;
    this.flagFiles = false;
  }

  flagFilesView() {
    this.flagAccount = false;
    this.flagCollaborator = false;
    this.flagFiles = true;
  }

  flagCollaboratorView() {
    this.flagAccount = false;
    this.flagCollaborator = true;
    this.flagFiles = false;
  }

  editUser() {
    this.apiTest.editarUser(this.user, this.userOld, '')
      .subscribe(resp => {
        M.toast({ html: 'Se Han Guardado Los Cambios', classes: 'green white-text' });
        this.flagAccountView();
      })
  }

  setDatos(folder: Folders) {
    this.propietarioDatos = folder.propietario;
    this.nombreFolder = folder.nombre;
    this.colaboradoresDatos = folder.colaboradores.join(',');
    this.actualFolder = folder;
  }

  actualizarColaboradores() {
    // Metodo Para actualizar Colaboradores
    if (this.colaborador != "") {
      this.actualFolder.colaboradores.push(this.colaborador);
      if(this.actualFolder.colaboradores.length > 1) {
        this.colaboradoresDatos = this.actualFolder.colaboradores.join(',');
      } else {
        this.colaboradoresDatos = this.actualFolder.colaboradores[0];
      }
      
      console.log(this.actualFolder);
      this.apiTest.addColaborador(this.actualFolder)
        .subscribe(resp => {
          M.toast({ html: 'Se Ha Agregado Al Colaborador', classes: 'light-blue darken-1 white-text' })
          this.colaborador = "";
          this.flagCollaboratorView();
        });
    } else {
      M.toast({ html: 'Datos Incorrectos', classes: 'red white-text' });
    }

  }


  Logout() {
    UserList.getInstance().clearList();
    FolderList.getInstance().clearList();
    this.router.navigate(['']);
  }

}
