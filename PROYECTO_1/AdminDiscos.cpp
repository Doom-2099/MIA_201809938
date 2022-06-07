#include "AdminDiscos.h"

void mkdisk(int size, string path, string name, char unit)
{
    char relleno[1024];
    string cmd = "mkdir -p ";
    cmd.append(path);
    system(cmd.c_str());

    for (int i = 0; i < 1024; i++)
    {
        relleno[i] = '\0';
    }
    
    path.append(name);

    FILE * disco;
    disco = fopen(path.c_str(), "wb+");

    if (disco != NULL)
    {
        MBR mbr;

        auto t = time(nullptr);
        auto tm = *localtime(&t);
        ostringstream oss;
        oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
        auto str = oss.str();
        strcpy(mbr.mbr_fecha_creacion, str.c_str());

        mbr.mbr_disk_signature = rand() % 1001;
        mbr.mbr_size = calculate_size(size, unit);

        mbr.mbr_partition_1.part_status = 'n';
        mbr.mbr_partition_1.part_fit = 'n';
        mbr.mbr_partition_1.part_type = 'n';
        mbr.mbr_partition_1.part_start = 0;
        mbr.mbr_partition_1.part_size = 0;
        strcpy(mbr.mbr_partition_1.part_name, "nn");

        mbr.mbr_partition_2.part_status = 'n';
        mbr.mbr_partition_2.part_fit = 'n';
        mbr.mbr_partition_2.part_type = 'n';
        mbr.mbr_partition_2.part_start = 0;
        mbr.mbr_partition_2.part_size = 0;
        strcpy(mbr.mbr_partition_2.part_name, "nn");

        mbr.mbr_partition_3.part_status = 'n';
        mbr.mbr_partition_3.part_fit = 'n';
        mbr.mbr_partition_3.part_type = 'n';
        mbr.mbr_partition_3.part_start = 0;
        mbr.mbr_partition_3.part_size = 0;
        strcpy(mbr.mbr_partition_3.part_name, "nn");

        mbr.mbr_partition_4.part_status = 'n';
        mbr.mbr_partition_4.part_fit = 'n';
        mbr.mbr_partition_4.part_type = 'n';
        mbr.mbr_partition_4.part_start = 0;
        mbr.mbr_partition_4.part_size = 0;
        strcpy(mbr.mbr_partition_4.part_name, "nn");

        rewind(disco);
        fwrite(&mbr, sizeof(MBR), 1, disco);

        while (ftell(disco) < mbr.mbr_size)
        {
            fwrite(&relleno, sizeof(relleno), 1, disco);
        }

        fclose(disco);
    }
    else
    {
        cout << "\tERROR EL DISCO NO SE HA PODIDO CREAR..." << endl;
        getchar();
        return;
    }
}

void rmdisk(string path)
{
    string op = "";
    if (is_file(path))
    {
        cout << "\tDESEA ELIMINAR EL DISCO UBICADO EN " << path.c_str() << "\n\t[S/N]: ";
        op = cin.get();

        if (op == "S" || op == "s")
        {
            if (remove(path.c_str()) == 0)
            {
                cout << "\tEL DISCO SE HA ELIMINADO CORRECTAMENTE..." << endl;
                getchar();
            }
            else
            {
                cout << "\tERROR EL DISCO NO SE HA PODIDO ELIMINAR..." << endl;
                getchar();
            }
        }
    }
    else
    {
        cout << "\tERROR LA RUTA AL ARCHIVO ES INCORRECTA..." << endl;
        getchar();
    }
}

