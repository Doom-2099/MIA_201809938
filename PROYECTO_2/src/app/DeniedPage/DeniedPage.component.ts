import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-DeniedPage',
  templateUrl: './DeniedPage.component.html',
  styleUrls: ['./DeniedPage.component.css']
})
export class DeniedPageComponent implements OnInit {

  constructor(private router : Router) { }

  ngOnInit() {
  }

  home() {
    this.router.navigate(['']);
  }

}
