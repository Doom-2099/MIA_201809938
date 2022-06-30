import { Component, OnInit } from '@angular/core';
import * as M from 'materialize-css';

@Component({
  selector: 'app-FilesUser',
  templateUrl: './FilesUser.component.html',
  styleUrls: ['./FilesUser.component.css']
})
export class FilesUserComponent implements OnInit {

  constructor() { }

  ngOnInit() {
    M.AutoInit();
  }

}
