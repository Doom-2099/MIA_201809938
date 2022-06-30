import { Component, OnInit } from '@angular/core';
import * as M from 'materialize-css';

@Component({
  selector: 'app-CollaboratorUser',
  templateUrl: './CollaboratorUser.component.html',
  styleUrls: ['./CollaboratorUser.component.css']
})
export class CollaboratorUserComponent implements OnInit {

  constructor() { }

  ngOnInit() {
    M.AutoInit();
  }

}
