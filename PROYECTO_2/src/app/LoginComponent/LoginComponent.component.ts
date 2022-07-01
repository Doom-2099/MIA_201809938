import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import * as M from 'materialize-css';
import { APIService } from '../API.service';
import { Users } from '../Interfaces/UserInterface';
import { Session } from '../Models/Session';

@Component({
   selector: 'app-LoginComponent',
   templateUrl: './LoginComponent.component.html',
   styleUrls: ['./LoginComponent.component.css']
})
export class LoginComponentComponent implements OnInit {

   emailLog: string = "";
   passLog: string = "";
   cont: number = 1;
   disabled = 'disabled btn-flat';


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

   constructor(private apiTest: APIService, private router: Router) { }

   ngOnInit(): void {
      M.AutoInit();
      Session.getInstance().clearSession();
   }

   registrarUser() {
      this.apiTest.registerUser(this.user)
         .subscribe();

      this.user.nombre = '';
      this.user.user = '';
      this.user.password = '';
      this.user.email = '';
      this.user.telefono = 0;
      this.user.fechaNacimiento = '';
      this.user.fechaRegistro = '';
      this.user.activador = false;
      this.user.habilitado = true;

      M.toast({ html: 'Cuenta Registrada, Se Le Notificara Cuando Sea Activada' })
   }

   login() {
      this.apiTest.Login(this.emailLog, this.passLog)
         .subscribe(resp => {
            Session.getInstance().setFlagLog(resp.flagLog);

            if (resp.flagLog) {
               if (resp.activada) {
                  if (resp.habilitada) {
                     this.cont = 1;
                     Session.getInstance().setUser(resp.user);
                     Session.getInstance().setEmail(resp.email);

                     if (resp.user == 'Administrador') {
                        this.router.navigate(['Administrator']);
                     } else {
                        this.router.navigate(['User']);
                     }

                  } else {
                     M.toast({ html: 'Su Cuenta Se Encuentra Inhabilitada' });
                  }
               } else {
                  M.toast({ html: 'Su Cuenta No Ha Sido Activada' })
               }
            } else {
               M.toast({ html: 'Datos Incorrectos' });
               this.disabled = 'btn';
               this.cont++;
            }
         });
   }

   
}