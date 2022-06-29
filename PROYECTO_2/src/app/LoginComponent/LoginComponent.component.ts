import { Component, OnInit } from '@angular/core';
import * as M from 'materialize-css';

@Component({
  selector: 'app-LoginComponent',
  templateUrl: './LoginComponent.component.html',
  styleUrls: ['./LoginComponent.component.css']
})
export class LoginComponentComponent implements OnInit {

  ngOnInit(): void {
    M.AutoInit();
  }
}

/* ejecutar(codigo: any): Observable<any> {
  const httpOptions = {
    headers: new HttpHeaders({
      'Content-Type': 'application/json',
    }),
  };
  return this.http.post<any>(baseURL + 'analizar', codigo, httpOptions);
} */


/* import { Injectable } from '@angular/core';
import { Http, RequestOptions, Headers } from '@angular/http';
import { Router } from '@angular/router';
import 'rxjs/add/operator/map';

@Injectable()
export class CitaService {
  uri = 'http://localhost:3000/api/cita/';
  citas:Array<any> = [];
  
  constructor(private http: Http,private router:Router) {}
  getCitas() {
    let usuario = localStorage.getItem('idUsuario')
    let uri2 = this.uri+'user/'+ usuario;
    return this.http.get(uri2).map(response=>{
            this.citas=response.json();
      });
  }

  addCita(data:any){
    let json = JSON.stringify(data);
    let final = JSON.parse(json);
    console.log(json);
    return this.http.post(this.uri,final).map(res => {
      return res.json();
    });
  }

  updateCita(data:any){
    let json = JSON.parse(JSON.stringify(data));
    return this.http.put(this.uri+data.idContacto, json).map(res => {
      return res.json();
    });
  }

  eliminarCita(id:any){
    return this.http.delete(this.uri+id).map(res => {
      return res.json();
    });
  } */


/*   import { Component, OnInit } from '@angular/core';
import { CitaService } from '../../../services/cita.service';
import { ContactoService } from '../../../services/contacto.service'

@Component({
  selector: 'app-cita',
  templateUrl: './cita.component.html',
  styleUrls: ['./cita.component.css']
})
export class CitaComponent implements OnInit {
  private model = {
    idCita: 0,
    fecha: '',
    descripcion: '',
    idUsuario: localStorage.getItem('idUsuario'),
    idContacto: 0,
    lugar: '',
    nombre: '',
    apellido: ''
  }
  constructor(private citaService:CitaService, private contactoService:ContactoService) { }

  ngOnInit() {
    this.citaService.getCitas().subscribe();
    this.contactoService.getContactos().subscribe();
  }

  private agregar() {
    this.citaService.addCita(this.model).subscribe(res => {
      this.resetForm();
      this.citaService.getCitas().subscribe();
    });
  }

  private editar() {
    this.citaService.updateCita(this.model).subscribe(res => {
      this.resetForm();
      this.citaService.getCitas().subscribe();
    })
  }

  private eliminar() {
    this.citaService.eliminarCita(this.model.idCita).subscribe(res => {
      this.resetForm();
      this.citaService.getCitas().subscribe();
    })
  }

  private cargar(data:any) {
    this.model.idCita = data.idCita;
    this.model.fecha = data.fecha;
    this.model.descripcion = data.descripcion;
    this.model.idContacto = data.idContacto;
    this.model.lugar = data.lugar;
    this.model.nombre = data.nombre;
    this.model.apellido = data.apellido;
  }

  private resetForm() {
    this.model.idCita = 0;
    this.model.fecha = '';
    this.model.descripcion = '';
    this.model.idContacto = 0;
    this.model.lugar = '';
  }

} */