#include "AdminArchivos.h"

void mkfile(string id, string path, bool p, int size, string cont)
{
    /* code */
}

void mkdir(string id, string path, bool p)
{
    vector<PrtMount> list = getList();

    int i = 0;
    PrtMount prt;
    bool raid = false;
    while (i < list.size())
    {
        if(!strcmp(id.c_str(), list[i].id.c_str()))
        {
            prt = list[i];
            break;
        }
        i++;
    }

    if(i == list.size())
    {
        cout << "\tERROR LA PARTICION INDICADA NO SE ENCUENTRA MONTADA..." << endl;
        getchar();
        return;
    }

    FILE *disco;
    FILE *discoaux;
    disco = fopen(prt.path.c_str(), "rb+");
    discoaux = fopen(prt.path.c_str(), "rb+");

    if (disco == NULL)
    {
        disco = fopen(getPathWithName(prt.path).c_str(), "rb+"); // Abrir Disco Copia
        discoaux = fopen(getPathWithName(prt.path).c_str(), "rb+");

        if (disco == NULL)
        {
            cout << "\tERROR LA RUTA ES INCORRECTA..." << endl;
            getchar();
            return;
        }
    }
    else
    {
        raid = true;
    }

    SuperBloque sb;
    fseek(disco, prt.part_start, SEEK_SET);         // Principio De La Particion
    fread(&sb, sizeof(SuperBloque), 1, disco);      // Leer Superbloque

    BloqueCarpeta carpeta;
    TablaInodos inodo;                     
    fseek(disco, sb.s_inode_start, SEEK_SET);   
    fread(&inodo, sizeof(TablaInodos), 1, disco);  // Recuperar Primer Inodo

    //**************************************************************
    //**************************************************************
    //**************************************************************
    // INODO CON METADATOS
    TablaInodos ti;
    ti.i_uid = 777;
    ti.i_gid = 777;
    ti.i_size = -1;
    ti.i_type = '0';

    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
    auto str = oss.str();
    strcpy(ti.i_atime, str.c_str());
    strcpy(ti.i_ctime, str.c_str());
    strcpy(ti.i_mtime, str.c_str());
    //**************************************************************
    //**************************************************************
    //**************************************************************

    // Enlace Padre E Hijo
    /* ti.i_block[0] = sb.s_block_start;

    for(int i = 1; i < 15; i++)
    {
        ti.i_block[i] = -1;
    } */

    if(p)
    {

    }
    else
    {
        char ruta[100];
        string nameCarpet;
        strcpy(ruta, path.c_str());
        char *partRuta = strtok(ruta, "/");

        while (partRuta != NULL)
        {
            nameCarpet = partRuta;
            partRuta = strtok(NULL, "/");
            // Moverse Dentro De Inodos Y Bloques
        }

        
        


        Content c;
        strcpy(c.b_name, nameCarpet.c_str());
        c.b_inodo = -1;

        BloqueCarpeta bc;
        bc.b_content[0] = c;

        
    }


}

void rm(string id, vector<string>files)
{
    cout << "\tCOMANDO RM" << endl;
}

void cp(string id, string path, string dest, string iddest)
{
    cout << "\tCOMANDO CP" << endl;
}

void find(string id, string path, string name)
{
    cout << "\tCOMANDO FIND" << endl;
}

void ls(string id, string path, char order)
{
    cout << "\tCOMANDO LS" << endl;
}

void cat(string id, vector<string> filen)
{
    cout << "\tCOMANDO CAT" << endl;
}

void recovery(string id)
{
    cout << "\tCOMANDO RECOVERY" << endl;
}

void loss(string id)
{
    cout << "\tCOMANDO LOSS" << endl;
}
