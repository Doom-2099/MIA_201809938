import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { LoginComponentComponent } from './LoginComponent/LoginComponent.component';
import { NotFountComponentComponent } from './NotFountComponent/NotFountComponent.component';
import { PageAdminComponent } from './PageAdmin/PageAdmin.component';
import { UsersAdminComponent } from './PageAdmin/UsersAdmin/UsersAdmin.component';
import { NavbarComponent } from './shared/Navbar/Navbar.component';
import { PermisosAdminComponent } from './PageAdmin/PermisosAdmin/PermisosAdmin.component';

@NgModule({
  declarations: [						
    AppComponent,
    LoginComponentComponent,
    NotFountComponentComponent,
    PageAdminComponent,
    NavbarComponent,
    UsersAdminComponent,
    PermisosAdminComponent
   ],
  imports: [
    BrowserModule,
    AppRoutingModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
