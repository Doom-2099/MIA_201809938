import { Component, OnInit } from '@angular/core';
import * as M from 'materialize-css';

@Component({
  selector: 'app-AccountUser',
  templateUrl: './AccountUser.component.html',
  styleUrls: ['./AccountUser.component.css']
})
export class AccountUserComponent implements OnInit {

  constructor() { }

  ngOnInit() {
    M.AutoInit();
  }

}
