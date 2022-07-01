import { Component, OnInit } from '@angular/core';
import * as M from 'materialize-css';
import { Session } from '../Models/Session';
import { Router } from '@angular/router';

@Component({
  selector: 'app-PageUser',
  templateUrl: './PageUser.component.html',
  styleUrls: ['./PageUser.component.css']
})
export class PageUserComponent implements OnInit {

  private flagAccount : boolean = true;
  private flagCollaborator: boolean = false;
  private flagFiles : boolean = false;

  constructor(private router : Router) { }

  ngOnInit() {
    M.AutoInit();

    if(!Session.getInstance().getFlagLog()) {
      this.router.navigate(['Denied']);
    } else {
      // Codigo Necesario
    }
  }

  getFlagAccount() {
    return this.flagAccount;
  }

  getFlagCollaborator() {
    return this.flagCollaborator;
  }

  getFlagFiles() {
    return this.flagFiles;
  }

  flagAccountView() {
    this.flagAccount = true;
    this.flagCollaborator = false;
    this.flagFiles = false;
  }

  flagFilesView() {
    this.flagAccount = false;
    this.flagCollaborator = false;
    this.flagFiles = true;
  }

  flagCollaboratorView() {
    this.flagAccount = false;
    this.flagCollaborator = true;
    this.flagFiles = false;
  }

  Logout() {
    this.router.navigate(['']);
  }

}
