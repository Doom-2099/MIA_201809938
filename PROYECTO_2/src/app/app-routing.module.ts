import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { DeniedPageComponent } from './DeniedPage/DeniedPage.component';
import { LoginComponentComponent } from './LoginComponent/LoginComponent.component';
import { NotFountComponentComponent } from './NotFountComponent/NotFountComponent.component';
import { PageAdminComponent } from './PageAdmin/PageAdmin.component';
import { PageUserComponent } from './PageUser/PageUser.component';
import { ResetPasswordComponent } from './ResetPassword/ResetPassword.component';

const routes: Routes = [
  { path: '', component: LoginComponentComponent },
  { path: 'Administrator', component: PageAdminComponent },
  { path: 'User', component: PageUserComponent },
  { path: 'Reset', component: ResetPasswordComponent },
  { path: 'Denied', component: DeniedPageComponent },
  { path: '**', component: NotFountComponentComponent }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
