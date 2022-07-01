import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-NotFountComponent',
  templateUrl: './NotFountComponent.component.html',
  styleUrls: ['./NotFountComponent.component.css']
})
export class NotFountComponentComponent implements OnInit {

  constructor(private router: Router) { }

  ngOnInit(): void {
    
  }

  home() {
    this.router.navigate(['']);
  }
}
