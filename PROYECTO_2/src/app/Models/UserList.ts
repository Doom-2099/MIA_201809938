import { Users } from '../Interfaces/UserInterface';

export class UserList {

    private listaUsers: Users[] = [];
    private static instance: UserList = new UserList();

    constructor() { }

    static getInstance(): UserList {
        return this.instance;
    }

    addUser(user: Users) {
        this.listaUsers.push(user);
    }

    clearList() {
        this.listaUsers = [];
    }
}