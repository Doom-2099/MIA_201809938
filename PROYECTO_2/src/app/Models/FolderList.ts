import { Folders } from '../Interfaces/FoldersInterface';

export class FolderList {
    private listaFolders:Folders[] = [];
    private static instance :FolderList = new FolderList();

    constructor() { }

    static getInstance():FolderList {
        return this.instance;
    }

    addFolder(folder:Folders) {
        this.listaFolders.push(folder);
    }

    clearList() {
        this.listaFolders = [];
    }

    getList() {
        return this.listaFolders;
    }

    editFolder(index:number, folder:Folders) {
        this.listaFolders[index] = folder;
    }

    getFoldersUser(user:string) : Folders[] {
        var folders:Folders[] = [];

        this.listaFolders.forEach((folder) => {
            if(folder.propietario == user) {
                folders.push(folder);
            }
        })

        return folders;
    }
}