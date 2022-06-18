#include "AdminArchivos.h"

void mkfile(string id, string path, bool p, int size, string cont)
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
        disco = fopen(getPathWithName(prt.path).c_str(), "rb+"); // Abrir Disco Copia
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
        disco = fopen(getPathWithName(prt.path).c_str(), "rb+"); // Abrir Disco Copia
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

    TablaInodos inodo;
    fseek(disco, sb.s_inode_start, SEEK_SET);
    fread(&inodo, sizeof(TablaInodos), 1, disco);   // Lectura Primer Inodo
    int lastInode = ftell(disco);

    BloqueCarpeta bc;
    fseek(disco, sb.s_block_start, SEEK_SET);
    fread(&bc, sizeof(BloqueCarpeta), 1, disco);    // Lectura Primer Bloque
    int lastBlock = ftell(disco);

    do
    {
        if (p)
        { 
            if(bc.b_content[0].b_inodo == -1)
            {
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

                for(int i = 0; i < 15; i++)
                {
                    ti.i_block[i] = -1;
                }

                // BLOQUE CON METADATOS
                BloqueCarpeta bcarpeta;
                strcpy(bcarpeta.b_content[0].b_name, partRoute);
                bcarpeta.b_content[0].b_inodo = -1;

                char bit;
                fseek(discoaux, sb.s_bm_block_start, SEEK_SET); // Posicion En BitMap Bloques
                i = 0;
                while (i < sb.s_blocks_count)
                {
                    fread(&bit, sizeof(bit), 1, discoaux);

                    if (bit == '0')
                    {
                        int posBlock = sb.s_block_start + (i * sizeof(BloqueCarpeta));
                        for (int x = 0; i < 12; i++)
                        {
                            if (ti.i_block[x] == -1)
                            {
                                ti.i_block[x] = posBlock;
                                break;
                            }
                        }

                        fseek(discoaux, -sizeof(bit), SEEK_CUR);
                        bit = '1';
                        fwrite(&bit, sizeof(bit), 1, discoaux);
                        bit = '0';
                        sb.s_free_blocks_count--;

                        fseek(discoaux, posBlock, SEEK_SET);
                        fwrite(&bcarpeta, sizeof(BloqueCarpeta), 1, discoaux);
                        break;
                    }
                    i++;
                }

                fseek(discoaux, sb.s_bm_inode_start, SEEK_SET);
                i = 0;
                while (i < sb.s_inodes_count)
                {
                    fread(&bit, sizeof(bit), 1, discoaux);

                    if (bit == '0')
                    {
                        fseek(discoaux, -sizeof(bit), SEEK_CUR);
                        bit = '1';
                        fwrite(&bit, sizeof(bit), 1, discoaux);
                        bit = '0';
                        sb.s_free_inodes_count--;

                        int posInode = sb.s_inode_start + (i * sizeof(TablaInodos));
                        bc.b_content[0].b_inodo = posInode;
                        fseek(discoaux, posInode, SEEK_SET);
                        fwrite(&ti, sizeof(TablaInodos), 1, discoaux);
                        break;
                    }

                    i++;
                }

                fseek(disco, lastBlock, SEEK_SET);
                fseek(disco, -sizeof(BloqueCarpeta), SEEK_CUR);
                fwrite(&bc, sizeof(BloqueCarpeta), 1, disco);
                bc = bcarpeta;
                lastBlock = ftell(disco) + sizeof(BloqueCarpeta);
                partRoute = strtok(NULL, "/");
            }
            else
            {
                fseek(disco, bc.b_content[0].b_inodo, SEEK_SET);
                fread(&inodo, sizeof(TablaInodos), 1, disco);
                lastInode = ftell(disco);

                int x = 0;
                while (x < 12)
                {
                    if(inodo.i_block[x] != -1)
                    {
                        fseek(disco, inodo.i_block[x], SEEK_SET);
                        fread(&bc, sizeof(BloqueCarpeta), 1, disco);
                        if(!strcasecmp(bc.b_content[0].b_name, partRoute))
                        {
                            break;
                        }
                    }

                    x++;
                }

                if(x < 12)
                {
                    partRoute = strtok(NULL, "/");
                }
                else
                {
                    BloqueCarpeta bcarpeta;
                    strcpy(bcarpeta.b_content[0].b_name, partRoute);
                    bcarpeta.b_content[0].b_inodo = -1;

                    x = 0;
                    while (x < 12)
                    {
                        if(inodo.i_block[x] == -1)
                        {
                            char bit;
                            fseek(discoaux, sb.s_bm_block_start, SEEK_SET); // Posicion En BitMap Bloques
                            i = 0;
                            while (i < sb.s_blocks_count)
                            {
                                fread(&bit, sizeof(bit), 1, discoaux);
                                if (bit == '0')
                                {
                                    int posBlock = sb.s_block_start + (i * sizeof(BloqueCarpeta));
                                    fseek(discoaux, -sizeof(bit), SEEK_CUR);
                                    bit = '1';
                                    fwrite(&bit, sizeof(bit), 1, discoaux);
                                    bit = '0';
                                    sb.s_free_blocks_count--;
                                    inodo.i_block[x] = posBlock;
                                    fseek(discoaux, posBlock, SEEK_SET);
                                    fwrite(&bcarpeta, sizeof(BloqueCarpeta), 1, discoaux);
                                    break;
                                }
                                i++;
                            }

                            if(i == sb.s_blocks_count)
                            {
                                cout << "\tERROR NO HAY BLOQUES DISPONIBLES..." << endl;
                                getchar();
                                return;
                            }
                            else
                            {
                                break;  
                            }
                        }
                        x++;
                    }

                    fseek(disco, lastInode, SEEK_SET);
                    fseek(disco, -sizeof(TablaInodos), SEEK_CUR);
                    fwrite(&inodo, sizeof(TablaInodos), 1, disco);
                    partRoute = strtok(NULL, "/");
                }
            }
        }
        else
        {
            if(bc.b_content[0].b_inodo != -1)
            {
                fseek(disco, bc.b_content[0].b_inodo, SEEK_SET);
                fread(&inodo, sizeof(TablaInodos), 1, disco);
                lastInode = ftell(disco);

                int x = 0;
                while (x < 12)
                {
                    if(inodo.i_block[x] != -1)
                    {
                        fseek(disco, inodo.i_block[x], SEEK_SET);
                        fread(&bc, sizeof(BloqueCarpeta), 1, disco);
                        if(!strcasecmp(bc.b_content[0].b_name, partRoute))
                        {
                            break;
                        }
                    }

                    x++;
                }

                if(x < 12)
                {
                    partRoute = strtok(NULL, "/");
                }
                else
                {
                    BloqueCarpeta bcarpeta;
                    strcpy(bcarpeta.b_content[0].b_name, partRoute);
                    bcarpeta.b_content[0].b_inodo = -1;
                    partRoute = strtok(NULL, "/");

                    if(partRoute == NULL)
                    {
                        x = 0;
                        while (x < 12)
                        {
                            if(inodo.i_block[x] == -1)
                            {
                                char bit;
                                fseek(discoaux, sb.s_bm_block_start, SEEK_SET); // Posicion En BitMap Bloques
                                i = 0;
                                while (i < sb.s_blocks_count)
                                {
                                    fread(&bit, sizeof(bit), 1, discoaux);
                                    if (bit == '0')
                                    {
                                        int posBlock = sb.s_block_start + (i * sizeof(BloqueCarpeta));
                                        fseek(discoaux, -sizeof(bit), SEEK_CUR);
                                        bit = '1';
                                        fwrite(&bit, sizeof(bit), 1, discoaux);
                                        bit = '0';
                                        sb.s_free_blocks_count--;
                                        inodo.i_block[x] = posBlock;
                                        fseek(discoaux, posBlock, SEEK_SET);
                                        fwrite(&bcarpeta, sizeof(BloqueCarpeta), 1, discoaux);
                                        break;
                                    }
                                    i++;
                                }

                                if(i == sb.s_blocks_count)
                                {
                                    cout << "\tERROR NO HAY BLOQUES DISPONIBLES..." << endl;
                                    getchar();
                                    return;
                                }
                                else
                                {
                                    break;  
                                }
                            }
                            x++;
                        }
                        
                        fseek(disco, lastInode, SEEK_SET);
                        fseek(disco, -sizeof(TablaInodos), SEEK_CUR);
                        fwrite(&inodo, sizeof(TablaInodos), 1, disco);
                    }
                    else
                    {
                        cout << "\tERROR LA RUTA ES INCORRECTA..." << endl;
                        getchar();
                        return;
                    }
                }
            }
            else
            {
                BloqueCarpeta bcarpeta;
                strcpy(bcarpeta.b_content[0].b_name, partRoute);
                bcarpeta.b_content[0].b_inodo = -1;
                partRoute = strtok(NULL, "/");

                if(partRoute == NULL)
                {
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

                    for(int i = 0; i < 15; i++)
                    {
                        ti.i_block[i] = -1;
                    }
                    
                    char bit;
                    fseek(discoaux, sb.s_bm_block_start, SEEK_SET); // Posicion En BitMap Bloques
                    i = 0;
                    while (i < sb.s_blocks_count)
                    {
                        fread(&bit, sizeof(bit), 1, discoaux);

                        if (bit == '0')
                        {
                            int posBlock = sb.s_block_start + (i * sizeof(BloqueCarpeta));
                            ti.i_block[0] = posBlock;
                            fseek(discoaux, -sizeof(bit), SEEK_CUR);
                            bit = '1';
                            fwrite(&bit, sizeof(bit), 1, discoaux);
                            bit = '0';
                            sb.s_free_blocks_count--;
                            fseek(discoaux, posBlock, SEEK_SET);
                            fwrite(&bcarpeta, sizeof(BloqueCarpeta), 1, discoaux);
                            break;
                        }
                        i++;
                    }

                    fseek(discoaux, sb.s_bm_inode_start, SEEK_SET);
                    i = 0;
                    while (i < sb.s_inodes_count)
                    {
                        fread(&bit, sizeof(bit), 1, discoaux);

                        if (bit == '0')
                        {
                            fseek(discoaux, -sizeof(bit), SEEK_CUR);
                            bit = '1';
                            fwrite(&bit, sizeof(bit), 1, discoaux);
                            bit = '0';
                            sb.s_free_inodes_count--;

                            int posInode = sb.s_inode_start + (i * sizeof(TablaInodos));
                            bc.b_content[0].b_inodo = posInode;
                            fseek(discoaux, posInode, SEEK_SET);
                            fwrite(&ti, sizeof(TablaInodos), 1, discoaux);
                            break;
                        }

                        i++;
                    }

                    fseek(disco, lastBlock, SEEK_SET);
                    fseek(disco, -sizeof(BloqueCarpeta), SEEK_CUR);
                    fwrite(&bc, sizeof(BloqueCarpeta), 1, disco);
                    lastBlock = ftell(disco) + sizeof(BloqueCarpeta);
                    partRoute = strtok(NULL, "/");
                }
                else
                {
                    cout << "\tERROR LA RUTA ES INCORRECTA..." << endl;
                    getchar();
                    return;
                }
            }
        }
    } while (partRoute != NULL);

    fseek(disco, prt.part_start, SEEK_SET);
    fwrite(&sb, sizeof(SuperBloque), 1, disco);

    Journal j;
    j.journal_contenido = '1';
    
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
    auto str = oss.str();
    strcpy(j.journal_fecha, str.c_str());

    strcpy(j.journal_nombre, path.c_str());
    j.journal_tipo = '1';
    j.journal_tipo_op = Operation::NEW_CARPET;
    j.next = -1;

    Journal journal;

    while (true)
    {
        fread(&journal, sizeof(Journal), 1, disco);
        if(journal.next == -1)
        {
            fwrite(&j, sizeof(Journal), 1, disco);
            break;
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
