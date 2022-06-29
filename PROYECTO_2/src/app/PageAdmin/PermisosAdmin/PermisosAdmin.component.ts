import { Component, OnInit } from '@angular/core';
import * as M from 'materialize-css';

declare var $:any;

@Component({
  selector: 'app-PermisosAdmin',
  templateUrl: './PermisosAdmin.component.html',
  styleUrls: ['./PermisosAdmin.component.css']
})
export class PermisosAdminComponent implements OnInit {

  ngOnInit() {
    M.AutoInit();

    $(function() {
      $('#jstree_demo_div').jstree();
    });

    $('#jstree_demo_div').on('changed.jstree', function(e:any, data:any) {
      console.log(data.selected);
    });

  }

  // Hacer Peticion Para Arbol Y Archivos

}
