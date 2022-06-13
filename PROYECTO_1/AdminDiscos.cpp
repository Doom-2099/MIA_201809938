#include "AdminDiscos.h"

static vector<PrtMount> mountList;
static vector<string> paths;

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

    cmd = "cp -a ";
    cmd.append(path);
    cmd.append(name);
    cmd.append(" ");
    cmd.append(getPath(path, name));

    path.append(name);

    FILE *disco;
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
        system(cmd.c_str());
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
    disco = fopen(path.c_str(), "rb+"); // Abrir Disco Orginal

    if (disco == NULL)
    {
        path = getPathWithName(path);
        disco = fopen(path.c_str(), "rb+"); // Abrir Disco Copia

        if (disco == NULL)
        {
            cout << "\tERROR LA RUTA ES INCORRECTA..." << endl;
            getchar();
            return;
        }
    }

    rewind(disco);
    fread(&mbr, sizeof(MBR), 1, disco);
    aux = ftell(disco);
    fclose(disco);

    vector<Particion> particiones;
    particiones.push_back(mbr.mbr_partition_1);
    particiones.push_back(mbr.mbr_partition_2);
    particiones.push_back(mbr.mbr_partition_3);
    particiones.push_back(mbr.mbr_partition_4);

    // Validacion Nombres Particiones MBR
    for (int i = 0; i < particiones.size(); i++)
    {
        if (!strcmp(particiones[i].part_name, name.c_str()))
        {
            cout << "\tEL NOMBRE PARA LA PARTICION YA EXISTE..." << endl;
            getchar();
            return;
        }
    }

    // Validacion Nombres Extendida
    for (int i = 0; i < particiones.size(); i++)
    {
        if (particiones[i].part_type == 'e')
        {
            EBR ebr;
            disco = fopen(path.c_str(), "rb+");
            fseek(disco, particiones[i].part_start, SEEK_SET);

            while (true)
            {
                fread(&ebr, sizeof(EBR), 1, disco);

                if (!strcmp(ebr.part_name, name.c_str()))
                {
                    fclose(disco);
                    cout << "\tEL NOMBRE PARA LA PARTICION YA EXISTE..." << endl;
                    getchar();
                    return;
                }
                else if (ebr.part_next != -1)
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
    if (type != 'l')
    {
        // Validacion Particiones Extendidas
        for (int i = 0; i < particiones.size(); i++)
        {
            if (particiones[i].part_type == type)
            {
                cout << "\tERROR YA EXISTE UNA PARTICION EXTENDIDA..." << endl;
                getchar();
                return;
            }
        }

        vector<BusyBlock> bblocks = getBusyBlocks(particiones);
        vector<FreeBlock> fblocks = getFreeBlocks(bblocks, mbr.mbr_size, aux);

        for (int i = 0; i < fblocks.size(); i++)
        {
            if (size < fblocks[i].size)
            {
                for (int j = 0; j < particiones.size(); j++)
                {
                    if (particiones[j].part_start == 0)
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

                            if (type == 'e')
                            {
                                EBR ebr;

                                ebr.part_fit = 'n';
                                ebr.part_next = -1;
                                ebr.part_size = 0;
                                ebr.part_start = 0;
                                strcpy(ebr.part_name, "n");

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

                            if (type == 'e')
                            {
                                EBR ebr;

                                ebr.part_fit = 'n';
                                ebr.part_next = -1;
                                ebr.part_size = 0;
                                ebr.part_start = 0;
                                strcpy(ebr.part_name, "n");

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

                            if (type == 'e')
                            {
                                EBR ebr;

                                ebr.part_fit = 'n';
                                ebr.part_next = -1;
                                ebr.part_size = 0;
                                ebr.part_start = 0;
                                strcpy(ebr.part_name, "n");

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

                            if (type == 'e')
                            {
                                EBR ebr;

                                ebr.part_fit = 'n';
                                ebr.part_next = -1;
                                ebr.part_size = 0;
                                ebr.part_start = 0;
                                strcpy(ebr.part_name, "n");

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
        for (int i = 0; i < particiones.size(); i++)
        {
            if (particiones[i].part_type == 'e')
            {
                EBR ebr;
                int end = particiones[i].part_start + particiones[i].part_size;
                disco = fopen(path.c_str(), "rb+");
                fseek(disco, particiones[i].part_start, SEEK_SET);
                fread(&ebr, sizeof(EBR), 1, disco);

                while (true)
                {
                    if (ebr.part_size == 0 && ebr.part_next == -1)
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
                    else if (ebr.part_size == 0 && ebr.part_next != -1)
                    {
                        int b = ebr.part_next - ebr.part_start;
                        if (size < b)
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
                    else if (ebr.part_size > 0 && ebr.part_next != -1)
                    {
                        fseek(disco, ebr.part_next, SEEK_SET);
                        fread(&ebr, sizeof(EBR), 1, disco);
                    }
                    else if (ebr.part_size > 0 && ebr.part_next == -1)
                    {
                        fseek(disco, ebr.part_start + ebr.part_size, SEEK_SET);
                        int aux = sizeof(EBR);
                        aux += ftell(disco);
                        aux = end - aux;
                        if (size < aux)
                        {
                            ebr.part_next = ftell(disco);
                            fseek(disco, ebr.part_start, SEEK_SET);
                            fseek(disco, -sizeof(EBR), SEEK_CUR);
                            fwrite(&ebr, sizeof(EBR), 1, disco);

                            fseek(disco, ebr.part_size, SEEK_CUR);

                            ebr.part_fit = fit;
                            ebr.part_start = ftell(disco) + sizeof(EBR);
                            ebr.part_next = -1;
                            ebr.part_size = size;
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
    MBR mbr;
    FILE *disco = NULL;
    disco = fopen(path.c_str(), "rb+"); // Abrir Disco Orginal

    if (disco == NULL)
    {
        path = getPathWithName(path);
        disco = fopen(path.c_str(), "rb+"); // Abrir Disco Copia

        if (disco == NULL)
        {
            cout << "\tERROR LA RUTA ES INCORRECTA..." << endl;
            getchar();
            return;
        }
    }

    rewind(disco);
    fread(&mbr, sizeof(MBR), 1, disco);

    vector<Particion> particiones;
    particiones.push_back(mbr.mbr_partition_1);
    particiones.push_back(mbr.mbr_partition_2);
    particiones.push_back(mbr.mbr_partition_3);
    particiones.push_back(mbr.mbr_partition_4);

    // Buscar por nombres
    for (int i = 0; i < particiones.size(); i++)
    {
        if (!strcmp(particiones[i].part_name, name.c_str()))
        {

            // BORRADO PARTICION DEL DISCO
            if (!strcmp(borrar.c_str(), "full"))
            {
                char relleno = '\0';
                fseek(disco, particiones[i].part_start, SEEK_SET);
                for(int i = 0; i <= particiones[i].part_size; i++)
                {
                    fwrite(&relleno, sizeof(relleno), 1, disco);
                }
            }

            // BORRADO PARTICION DEL MBR
            particiones[i].part_fit = 'n';
            particiones[i].part_size = 0;
            particiones[i].part_start = 0;
            particiones[i].part_status = 'n';
            particiones[i].part_type = 'n';
            strcpy(particiones[i].part_name, "n");

            switch (i)
            {
            case 0:
                mbr.mbr_partition_1 = particiones[i];
                break;

            case 1:
                mbr.mbr_partition_2 = particiones[i];
                break;

            case 2:
                mbr.mbr_partition_3 = particiones[i];
                break;

            case 3:
                mbr.mbr_partition_4 = particiones[i];
                break;
            }

            rewind(disco);
            fwrite(&mbr, sizeof(MBR), 1, disco);
            fclose(disco);
            return;
        }
    }

    for (int i = 0; i < particiones.size(); i++)
    {
        if (particiones[i].part_type == 'e')
        {
            EBR ebr;
            fseek(disco, particiones[i].part_start, SEEK_SET);

            while (true)
            {
                fread(&ebr, sizeof(EBR), 1, disco);

                if (!strcmp(ebr.part_name, name.c_str()))
                {
                    if (!strcmp(borrar.c_str(), "full"))
                    {
                        char relleno = '\0';
                        for(int i = 0; i <= ebr.part_size; i++)
                        {
                            fwrite(&relleno, sizeof(relleno), 1, disco);
                        }
                    }

                    fseek(disco, ebr.part_start, SEEK_SET);
                    fseek(disco, -sizeof(EBR), SEEK_CUR);

                    ebr.part_fit = 'n';
                    ebr.part_size = 0;
                    ebr.part_start = 0;
                    strcpy(ebr.part_name, "n");

                    fwrite(&ebr, sizeof(EBR), 1, disco);
                    fclose(disco);
                    return;
                }
                else if (ebr.part_next != -1)
                {
                    fseek(disco, ebr.part_next, SEEK_SET);
                }
                else
                {
                    fclose(disco);
                    cout << "\tERROR NO SE HA ENCONTRADO LA PARTICION INDICADA..." << endl;
                    getchar();
                    return;
                }
            }
        }
    }
}

void fdiskAdd(string path, string name, int add, char unit)
{
    MBR mbr;
    add = calculate_size(add, unit);
    FILE *disco = NULL;
    disco = fopen(path.c_str(), "rb+"); // Abrir Disco Orginal

    if (disco == NULL)
    {
        path = getPathWithName(path);
        disco = fopen(path.c_str(), "rb+"); // Abrir Disco Copia

        if (disco == NULL)
        {
            cout << "\tERROR LA RUTA ES INCORRECTA..." << endl;
            getchar();
            return;
        }
    }

    rewind(disco);
    fread(&mbr, sizeof(MBR), 1, disco);
    fclose(disco);

    vector<Particion> particiones;
    particiones.push_back(mbr.mbr_partition_1);
    particiones.push_back(mbr.mbr_partition_2);
    particiones.push_back(mbr.mbr_partition_3);
    particiones.push_back(mbr.mbr_partition_4);

    for (int i = 0; i < particiones.size(); i++)
    {
        if (!strcmp(particiones[i].part_name, name.c_str()))
        {
            vector<BusyBlock> bblocks = getBusyBlocks(particiones);

            for (int j = 0; j < bblocks.size(); j++)
            {
                if (bblocks[j].start == particiones[i].part_start)
                {
                    if ((add + bblocks[j].end) > 0)
                    {
                        if ((add + bblocks[j].end) < bblocks[j + 1].start)
                        {
                            particiones[i].part_size += add;
                            disco = fopen(path.c_str(), "rb+");
                            rewind(disco);

                            switch (i)
                            {
                            case 0:
                                mbr.mbr_partition_1 = particiones[i];
                                break;
                            case 1:
                                mbr.mbr_partition_2 = particiones[i];
                                break;

                            case 2:
                                mbr.mbr_partition_3 = particiones[i];

                            case 3:
                                mbr.mbr_partition_4 = particiones[i];
                            }

                            fwrite(&mbr, sizeof(MBR), 1, disco);
                            fclose(disco);
                            return;
                        }
                        else
                        {
                            cout << "\tERROR NO HAY ESPACIO DISPONIBLE PARA MODIFICAR EL TAMAÑO..." << endl;
                            getchar();
                            return;
                        }
                    }
                    else
                    {
                        cout << "\tERROR EL TAMAÑO RESULTANTE ES NEGATIVO, ES UN VALOR INCORRECTO..." << endl;
                        getchar();
                        return;
                    }
                }
            }
        }
    }

    for (int i = 0; i < particiones.size(); i++)
    {
        if (particiones[i].part_type = 'e')
        {
            EBR ebr;
            disco = fopen(path.c_str(), "rb+");
            fseek(disco, particiones[i].part_start, SEEK_SET);

            while (true)
            {
                fread(&ebr, sizeof(EBR), 1, disco);

                if (!strcmp(ebr.part_name, name.c_str()))
                {
                    if ((add + ebr.part_size) > 0)
                    {
                        if ((add + ebr.part_size) < ebr.part_next)
                        {
                            ebr.part_size += add;
                            fseek(disco, -sizeof(EBR), SEEK_CUR);
                            fwrite(&ebr, sizeof(EBR), 1, disco);
                            fclose(disco);
                            return;
                        }
                        else
                        {
                            cout << "\tERROR NO HAY ESPACIO DISPONIBLE PARA MODIFICAR EL TAMAÑO..." << endl;
                            getchar();
                            return;
                        }
                    }
                    else
                    {
                        cout << "\tERROR EL TAMAÑO RESULTANTE ES NEGATIVO, ES UN VALOR INCORRECTO..." << endl;
                        getchar();
                        return;
                    }
                }
                else if (ebr.part_next != 1)
                {
                    fseek(disco, ebr.part_next, SEEK_SET);
                }
                else
                {
                    fclose(disco);
                    cout << "\tERROR NO SE HA ENCONTRADO LA PARTICION INDICADA..." << endl;
                    getchar();
                    break;
                }
            }
        }
    }
}

void fdiskMov(string path, string name)
{
    cout << "\tCOMANDO MOV..." << endl;
}

void mount(string path, string name)
{
    MBR mbr;
    FILE *disco = NULL;
    disco = fopen(path.c_str(), "rb+"); // Abrir Disco Orginal

    if (disco == NULL)
    {
        path = getPathWithName(path);
        disco = fopen(path.c_str(), "rb+"); // Abrir Disco Copia

        if (disco == NULL)
        {
            cout << "\tERROR LA RUTA ES INCORRECTA..." << endl;
            getchar();
            return;
        }
    }

    rewind(disco);
    fread(&mbr, sizeof(MBR), 1, disco);
    fclose(disco);

    vector<Particion> particiones;
    particiones.push_back(mbr.mbr_partition_1);
    particiones.push_back(mbr.mbr_partition_2);
    particiones.push_back(mbr.mbr_partition_3);
    particiones.push_back(mbr.mbr_partition_4);

    for (int i = 0; i < particiones.size(); i++)
    {
        if (!strcmp(particiones[i].part_name, name.c_str()))
        {
            if (mountList.size() == 0)
            {
                PrtMount prt;
                prt.id = "vda1";
                prt.name = name;
                prt.path = path;
                prt.part_start = particiones[i].part_start;
                prt.part_size = particiones[i].part_size;
                prt.part_fit = particiones[i].part_fit;

                auto t = time(nullptr);
                auto tm = *localtime(&t);
                ostringstream oss;
                oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
                auto str = oss.str();
                strcpy(prt.mountDate, str.c_str());
                
                mountList.push_back(prt);
                paths.push_back(path);
                return;
            }
            else
            {
                PrtMount prt;
                char aux = 'a';
                string id = "vd";
                int cont = 0;

                while (cont < paths.size())
                {
                    if (!strcmp(path.c_str(), paths[cont].c_str()))
                    {
                        aux += cont;
                        break;
                    }
                    cont++;
                }

                if (cont == paths.size())
                {
                    paths.push_back(path);
                }

                id += aux;
                id.append(to_string(getNum(mountList, path)));

                prt.id = id;
                prt.name = name;
                prt.path = path;
                prt.part_start = particiones[i].part_start;
                prt.part_size = particiones[i].part_size;
                prt.part_fit = particiones[i].part_fit;

                auto t = time(nullptr);
                auto tm = *localtime(&t);
                ostringstream oss;
                oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
                auto str = oss.str();
                strcpy(prt.mountDate, str.c_str());

                mountList.push_back(prt);
                return;
            }
        }
    }

    for (int i = 0; i < particiones.size(); i++)
    {
        if (particiones[i].part_type == 'e')
        {
            EBR ebr;
            disco = fopen(path.c_str(), "rb+");
            fseek(disco, particiones[i].part_start, SEEK_SET);

            while (true)
            {
                fread(&ebr, sizeof(EBR), 1, disco);
                if (!strcmp(ebr.part_name, name.c_str()))
                {

                    if (mountList.size() == 0)
                    {
                        PrtMount prt;
                        prt.id = "vda1";
                        prt.name = name;
                        prt.path = path;
                        prt.part_start = ebr.part_start;
                        prt.part_size = ebr.part_size;
                        prt.part_fit = ebr.part_fit;
                        
                        auto t = time(nullptr);
                        auto tm = *localtime(&t);
                        ostringstream oss;
                        oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
                        auto str = oss.str();
                        strcpy(prt.mountDate, str.c_str());

                        mountList.push_back(prt);
                        paths.push_back(path);
                        return;
                    }
                    else
                    {
                        PrtMount prt;
                        char aux = 'a';
                        string id = "vd";
                        int cont = 0;

                        while (cont < paths.size())
                        {
                            if (!strcmp(path.c_str(), paths[cont].c_str()))
                            {
                                aux += cont;
                                break;
                            }
                            cont++;
                        }

                        if (cont == paths.size())
                        {
                            paths.push_back(path);
                        }

                        id += aux;
                        id.append(to_string(getNum(mountList, path)));

                        cout << "\tID PARTICION: " << id.c_str() << endl;

                        prt.id = id;
                        prt.name = name;
                        prt.path = path;
                        prt.part_start = particiones[i].part_start;
                        prt.part_size = particiones[i].part_size;
                        prt.part_fit = particiones[i].part_fit;

                        auto t = time(nullptr);
                        auto tm = *localtime(&t);
                        ostringstream oss;
                        oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
                        auto str = oss.str();
                        strcpy(prt.mountDate, str.c_str());

                        mountList.push_back(prt);
                        return;
                    }
                }
                else if (ebr.part_next != -1)
                {
                    fseek(disco, ebr.part_next, SEEK_SET);
                }
                else
                {
                    fclose(disco);
                    cout << "\tERROR NO EXISTE LA PARTICION INDICADA EN EL DISCO..." << endl;
                    getchar();
                    return;
                }
            }
        }
    }
}

void printMount()
{
    cout << endl << endl;
    cout << "\t-------------- PARTICIONES MONTADAS -------------------" << endl;
    for (int i = 0; i < mountList.size(); i++)
    {
        cout << "\t---------------- " << mountList[i].name << " ---------------" << endl;
        cout << "\tID: " << mountList[i].id << endl;
        cout << "\tPATH: " << mountList[i].path << endl;
        cout << "\tFECHA: " << mountList[i].mountDate << endl
             << endl;
    }
}

void unmount(vector<string> ids)
{
    for_each(ids.begin(), ids.end(), [](string &id)
    {
        int i = 0;
        while (i < mountList.size())
        {
            if(!strcmp(id.c_str(), mountList[i].id.c_str()))
            {
                mountList.erase(mountList.begin() + i);
                cout << "\tLA PARTICION " << id << " SE HA DESMONTADO" << endl;
                break;
            }
            
            i++;
        }

        if(i == mountList.size())
        {
            cout << "\tLA PARTICION " << id << " NO SE HA ENCONTRADO..." << endl;
            getchar();
            return;
        } 
    });
}

void mkfs(string id, string type)
{
    int i = 0;
    PrtMount prt;
    while (i < mountList.size())
    {
        if(!strcmp(id.c_str(), mountList[i].id.c_str()))
        {
            prt = mountList[i];
            break;
        }
        i++;
    }

    if(i == mountList.size())
    {
        cout << "\tERROR LA PARTICION INDICADA NO SE ENCUENTRA MONTADA..." << endl;
        getchar();
        return;
    }

    int n = 0;
    n = floor((prt.part_size - sizeof(SuperBloque))/(4+sizeof(Journal) + sizeof(TablaInodos)+(3*sizeof(BloqueArchivo))));
    cout << "\tVALOR N: " << n << endl;
    getchar();

    FILE * disco;
    disco = fopen(prt.path.c_str(), "rb+");

    if (disco == NULL)
    {
        prt.path = getPathWithName(prt.path);
        disco = fopen(prt.path.c_str(), "rb+"); // Abrir Disco Copia

        if (disco == NULL)
        {
            cout << "\tERROR LA RUTA ES INCORRECTA..." << endl;
            getchar();
            return;
        }
    }

    fseek(disco, prt.part_start, SEEK_SET);
    fseek(disco, sizeof(SuperBloque), SEEK_CUR);
    fseek(disco, (n*sizeof(Journal)), SEEK_CUR);
    fseek(disco, n, SEEK_CUR);
    fseek(disco, (3*n), SEEK_CUR);
    fseek(disco, (n*sizeof(TablaInodos)), SEEK_CUR);
    fseek(disco, (3*n*sizeof(BloqueArchivo)), SEEK_CUR);

    if(ftell(disco) > (prt.part_start + prt.part_size)) // Verificar que EXT3 cabe en particion
    {
        cout << "\tERROR EL SISTEMA DE ARCHIVOS NO CABE EN LA PARTICION SELECCIONADA..." << endl;
        getchar();
        return;
    }

    fseek(disco, prt.part_start, SEEK_SET); 

    SuperBloque sb;                                        // Setear Datos Del SuperBloque
    sb.s_filesystem_type = 3;
    sb.s_inodes_count = n;
    sb.s_blocks_count = 3*n;
    sb.s_free_blocks_count = 3*n;
    sb.s_free_inodes_count = n;
    strcpy(sb.s_mtime, prt.mountDate);
    strcpy(sb.s_umtime, "00/00/0000 00:00:00");
    sb.s_mnt_count += 1;
    sb.s_magic = 0xEF53;
    sb.s_inode_size = sizeof(TablaInodos);
    sb.s_block_size = sizeof(BloqueArchivo);
    sb.s_bm_inode_start = ftell(disco) + sizeof(SuperBloque) + (n*sizeof(Journal));
    sb.s_bm_block_start = sb.s_bm_inode_start + n;
    sb.s_inode_start = sb.s_bm_block_start + (3*n);
    sb.s_block_start = sb.s_inode_start + (n*sizeof(TablaInodos));
    sb.s_first_ino = sb.s_inode_start;
    sb.s_first_blo = sb.s_block_start;

    fseek(disco, prt.part_start, SEEK_SET);                 // Moverse al principio de particion
    fwrite(&sb, sizeof(SuperBloque), 1, disco);             // Escribir el superbloque en la particion

    Journal j;                                              // Setear Datos Journal
    j.journal_tipo_op = Operation::NEW_CARPET;
    j.journal_tipo = '1';
    strcpy(j.journal_nombre, "/");
    j.journal_contenido = '0';

    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
    auto str = oss.str();
    strcpy(j.journal_fecha, str.c_str());
    j.next = -1;

    fwrite(&j, sizeof(Journal), 1, disco);          // Escribir el Journal
    fseek(disco, (n-1)*sizeof(Journal), SEEK_CUR);  // Salto Hacia El BitMap De Bloques

    char bit = '1';                                  // Bit para los bitmap
    fwrite(&bit, sizeof(bit), 1, disco);             // Bit Inodo Carpeta /
    bit = '0';                                       // Cambio De Valor
    for(int i = 1; i < n; i++)
    {
        fwrite(&bit, sizeof(bit), 1, disco);       // BitMap de Inodos
    }

    bit = '1';                                       // Cambio De Valor
    fwrite(&bit, sizeof(bit), 1, disco);             // Bit Bloque De Carpeta
    bit = '0';                                       // Cambio De Valor
    for(int i = 1; i < (3*n); i++)
    {
        fwrite(&bit, sizeof(bit), 1, disco);       // BitMap de Bloques
    }

    TablaInodos ti;                                  // Primer Inodo, Carpeta /
    ti.i_uid = 777;
    ti.i_gid = 777;
    ti.i_size = -1;

    t = time(nullptr);
    tm = *localtime(&t);
    oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
    str = oss.str();
    strcpy(ti.i_atime, str.c_str());
    strcpy(ti.i_ctime, str.c_str());
    strcpy(ti.i_mtime, str.c_str());

    ti.i_block[0] = ftell(disco) + (n*sizeof(TablaInodos));

    for(int i = 1; i < 15; i++)
    {
        ti.i_block[i] = -1;
    }

    ti.i_type = '0';

    fwrite(&ti, sizeof(TablaInodos), 1, disco);             // Escribir Primer Inodos
    fseek(disco, (n-1)*sizeof(TablaInodos), SEEK_CUR);      // Salto Hacia Los Bloques

    Content c;              // Setear Datos Content 1
    strcpy(c.b_name, "/");
    c.b_inodo = -1;

    BloqueCarpeta bc;       // Setear Datos Bloque 1
    bc.b_content[0] = c;

    fwrite(&bc, sizeof(BloqueCarpeta), 1, disco);
    fclose(disco);

}

vector<PrtMount> getList()
{
    return mountList;
}
