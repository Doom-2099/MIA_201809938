import { Component, OnInit } from '@angular/core';
import * as M from 'materialize-css';

@Component({
  selector: 'app-UsersAdmin',
  templateUrl: './UsersAdmin.component.html',
  styleUrls: ['./UsersAdmin.component.css']
})
export class UsersAdminComponent implements OnInit {
  
  constructor() { }

  ngOnInit() {
    M.AutoInit();
  }

  // Hacer Peticion De Usuarios

}
