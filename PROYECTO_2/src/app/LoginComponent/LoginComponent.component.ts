import { HttpHeaders } from '@angular/common/http';
import { Component, OnInit } from '@angular/core';
import * as M from 'materialize-css';
import { APIService } from '../API.service';
import { Users } from '../Interfaces/UserInterface';

@Component({
  selector: 'app-LoginComponent',
  templateUrl: './LoginComponent.component.html',
  styleUrls: ['./LoginComponent.component.css']
})
export class LoginComponentComponent implements OnInit { 

  emailLog:string = "";
  passLog:string = "";

  user:Users = {
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

  constructor(private apiTest: APIService) { }

  ngOnInit(): void {
    M.AutoInit();
  }
  
  registrarUser() {
    this.apiTest.registerUser(this.user);
  }

  login() {
    console.log(this.apiTest.Login(this.emailLog, this.passLog));
  }
}