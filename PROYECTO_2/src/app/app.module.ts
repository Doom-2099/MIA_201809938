import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { HttpClientModule } from '@angular/common/http';
import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { LoginComponentComponent } from './LoginComponent/LoginComponent.component';
import { NotFountComponentComponent } from './NotFountComponent/NotFountComponent.component';
import { PageAdminComponent } from './PageAdmin/PageAdmin.component';
import { NavbarComponent } from './shared/Navbar/Navbar.component';
import { UsersAdminComponent } from './UsersAdmin/UsersAdmin.component';
import { PermisosAdminComponent } from './PermisosAdmin/PermisosAdmin.component';
import { PageUserComponent } from './PageUser/PageUser.component';
import { AccountUserComponent } from './AccountUser/AccountUser.component';
import { CollaboratorUserComponent } from './CollaboratorUser/CollaboratorUser.component';
import { FilesUserComponent } from './FilesUser/FilesUser.component';
import { ResetPasswordComponent } from './ResetPassword/ResetPassword.component';
import { DeniedPageComponent } from './DeniedPage/DeniedPage.component';
import { FormsModule } from '@angular/forms';

@NgModule({
  declarations: [												
    AppComponent,
    LoginComponentComponent,
    NotFountComponentComponent,
    PageAdminComponent,
    NavbarComponent,
    UsersAdminComponent,
    PermisosAdminComponent,
    PageUserComponent,
    AccountUserComponent,
    CollaboratorUserComponent,
    FilesUserComponent,
    ResetPasswordComponent,
    DeniedPageComponent
   ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    FormsModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
