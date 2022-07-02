import { DatePipe } from '@angular/common';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Users } from './Interfaces/UserInterface';
import { SessionInterface } from './Interfaces/SessionInterface';
import { Folders } from './Interfaces/FoldersInterface';

const headerDict = {
   'Content-Type': 'application/json',
   'Accept': 'application/json',
   'Access-Control-Allow-Origin': '*'
}

const requestOptions = {
   headers: new HttpHeaders(headerDict)
}

@Injectable({
   providedIn: 'root'
})
export class APIService {

   private date: Date = new Date();
   datePipe: DatePipe = new DatePipe('en-US');
   formDate: string = "";


   constructor(private http: HttpClient) { }


   // ------> PETICIONES
   registerUser(user: Users) {
      this.formDate = this.datePipe.transform(this.date, 'mediumDate')!;
      user.fechaRegistro = this.formDate;

      return this.http.post('http://localhost:5000/registerUser', user, requestOptions)
   }

   Login(email: string, password: string) {
      return this.http.post<SessionInterface>('http://localhost:5000/login', { email: email, password: password }, requestOptions);
   }

   getUsers() {
      return this.http.get<Users[]>('http://localhost:5000/getUsers', requestOptions);
   }

   editarUser(user: Users, nuevoUser: string, tipo: string) {
      if (tipo == 'activar') {
         return this.http.post('http://localhost:5000/activarCuenta', user, requestOptions);
      } else if (tipo == 'desactivar') {
         return this.http.post('http://localhost:5000/desactivarCuenta', user, requestOptions);
      } else if (tipo == 'habilitar') {
         return this.http.post('http://localhost:5000/habilitarCuenta', user, requestOptions);
      } else {
         return this.http.post('http://localhost:5000/editUser', { user: user, valor: nuevoUser }, requestOptions);
      }
   }

   inhabilitarCuentaLogin(email: string) {
      return this.http.post('http://localhost:5000/inhabilitarCuenta', { email: email }, requestOptions)
   }

   getFolders() {
      return this.http.get<Folders[]>('http://localhost:5000/getFoldersAdmin', requestOptions);
   }

   editPropetary(folder: Folders) {
      return this.http.post('http://localhost:5000/editFolderAdmin', folder, requestOptions);
   }

   addColaborador(folder: Folders) {
      return this.http.post('http://localhost:5000/addColaborador', folder, requestOptions)
   }

   codigoRecPass(email: string) {
      return this.http.post('http://localhost:5000/codigoNewPass', { email: email }, requestOptions);
   }

   getValidacionLinkReset() {
      return this.http.get<{ flag: boolean }>('http://localhost:5000/getFlagLink', requestOptions)
   }

   nuevoPass(newPass: string, key: string, email: string) {
      return this.http.post<{ flag: boolean }>('http://localhost:5000/resetPass', { pass: newPass, key: key, email: email }, requestOptions);
   }
}