void fdisk(int size, string path, string name, char unit, char type, char fit)
{
    size = calculate_size(size, unit);
    MBR mbr;
    int aux;
    FILE *disco = NULL;
    disco = fopen(path.c_str(), "rb+");
    if(disco != NULL)
    {
        rewind(disco);
        fread(&mbr, sizeof(MBR), 1, disco);
        aux = ftell(disco);
        fclose(disco);
    }
    else
    {
        cout << "\tERROR LA RUTA ES INCORRECTA..." << endl;
        getchar();
        return;
    }

    vector<Particion> particiones;
    particiones.push_back(mbr.mbr_partition_1);
    particiones.push_back(mbr.mbr_partition_2);
    particiones.push_back(mbr.mbr_partition_3);
    particiones.push_back(mbr.mbr_partition_4);

    //Validacion Nombres Particiones MBR
    for(int i = 0; i < particiones.size(); i++)
    {
        if(!strcmp(particiones[i].part_name, name.c_str()))
        {
            cout << "\tEL NOMBRE PARA LA PARTICION YA EXISTE..." << endl;
            getchar();
            return;
        }
    }

    //Validacion Nombres Extendida
    for(int i = 0; i < particiones.size(); i++)
    {
        if(particiones[i].part_type == 'e')
        {
            EBR ebr;
            disco = fopen(path.c_str(), "rb+");
            fseek(disco, particiones[i].part_start, SEEK_SET);

            while(true)
            {
                fread(&ebr, sizeof(EBR), 1, disco);

                if(!strcmp(ebr.part_name, name.c_str()))
                {
                    fclose(disco);
                    cout << "\tEL NOMBRE PARA LA PARTICION YA EXISTE..." << endl;
                    getchar();
                    return;
                }
                else if(ebr.part_next != -1)
                {
                    fseek(disco, ebr.part_next, SEEK_SET);
                }
                else
                {
                    fclose(disco);
                    break;
                }
            }
        }
    }

    // Acomodar Particiones
    if(type != 'l')
    {
        //Validacion Particiones Extendidas
        for(int i = 0; i < particiones.size(); i++)
        {
            if(particiones[i].part_type == 'e')
            {
                cout << "\tERROR YA EXISTE UNA PARTICION EXTENDIDA..." << endl;
                getchar();
                return;
            }
        }

        vector<BusyBlock> bblocks = getBusyBlocks(particiones);
        vector<FreeBlock> fblocks = getFreeBlocks(bblocks, mbr.mbr_size, aux);

        for(int i = 0; i < fblocks.size(); i++)
        {
            if(size < fblocks[i].size)
            {
                for(int j = 0; j < particiones.size(); j++)
                {
                    if(particiones[j].part_start == 0)
                    {
                        switch (j)
                        {
                            case 0:
                                particiones[j].part_fit = fit;
                                particiones[j].part_status = '0';
                                particiones[j].part_type = type;
                                particiones[j].part_start = fblocks[i].start;
                                particiones[j].part_size = size;
                                strcpy(particiones[j].part_name, name.c_str());
                                mbr.mbr_partition_1 = particiones[j];

                                if(type == 'e')
                                {
                                    EBR ebr;

                                    ebr.part_fit = 'n';
                                    ebr.part_next = -1;
                                    ebr.part_size = 0;
                                    ebr.part_start = 0;
                                    strcpy(ebr.part_name, "nn");

                                    disco = fopen(path.c_str(), "rb+");
                                    fseek(disco, particiones[j].part_start, SEEK_SET);
                                    fwrite(&ebr, sizeof(EBR), 1, disco);
                                    fclose(disco);
                                }

                                break;

                            case 1:
                                particiones[j].part_fit = fit;
                                particiones[j].part_status = '0';
                                particiones[j].part_type = type;
                                particiones[j].part_start = fblocks[i].start;
                                particiones[j].part_size = size;
                                strcpy(particiones[j].part_name, name.c_str());
                                mbr.mbr_partition_2 = particiones[j];

                                if(type == 'e')
                                {
                                    EBR ebr;

                                    ebr.part_fit = 'n';
                                    ebr.part_next = -1;
                                    ebr.part_size = 0;
                                    ebr.part_start = 0;
                                    strcpy(ebr.part_name, "nn");

                                    disco = fopen(path.c_str(), "rb+");
                                    fseek(disco, particiones[j].part_start, SEEK_SET);
                                    fwrite(&ebr, sizeof(EBR), 1, disco);
                                    fclose(disco);
                                }

                                break;

                            case 2:
                                particiones[j].part_fit = fit;
                                particiones[j].part_status = '0';
                                particiones[j].part_type = type;
                                particiones[j].part_start = fblocks[i].start;
                                particiones[j].part_size = size;
                                strcpy(particiones[j].part_name, name.c_str());
                                mbr.mbr_partition_3 = particiones[j];

                                if(type == 'e')
                                {
                                    EBR ebr;

                                    ebr.part_fit = 'n';
                                    ebr.part_next = -1;
                                    ebr.part_size = 0;
                                    ebr.part_start = 0;
                                    strcpy(ebr.part_name, "nn");

                                    disco = fopen(path.c_str(), "rb+");
                                    fseek(disco, particiones[j].part_start, SEEK_SET);
                                    fwrite(&ebr, sizeof(EBR), 1, disco);
                                    fclose(disco);
                                }

                                break;

                            case 3:
                                particiones[j].part_fit = fit;
                                particiones[j].part_status = '0';
                                particiones[j].part_type = type;
                                particiones[j].part_start = fblocks[i].start;
                                particiones[j].part_size = size;
                                strcpy(particiones[j].part_name, name.c_str());
                                mbr.mbr_partition_4 = particiones[j];

                                if(type == 'e')
                                {
                                    EBR ebr;

                                    ebr.part_fit = 'n';
                                    ebr.part_next = -1;
                                    ebr.part_size = 0;
                                    ebr.part_start = 0;
                                    strcpy(ebr.part_name, "nn");

                                    disco = fopen(path.c_str(), "rb+");
                                    fseek(disco, particiones[j].part_start, SEEK_SET);
                                    fwrite(&ebr, sizeof(EBR), 1, disco);
                                    fclose(disco);
                                }

                                break;
                        }

                        disco = fopen(path.c_str(), "rb+");
                        rewind(disco);
                        fwrite(&mbr, sizeof(MBR), 1, disco);
                        fclose(disco);
                        return;
                    }
                }

                cout << "\tERROR NO QUEDAN ESPACIOS PARA PARTICIONES DISPONIBLES..." << endl;
                getchar();
                return;
            }
        }

        cout << "\tERROR NO HAY ESPACIO DISPONIBLE PARA LA PARTICION..." << endl;
        getchar();
        return;
    }
    else
    {
        for(int i = 0; i < particiones.size(); i++)
        {   
            if(particiones[i].part_type == 'e')
            {
                EBR ebr;
                int end = particiones[i].part_start + particiones[i].part_size;
                disco = fopen(path.c_str(), "rb+");
                fseek(disco, particiones[i].part_start, SEEK_SET);
                fread(&ebr, sizeof(EBR), 1, disco);
                
                while(true)
                {
                    if(ebr.part_size == 0 && ebr.part_next == -1)
                    {
                        ebr.part_fit = fit;
                        ebr.part_next = -1;
                        ebr.part_size = size;
                        ebr.part_start = ftell(disco);
                        strcpy(ebr.part_name, name.c_str());

                        fseek(disco, -sizeof(EBR), SEEK_CUR);
                        fwrite(&ebr, sizeof(EBR), 1, disco);
                        fclose(disco);
                        return;
                    }
                    else if(ebr.part_size == 0 && ebr.part_next != -1)
                    {
                        int b = ebr.part_next - ebr.part_start;
                        if(size < b)
                        {
                            ebr.part_fit = fit;
                            ebr.part_size = size;
                            strcpy(ebr.part_name, name.c_str());

                            fseek(disco, -sizeof(EBR), SEEK_CUR);
                            fwrite(&ebr, sizeof(EBR), 1, disco);
                            fclose(disco);
                            return;
                        }
                        else
                        {
                            fseek(disco, ebr.part_next, SEEK_SET);
                            fread(&ebr, sizeof(EBR), 1, disco);
                        }
                    }
                    else if(ebr.part_size > 0 && ebr.part_next != -1)
                    {
                        fseek(disco, ebr.part_next, SEEK_SET);
                        fread(&ebr, sizeof(EBR), 1, disco);
                    }
                    else if(ebr.part_size > 0 && ebr.part_next == -1)
                    {
                        fseek(disco, ebr.part_start + ebr.part_size, SEEK_SET);
                        int aux = sizeof(EBR);
                        aux += ftell(disco);
                        aux = end - aux;
                        if(size < aux)
                        {
                            ebr.part_fit = fit;
                            ebr.part_next = -1;
                            ebr.part_size = size;
                            aux = sizeof(EBR);
                            aux += ftell(disco);
                            ebr.part_start = aux;
                            strcpy(ebr.part_name, name.c_str());
                            fwrite(&ebr, sizeof(EBR), 1, disco);
                            fclose(disco);
                            return;
                        }
                        else
                        {
                            cout << "\tNO HAY ESPACIO SUFICIENTE DENTRO DE LA PARTICION..." << endl;
                            getchar();
                            return;
                        }
                    }
                }
            }
        }
    }
}

void fdiskDelete(string path, string name, string borrar)
{
    /* code */
}

void fdiskAdd(string path, string name, int add, char unit)
{
    /* code */
}

void fdiskMov(string path, string name)
{
    /* code */
}

void mount(string path, string name)
{
    /* code */
}

void unmount(string id)
{
    /* code */
}


// strtok y strcasecmp