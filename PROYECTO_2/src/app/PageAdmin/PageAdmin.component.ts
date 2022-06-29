import { Component, OnInit } from '@angular/core';
import * as M from 'materialize-css';

@Component({
  selector: 'app-PageAdmin',
  templateUrl: './PageAdmin.component.html',
  styleUrls: ['./PageAdmin.component.css']
})
export class PageAdminComponent implements OnInit {

  user = "Administrador";
  email = "adminArchivos2022@gmail.com"
  private flagUserView: boolean = true;
  private flagPermisoView: boolean = false;

  constructor() { }

  ngOnInit() {
    M.AutoInit();
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

}
