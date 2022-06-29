import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { LoginComponentComponent } from './LoginComponent/LoginComponent.component';
import { NotFountComponentComponent } from './NotFountComponent/NotFountComponent.component';
import { PageAdminComponent } from './PageAdmin/PageAdmin.component';

const routes: Routes = [
  { path: '', component: LoginComponentComponent },
  { path: 'Administrator', component: PageAdminComponent},
  { path: '**', component: NotFountComponentComponent }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
