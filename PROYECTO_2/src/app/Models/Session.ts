export class Session {
    private user : string = "";
    private email : string = "";
    private flagLog : boolean = false;
    private static instance:Session = new Session();

    constructor() { }

    static getInstance():Session {
        return this.instance;
    }

    setUser(user:string) {
        this.user = user;
    }

    setEmail(email: string) {
        this.email = email;
    }

    setFlagLog(flagLog:boolean) {
        this.flagLog = flagLog;
    }

    getUser() {
        return this.user;
    }

    getEmail() {
        return this.email;
    }

    getFlagLog() {
        return this.flagLog;
    }
}