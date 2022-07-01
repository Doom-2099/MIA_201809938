import { Component, OnInit, AfterViewInit } from '@angular/core';
import * as M from 'materialize-css';
import { APIService } from '../API.service';
import { UserList } from '../Models/UserList';
import { Session } from '../Models/Session';
import { Users } from '../Interfaces/UserInterface';
import { Router } from '@angular/router';
import { Folders } from '../Interfaces/FoldersInterface';
import { FolderList } from '../Models/FolderList';

@Component({
  selector: 'app-PageAdmin',
  templateUrl: './PageAdmin.component.html',
  styleUrls: ['./PageAdmin.component.css']
})
export class PageAdminComponent implements OnInit, AfterViewInit {

  user = "";
  email = "";
  colores = "";
  value = "";
  private flagUserView: boolean = false;
  private flagPermisoView: boolean = false;
  usuarios: Users[] = [];
  folders: Folders[] = [];

  constructor(private apiTest: APIService, private router: Router) { }

  ngOnInit() {
    M.AutoInit();
    if (!Session.getInstance().getFlagLog()) {
      this.router.navigate(['Denied']);
    } else {
      UserList.getInstance().clearList();

      this.apiTest.getUsers()
        .subscribe(resp => {
          resp.forEach(function (user) {
            if (user.user != 'Administrador') {
              UserList.getInstance().addUser(user);
            }
          });

          this.user = Session.getInstance().getUser();
          this.email = Session.getInstance().getEmail();
          this.usuarios = UserList.getInstance().getList();
        });

      this.apiTest.getFolders()
        .subscribe(resp => {
          resp.forEach(function(folder) {
            folder.ruta = folder.ruta.slice(1);
            FolderList.getInstance().addFolder(folder);
          })

          this.folders = FolderList.getInstance().getList();
          console.log(this.folders);

          // Meter Folders En La Vista
          // Si Le Jugas Al V***s Se Meten Las Rutas En Collapsibles
        });
    }
  }

  ngAfterViewInit(): void {
    setInterval(() => {
      M.AutoInit();
    }, 5000)
  }

  flagUsers() {
    this.flagUserView = true;
    this.flagPermisoView = false;
  }

  flagPermisos() {
    this.flagUserView = false;
    this.flagPermisoView = true;
  }

  getFlagUserView() {
    return this.flagUserView;
  }

  getFlagPermisoView() {
    return this.flagPermisoView;
  }

  getColor(user: Users): string {
    if (!user.activador) {
      return 'red-text';
    } else if (!user.habilitado && user.activador) {
      return 'orange-text';
    } else {
      return 'green-text';
    }
  }

  activarUser(usr: Users) {
    console.log(usr);
    for (var i = 0; i < this.usuarios.length; i++) {
      if (this.usuarios[i].email == usr.email) {
        this.usuarios[i].activador = true;
        UserList.getInstance().editUser(i, this.usuarios[i]);
        this.apiTest.editarUser(this.usuarios[i], 'activar')
          .subscribe(resp => {
            this.flagUsers();
          });

        break;
      }
    }
  }

  desactivarUser(usr: Users) {
    console.log(usr);
    for (var i = 0; i < this.usuarios.length; i++) {
      if (this.usuarios[i].email == usr.email) {
        this.usuarios[i].activador = false;
        UserList.getInstance().editUser(i, this.usuarios[i]);
        this.apiTest.editarUser(this.usuarios[i], 'desactivar')
          .subscribe(resp => {
            this.flagUsers();
          });

        break;
      }
    }
  }

  habilitarUser(usr: Users) {
    console.log(usr);
    for (var i = 0; i < this.usuarios.length; i++) {
      if (this.usuarios[i].email == usr.email) {
        this.usuarios[i].habilitado = !this.usuarios[i].habilitado;
        UserList.getInstance().editUser(i, this.usuarios[i]);
        this.apiTest.editarUser(this.usuarios[i], 'habilitar')
          .subscribe(resp => {
            this.flagUsers();
          });

        break;
      }
    }
  }

  Logout() {
    this.router.navigate(['']);
  }



}
