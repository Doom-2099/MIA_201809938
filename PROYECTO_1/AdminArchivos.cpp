#include "AdminArchivos.h"

void mkfile(string id, string path, bool p, int size, string cont)
{
    /* vector<PrtMount> list = getList();

    int i = 0;
    PrtMount prt;
    bool raid = false;
    while (i < list.size())
    {
        if (!strcmp(id.c_str(), list[i].id.c_str()))
        {
            prt = list[i];
            break;
        }
        i++;
    }

    if (i == list.size())
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
        disco = fopen(getPathWithName(prt.path).c_str(), "rb+");    // Abrir Disco Copia
        discoaux = fopen(getPathWithName(prt.path).c_str(), "rb+"); // FILE auxiliar

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
    fseek(disco, prt.part_start, SEEK_SET);    // Principio De La Particion
    fread(&sb, sizeof(SuperBloque), 1, disco); // Leer Superbloque

    // ESCRIBIR ARCHIVOS
    if(sb.s_free_blocks_count < 0 || sb.s_free_inodes_count < 0)
    {
        cout << "\tERROR NO HAY ESPACION EN EL SISTEMA DE ARCHIVOS..." << endl;
        getchar();
        return;
    }

    char pathAux[50];
    strcpy(pathAux, path.c_str());
    char *partRoute = strtok(pathAux, "/"); */

    // MKFILE
}

