import { Component } from '@angular/core';

@Component({
  selector: 'app-NotFountComponent',
  templateUrl: './NotFountComponent.component.html',
  styleUrls: ['./NotFountComponent.component.css']
})
export class NotFountComponentComponent {

  resources() {
    alert('Cargando Resource');
  }

}
