import { Component, OnInit } from '@angular/core';
import * as M from 'materialize-css';
import { APIService } from '../API.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-ResetPassword',
  templateUrl: './ResetPassword.component.html',
  styleUrls: ['./ResetPassword.component.css']
})
export class ResetPasswordComponent implements OnInit {

  key: string = '';
  newPass: string = '';
  email: string = '';

  constructor(private apiTest: APIService, private router: Router) { }

  ngOnInit() {
    this.apiTest.getValidacionLinkReset()
      .subscribe(resp => {
        if (!resp.flag) {
          this.router.navigate(['Denied'])
        }
      });

    M.AutoInit();
  }

  nuevoPass() {
    if (this.key != '' && this.newPass != '' && this.email != '') {
      this.apiTest.nuevoPass(this.newPass, this.key, this.email)
        .subscribe(resp => {
          console.log(resp)
          if (resp.flag) {
            M.toast({ html: 'Password Cambiado', classes: 'green white-text' })
            setTimeout(() => {
              this.router.navigate([''])
            }, 3000);
          } else {
            M.toast({ html: 'EL Password No Se Pudo Cambiar', classes: 'red white-text' })
            setTimeout(() => {
              this.router.navigate([''])
            }, 3000);
          }
        });
    } else {
      M.toast({html:'Datos Incorrectos', classes: 'orange darken-3 white-text'})
      this.key = '';
      this.newPass = ''
      this.email = ''
    }
  }

}