void mkdir(string id, string path, bool p)
{
    vector<PrtMount> list = getList();

    int i = 0;
    PrtMount prt;
    bool raid = false;
    while (i < list.size())
    {
        if (!strcmp(id.c_str(), list[i].id.c_str()))
        {
            prt = list[i];
            break;
        }
        i++;
    }

    if (i == list.size())
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
        disco = fopen(getPathWithName(prt.path).c_str(), "rb+");    // Abrir Disco Copia
        discoaux = fopen(getPathWithName(prt.path).c_str(), "rb+"); // FILE auxiliar

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
    fseek(disco, prt.part_start, SEEK_SET);    // Principio De La Particion
    fread(&sb, sizeof(SuperBloque), 1, disco); // Leer Superbloque

    if ((sb.s_free_blocks_count < 0) || (sb.s_free_inodes_count < 0))
    {
        cout << "\tERROR NO HAY ESPACIO EN EL SISTEMA DE ARCHIVOS..." << endl;
        getchar();
        return;
    }

    char pathAux[50];
    strcpy(pathAux, path.c_str());
    char *partRoute = strtok(pathAux, "/");

    TablaInodos inodo; // Variable Del Metodo
    fseek(disco, sb.s_inode_start, SEEK_SET);
    fread(&inodo, sizeof(TablaInodos), 1, disco); // Lectura Primer Inodo
    int lastInode = ftell(disco);

    BloqueCarpeta bloqueC; // Variable Del Metodo
    fseek(disco, sb.s_block_start, SEEK_SET);
    fread(&bloqueC, sizeof(BloqueCarpeta), 1, disco); // Lectura Primer Bloque
    int lastBlock = ftell(disco);
    int posBlock;
    int posInode;
    string date = getDate();

    // Metodo MKDIR
    if (p)
    {
        // RUTA COMPLETA INEXISTENTE
        while (partRoute != NULL)
        {
            // Buscar Carpeta
            if (bloqueC.b_content[0].b_inodo != -1)
            {
                fseek(disco, bloqueC.b_content[0].b_inodo, SEEK_SET);
                fread(&inodo, sizeof(TablaInodos), 1, disco);
                lastInode = ftell(disco);

                for (i = 0; i < 12; i++)
                {
                    if (inodo.i_block[i] != -1)
                    {
                        fseek(disco, inodo.i_block[i], SEEK_SET);
                        fread(&bloqueC, sizeof(BloqueCarpeta), 1, disco);
                        if (!strcasecmp(bloqueC.b_content[0].b_name, partRoute))
                        {
                            lastBlock = ftell(disco);
                            break;
                        }
                    }
                }

                if (i < 12)
                {
                    // SI EXISTE LA CARPETA
                    partRoute = strtok(NULL, "/");
                    continue;
                }
                else
                {
                    // NO EXISTE LA CARPETA
                    i = 0;
                    while (i < 12)
                    {
                        if (inodo.i_block[i] == -1)
                        {
                            break;
                        }
                        i++;
                    }

                    if (i == 12)
                    {
                        cout << "\tERROR, LA CARPETA NO PUEDE TENER OTRA CARPETA ANIDADA..." << endl;
                        getchar();
                        fclose(disco);
                        fclose(discoaux);
                        return;
                    }

                    BloqueCarpeta newFolder;
                    strcpy(newFolder.b_content[0].b_name, partRoute);
                    newFolder.b_content[0].b_inodo = -1;

                    char bit;
                    fseek(discoaux, sb.s_bm_block_start, SEEK_SET);
                    for (int x = 0; x < sb.s_blocks_count; x++)
                    {
                        fread(&bit, sizeof(bit), 1, discoaux);
                        if (bit == '0')
                        {
                            posBlock = sb.s_block_start + (x * sizeof(BloqueCarpeta));
                            fseek(discoaux, -sizeof(bit), SEEK_CUR);
                            bit = '1';
                            fwrite(&bit, sizeof(bit), 1, discoaux);
                            inodo.i_block[i] = posBlock;
                            fseek(disco, posBlock, SEEK_SET);
                            fwrite(&newFolder, sizeof(BloqueCarpeta), 1, disco);
                            lastBlock = ftell(disco);
                            fseek(disco, lastInode, SEEK_SET);
                            fseek(disco, -sizeof(TablaInodos), SEEK_CUR);
                            fwrite(&inodo, sizeof(TablaInodos), 1, disco);
                            bloqueC = newFolder;
                            partRoute = strtok(NULL, "/");
                            break;
                        }
                    }
                }
            }
            else
            {
                // Nuevo Inodo Y Nueva Carpeta
                TablaInodos newInodo;
                newInodo.i_uid = 777;
                newInodo.i_gid = 777;
                newInodo.i_type = '0';
                newInodo.i_size = -1;

                strcpy(newInodo.i_atime, date.c_str());
                strcpy(newInodo.i_ctime, date.c_str());
                strcpy(newInodo.i_mtime, date.c_str());

                for (int x = 0; x < 15; x++)
                {
                    newInodo.i_block[x] = -1;
                }

                BloqueCarpeta newFolder;
                newFolder.b_content[0].b_inodo = -1;
                strcpy(newFolder.b_content[0].b_name, partRoute);

                char bit;
                fseek(discoaux, sb.s_bm_block_start, SEEK_SET);
                for (int x = 0; x < sb.s_blocks_count; x++)
                {
                    fread(&bit, sizeof(bit), 1, discoaux);
                    if (bit == '0')
                    {
                        posBlock = sb.s_block_start + (x * sizeof(BloqueCarpeta));
                        fseek(discoaux, -sizeof(bit), SEEK_CUR);
                        bit = '1';
                        fwrite(&bit, sizeof(bit), 1, discoaux);
                        newInodo.i_block[0] = posBlock;
                        fseek(disco, posBlock, SEEK_SET);
                        fwrite(&newFolder, sizeof(BloqueCarpeta), 1, disco);
                        break;
                    }
                }

                fseek(discoaux, sb.s_bm_inode_start, SEEK_SET);
                for (int x = 0; x < sb.s_inodes_count; x++)
                {
                    fread(&bit, sizeof(bit), 1, discoaux);
                    if (bit == '0')
                    {
                        posInode = sb.s_inode_start + (x * sizeof(TablaInodos));
                        fseek(discoaux, -sizeof(bit), SEEK_CUR);
                        bit = '1';
                        fwrite(&bit, sizeof(bit), 1, discoaux);
                        bloqueC.b_content[0].b_inodo = posInode;
                        fseek(disco, posInode, SEEK_SET);
                        fwrite(&newInodo, sizeof(TablaInodos), 1, disco);
                        fseek(disco, lastBlock, SEEK_SET);
                        fseek(disco, -sizeof(BloqueCarpeta), SEEK_CUR);
                        fwrite(&bloqueC, sizeof(BloqueCarpeta), 1, disco);
                        bloqueC = newFolder;
                        lastBlock = posBlock + sizeof(BloqueCarpeta);
                        partRoute = strtok(NULL, "/");
                        break;
                    }
                }
            }
        }
    }
    else
    {
        // RUTA COMPLETA EXISTENTE
        while (true)
        {
            if (bloqueC.b_content[0].b_inodo == -1)
            {
                string nombre = partRoute;
                partRoute = strtok(NULL, "/");
                if (partRoute != NULL)
                {
                    cout << "\tERROR LA RUTA ESPECIFICADA NO EXISTE..." << endl;
                    getchar();
                    fclose(disco);
                    fclose(discoaux);
                    return;
                }
                else
                {
                    // Nuevo Inodo Y Nueva Carpeta
                    TablaInodos newInodo;
                    newInodo.i_uid = 777;
                    newInodo.i_gid = 777;
                    newInodo.i_type = '0';
                    newInodo.i_size = -1;

                    strcpy(newInodo.i_atime, date.c_str());
                    strcpy(newInodo.i_ctime, date.c_str());
                    strcpy(newInodo.i_mtime, date.c_str());

                    for (int x = 0; x < 15; x++)
                    {
                        newInodo.i_block[x] = -1;
                    }

                    BloqueCarpeta newFolder;
                    newFolder.b_content[0].b_inodo = -1;
                    strcpy(newFolder.b_content[0].b_name, nombre.c_str());

                    char bit;
                    fseek(discoaux, sb.s_bm_block_start, SEEK_SET);
                    for (int x = 0; x < sb.s_blocks_count; x++)
                    {
                        fread(&bit, sizeof(bit), 1, discoaux);
                        if (bit == '0')
                        {
                            posBlock = sb.s_block_start + (x * sizeof(BloqueCarpeta));
                            fseek(discoaux, -sizeof(bit), SEEK_CUR);
                            bit = '1';
                            fwrite(&bit, sizeof(bit), 1, discoaux);
                            newInodo.i_block[0] = posBlock;
                            fseek(disco, posBlock, SEEK_SET);
                            fwrite(&newFolder, sizeof(BloqueCarpeta), 1, disco);
                            break;
                        }
                    }

                    fseek(discoaux, sb.s_bm_inode_start, SEEK_SET);
                    for (int x = 0; x < sb.s_inodes_count; x++)
                    {
                        fread(&bit, sizeof(bit), 1, discoaux);
                        if (bit == '0')
                        {
                            posInode = sb.s_inode_start + (x * sizeof(TablaInodos));
                            fseek(discoaux, -sizeof(bit), SEEK_CUR);
                            bit = '1';
                            fwrite(&bit, sizeof(bit), 1, discoaux);
                            bloqueC.b_content[0].b_inodo = posInode;
                            fseek(disco, posInode, SEEK_SET);
                            fwrite(&newInodo, sizeof(TablaInodos), 1, disco);
                            fseek(disco, lastBlock, SEEK_SET);
                            fseek(disco, -sizeof(BloqueCarpeta), SEEK_CUR);
                            fwrite(&bloqueC, sizeof(BloqueCarpeta), 1, disco);
                            bloqueC = newFolder;
                            lastBlock = posBlock + sizeof(BloqueCarpeta);
                            fclose(disco);
                            fclose(discoaux);
                            return;
                        }
                    }
                }
            }
            else
            {
                fseek(disco, bloqueC.b_content[0].b_inodo, SEEK_SET);
                fread(&inodo, sizeof(TablaInodos), 1, disco);
                lastInode = ftell(disco);

                i = 0;
                while (i < 12)
                {
                    if (inodo.i_block[i] != -1)
                    {
                        fseek(disco, inodo.i_block[i], SEEK_SET);
                        fread(&bloqueC, sizeof(BloqueCarpeta), 1, disco);
                        if (!strcasecmp(partRoute, bloqueC.b_content[0].b_name))
                        {
                            lastBlock = ftell(disco);
                            break;
                        }
                    }

                    i++;
                }

                if (i < 12)
                {
                    string nombre = partRoute;
                    partRoute = strtok(NULL, "/");
                    if (partRoute == NULL)
                    {
                        char xd[50];
                        strcpy(xd, nombre.c_str());
                        partRoute = strtok(xd, "/");
                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    string nombre = partRoute;
                    partRoute = strtok(NULL, "/");
                    if (partRoute == NULL)
                    {
                        i = 0;
                        while (i < 12)
                        {
                            if (inodo.i_block[i] == -1)
                            {
                                break;
                            }
                            i++;
                        }

                        if (i == 12)
                        {
                            cout << "\tERROR, LA CARPETA NO PUEDE TENER OTRA CARPETA ANIDADA..." << endl;
                            getchar();
                            fclose(disco);
                            fclose(discoaux);
                            return;
                        }

                        BloqueCarpeta newFolder;
                        strcpy(newFolder.b_content[0].b_name, nombre.c_str());
                        newFolder.b_content[0].b_inodo = -1;

                        char bit;
                        fseek(discoaux, sb.s_bm_block_start, SEEK_SET);
                        for (int x = 0; x < sb.s_blocks_count; x++)
                        {
                            fread(&bit, sizeof(bit), 1, discoaux);
                            if (bit == '0')
                            {
                                posBlock = sb.s_block_start + (x * sizeof(BloqueCarpeta));
                                fseek(discoaux, -sizeof(bit), SEEK_CUR);
                                bit = '1';
                                fwrite(&bit, sizeof(bit), 1, discoaux);
                                inodo.i_block[i] = posBlock;
                                fseek(disco, posBlock, SEEK_SET);
                                fwrite(&newFolder, sizeof(BloqueCarpeta), 1, disco);
                                fseek(disco, lastInode, SEEK_SET);
                                fseek(disco, -sizeof(TablaInodos), SEEK_CUR);
                                fwrite(&inodo, sizeof(TablaInodos), 1, disco);
                                bloqueC = newFolder;
                                partRoute = strtok(NULL, "/");
                                fclose(disco);
                                fclose(discoaux);
                                return;
                            }
                        }
                    }
                    else
                    {
                        cout << "\tERROR LA RUTA INGRESADA ES INCORRECTA..." << endl;
                        getchar();
                        fclose(disco);
                        fclose(discoaux);
                        return;
                    }
                }
            }
        }
    }

    fclose(disco);
    fclose(discoaux);

    if (raid)
    {
        string cmd = "cp -a ";
        cmd.append(prt.path);
        cmd.append(" ");
        cmd.append(getPathWithName(prt.path));
        system(cmd.c_str());
    }
}

void synchronice(string id, string path)
{
    // Comando Synchronice
    vector<PrtMount> list = getList();

    int i = 0;
    PrtMount prt;
    bool raid = false;
    while (i < list.size())
    {
        if (!strcmp(id.c_str(), list[i].id.c_str()))
        {
            prt = list[i];
            break;
        }
        i++;
    }

    if (i == list.size())
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
        disco = fopen(getPathWithName(prt.path).c_str(), "rb+");    // Abrir Disco Copia
        discoaux = fopen(getPathWithName(prt.path).c_str(), "rb+"); // FILE auxiliar

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
    fseek(disco, prt.part_start, SEEK_SET);    // Principio De La Particion
    fread(&sb, sizeof(SuperBloque), 1, disco); // Leer Superbloque

    TablaInodos inodo; // Variable Del Metodo
    fseek(disco, sb.s_inode_start, SEEK_SET);
    fread(&inodo, sizeof(TablaInodos), 1, disco); // Lectura Primer Inodo
    int lastInode = ftell(disco);

    BloqueCarpeta bloqueC; // Variable Del Metodo
    fseek(disco, sb.s_block_start, SEEK_SET);
    fread(&bloqueC, sizeof(BloqueCarpeta), 1, disco); // Lectura Primer Bloque

   string content = constructJson(disco, sb.s_inode_start);

    FILE *json = NULL;
    json = fopen("/home/jorge/Escritorio/MIA_201809938/PROYECTO_2/Server/Files/JFiles.json", "w+");
    if (json == NULL)
    {
        // ERROR
        cout << "\tERROR NO SE PUDO HACER EL SYNCRONICE DEL DISCO..." << endl;
        getchar();
        return;
    }
    else
    {
        fputs(content.c_str(), json);
        fclose(json);
        cout << "\tEL ARCHIVO DE SINCRONIZACION SE HA GENERADO CORRECTAMENTE..." << endl;
        getchar();
        return;
    }

}

void rm(string id, vector<string> files)
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
