import { Component, OnInit } from '@angular/core';
import * as M from 'materialize-css';

@Component({
  selector: 'app-SideAdmin',
  templateUrl: './SideAdmin.component.html',
  styleUrls: ['./SideAdmin.component.css']
})
export class SideAdminComponent implements OnInit {

  user = "Administrador";
  email = "adminArchivos2022@gmail.com"

  constructor() { }

  ngOnInit() {
    M.AutoInit();
  }

}
