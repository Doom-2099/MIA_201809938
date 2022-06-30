import { DatePipe } from '@angular/common';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Users } from './Interfaces/UserInterface';
import { UserList } from './Models/UserList';
import { SessionInterface } from './Interfaces/SessionInterface';
import { Session } from './Models/Session';

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


    // ------> METODO REGISTRO YA FUNCIONA
    registerUser(user: Users) {
        this.formDate = this.datePipe.transform(this.date, 'mediumDate')!;
        user.fechaRegistro = this.formDate;

        return this.http.post('http://localhost:5000/registerUser', user, requestOptions)
            .subscribe();
    }

    Login(email: string, password: string) {
        return this.http.post<SessionInterface>('http://localhost:5000/login', { email: email, password: password }, requestOptions)
            .subscribe(resp => {
                Session.getInstance().setFlagLog(resp.flagLog);
                if(resp.flagLog) {
                    Session.getInstance().setUser(resp.user);
                    Session.getInstance().setEmail(resp.email);
                    console.log('logueado');
                }
                else
                {
                    console.log('no logueado');
                }
            });
    }
}
