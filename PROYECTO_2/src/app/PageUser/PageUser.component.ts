import { Component, OnInit } from '@angular/core';
import * as M from 'materialize-css';

@Component({
  selector: 'app-PageUser',
  templateUrl: './PageUser.component.html',
  styleUrls: ['./PageUser.component.css']
})
export class PageUserComponent implements OnInit {

  private flagAccount : boolean = true;
  private flagCollaborator: boolean = false;
  private flagFiles : boolean = false;

  constructor() { }

  ngOnInit() {
    M.AutoInit();
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

}
