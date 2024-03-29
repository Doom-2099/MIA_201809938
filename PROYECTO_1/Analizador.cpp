#include "Analizador.h"

void analizarComando(string comando)
{
    char cmd[100];
    strcpy(cmd, comando.c_str()); // Copiar el comando a un char Array

    char *partComand = strtok(cmd, " "); // Primera llamada a strtok
    char del[] = "=>"; // Delimitadores para separar parametro de valor
    vector<string> parametros; // vector para guardar las partes del comando completo

    while (partComand != NULL) 
    {
        parametros.push_back(partComand);
        partComand = strtok(NULL, " "); // Splitear todo el comando por espacios (Pendiente analizar Ruta con espacios)
    }

    if(!strcasecmp(parametros[0].c_str(), "mkdisk")) // comparar la primera posicion del vector
    {                                                // para saber que comando se ejecutara
        int size = -1;
        char fit = 'f';
        char unit = 'm';
        string path = "";
        string name = "";

        for(int i = 1; i < parametros.size(); i++)  // Recorrer el resto del vector parametros
        {
            char aux[50];                           // char array para hacer la separacion de parametro-valor
            strcpy(aux, parametros[i].c_str());     // copia del contenido al char array
            partComand = strtok(aux, del);          // primer llamada al strtok para parametro valor

            if(!strcasecmp(partComand, "$size"))    // comparacion para saber a que parametro se refiere
            {
                partComand = strtok(NULL, del);     // nueva llamada a strtok para obtener valor con del --> "=>"
                if(partComand != NULL)              // pregunta si no es nulo
                {
                    size = atoi(partComand);
                }
            }
            else if(!strcasecmp(partComand, "$path"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    path = partComand;
                    path.erase(find(path.begin(), path.end(), '\"'));
                    path.erase(find(path.begin(), path.end(), '\"'));
                }
            }
            else if(!strcasecmp(partComand, "$name"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    name = partComand;
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        //Validaciones
        if (size > 0)
        {
            if ((size % 8) == 0)
            {
                if (path != "")
                {
                    if (name != "")
                    {
                        if (strstr(name.c_str(), ".dsk"))
                        {
                            mkdisk(size, path, name, 'm');
                        }
                        else
                        {
                            cout << "\tERROR EL NOMBRE DEL DISCO NO CONTIENE LA EXTENSION ESPECIFICADA \".dsk\"" << endl;
                            getchar();
                            return;
                        }
                    }
                    else
                    {
                        cout << "\tERROR EL PARAMETRO NAME NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                        getchar();
                        return;
                    }
                }
                else
                {
                    cout << "\tERROR EL PARAMETRO PATH NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                    getchar();
                    return;
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO SIZE DEBE SER UN ENTERMO MULTIPLO DE 8..." << endl;
                getchar();
                return;
            }
        }
        else
        {
            cout << "\tERROR EL PARAMETRO SIZE NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "rmdisk"))
    {
        string path = "";

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$path"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    path = partComand;
                    path.erase(find(path.begin(), path.end(), '\"'));
                    path.erase(find(path.begin(), path.end(), '\"'));
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if (path != "")
        {
            rmdisk(path);
        }
        else
        {
            cout << "\tERROR EL PARAMETRO PATH NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "fdisk"))
    { 
        int size = 0;
        char unit = 'k';
        string path = "";
        char type = 'p';
        char fit = 'w';
        string borrar = "";
        string name = "";
        int add = 0;
        bool mov = false;

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$size"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    size = atoi(partComand);
                }
            }
            else if(!strcasecmp(partComand, "@unit"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    unit = tolower(partComand[0]);
                }
            }
            else if(!strcasecmp(partComand, "$path"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    path = partComand;
                    path.erase(find(path.begin(), path.end(), '\"'));
                    path.erase(find(path.begin(), path.end(), '\"'));
                }
            }
            else if(!strcasecmp(partComand, "@type"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    type = tolower(partComand[0]);
                }
            }
            else if(!strcasecmp(partComand, "@fit"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    fit = tolower(partComand[0]);
                }
            }
            else if(!strcasecmp(partComand, "@delete"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    borrar = toLowerCase(partComand);
                }                
            }
            else if(!strcasecmp(partComand, "$name"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    name = partComand;
                }                
            }
            else if(!strcasecmp(partComand, "@add"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    add = atoi(partComand);
                }
            }
            else if(!strcasecmp(partComand, "@mov"))
            {
                partComand = strtok(NULL, del);

                if(partComand == NULL)
                {
                    mov = true;
                }
                else
                {
                    cout << "\tERROR, ESTE PARAMETRO NO LLEVA NINGUN VALOR..." << endl;
                    getchar();
                    return; 
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        //Validaciones
        if(path != "")
        {
            if(name != "")
            {
                if(unit == 'b' || unit == 'k' || unit == 'm')
                {
                    if(type == 'p' || type == 'e' || type == 'l')
                    {
                        if(fit == 'f' || fit == 'b' || fit == 'w')
                        {
                            if(borrar == "" && add == 0 && !mov)
                            {
                                if(size > 0)
                                {
                                    fdisk(size, path, name, unit, type, fit);
                                }
                                else
                                {
                                    cout << "\tERROR EL PARAMETRO SIZE NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                                    getchar();
                                    return;
                                }
                            }
                            else if(borrar != "" && add == 0 && !mov && size == 0)
                            {
                                if(borrar == "fast" || borrar == "full")
                                {
                                    fdiskDelete(path, name, borrar);
                                }
                                else
                                {
                                    cout << "\tERROR EL PARAMETRO DELETE NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                                    getchar();
                                    return;
                                }
                            }
                            else if(borrar == "" && !mov && size == 0)
                            {
                                if(add != 0)
                                {
                                    fdiskAdd(path, name, add, unit);
                                }
                                else
                                {
                                    cout << "\tERROR LA COMBINACION DE PARAMETROS INGRESADOS ES INCORRECTA..." << endl;
                                    getchar();
                                    return;
                                }
                            }
                            else if(mov && borrar == "" && size == 0 &&add == 0)
                            {
                                fdiskMov(path, name);
                            }
                            else
                            {
                                cout << "\tERROR NO EXISTE UNA COMBINACION PARA LOS PARAMETROS INGRESADOS..." << endl;
                                getchar();
                                return;
                            }
                        }
                        else
                        {
                            cout << "\tERROR EL PARAMETRO FIT TIENE UN VALOR INCORRECTO..." << endl;
                            getchar();
                            return;
                        }
                    }
                    else
                    {
                        cout << "\tERROR EL PARAMETRO TYPE TIENE UN VALOR INCORRECTO..." << endl;
                        getchar();
                        return;
                    }
                }
                else
                {
                    cout << "\tERROR EL PARAMETRO UNIT TIENE UN VALOR INCORRECTO..." << endl;
                    getchar();
                    return;
                }
            }
            else
            {
                cout << "\tEL PARAMETRO NAME NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                getchar();
                return;
            }
        }
        else
        {
            cout << "\tERROR EL PARAMETRO PATH NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "mount"))
    {
        string path = "";
        string name = "";

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$path"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    path = partComand;
                    path.erase(find(path.begin(), path.end(), '\"'));
                    path.erase(find(path.begin(), path.end(), '\"'));
                }
            }
            else if(!strcasecmp(partComand, "$name"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    name = partComand;
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if(path != "")
        {
            if(name != "")
            {
                mount(path, name);
            }
            else
            {
                cout << "\tERROR EL PARAMETRO NAME NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                getchar();
                return;
            }
        }
        else if(path == "" && name == "")
        {
            printMount();
        }
        else
        {
            cout << "\tERROR EL PARAMETRO PATH NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "unmount"))
    {
        vector<string> ids;
        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(strstr(toLowerCase(partComand).c_str(), "$id") != NULL)
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    ids.push_back(toLowerCase(partComand));
                }                
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if(ids.size() > 0)
        {
            unmount(ids);
        }
        else
        {
            cout << "\tERROR NO SE HA ESPECIFICADO NINGUN ID..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "mkfs"))
    {
        string id = "";
        string type = "full";
        int add = 0;
        char unit = 'k';

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$id"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    id = toLowerCase(partComand);
                }
            }
            else if(!strcasecmp(partComand, "@type"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    type = tolower(partComand[0]);
                }
            }
            else if(!strcasecmp(partComand, "@add"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    add = atoi(partComand);
                }
            }
            else if(!strcasecmp(partComand, "@unit"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    unit = tolower(partComand[0]);
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if(id != "")
        {
            if(!strcmp(type.c_str(), "fast") || !strcmp(type.c_str(), "full"))
            {
                if(add == 0)
                {
                    mkfs(id, type);
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO TYPE TIENE UN VALOR INCORRECTO..." << endl;
                getchar();
                return;
            }
        }
        else
        {
            cout << "\tERROR EL PARAMETRO ID NO SE HA ESPECIFICADO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "mkfile"))
    {
        string id = "";
        string path = "";
        bool p = false;
        int size = 0;
        string cont = "";

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$id"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    id = toLowerCase(partComand);
                }
            }
            else if(!strcasecmp(partComand, "$path"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    path = partComand;
                    path.erase(find(path.begin(), path.end(), '\"'));
                    path.erase(find(path.begin(), path.end(), '\"'));
                }
            }
            else if(!strcasecmp(partComand, "@p"))
            {
                partComand = strtok(NULL, del);
                
                if(partComand == NULL)
                {
                    p = true;
                }
                else
                {
                    cout << "\tERROR, ESTE PARAMETRO NO LLEVA NINGUN VALOR..." << endl;
                    getchar();
                    return;
                }
            }
            else if(!strcasecmp(partComand, "@size"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    size = atoi(partComand);
                }
            }
            else if(!strcasecmp(partComand, "@cont"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    cont = partComand;
                    cont.erase(find(cont.begin(), cont.end(), '\"'));
                    cont.erase(find(cont.begin(), cont.end(), '\"'));
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if(id != "")
        {
            if(path != "")
            {
                if(size < 0)
                {
                    mkfile(id, path, p, size, cont);
                }
                else
                {
                    cout << "\tERROR EL PARAMETRO SIZE TIENE VALOR NEGATIVO..." << endl;
                    getchar();
                    return;
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO PATH NO SE HA ESPECIFICADO..." << endl;
                getchar();
                return;
            }
        }
        else
        {
            cout << "\tERROR EL PARAMETRO ID NO SE HA ESPECIFICADO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "mkdir"))
    {
        string id = "";
        string path = "";
        bool p = false;

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$id"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    id = toLowerCase(partComand);
                }
            }
            else if(!strcasecmp(partComand, "$path"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    path = partComand;
                    path.erase(find(path.begin(), path.end(), '\"'));
                    path.erase(find(path.begin(), path.end(), '\"'));
                }
            }
            else if(!strcasecmp(partComand, "@p"))
            {
                partComand = strtok(NULL, del);
                if(partComand == NULL)
                {
                    p = true;
                }
                else
                {
                    cout << "\tERROR, ESTE PARAMETRO NO LLEVA NINGUN VALOR..." << endl;
                    getchar();
                    return;
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        //Validaciones
        if(id != "")
        {
            if(path != "")
            {
                mkdir(id, path, p);
            }
            else
            {
                cout << "\tERROR EL PATH NO SE HA ESPECIFICADO..." << endl;
                getchar();
                return;
            }
        }
        else
        {
            cout << "\tERROR EL ID NO SE HA ESPECIFICADO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "syncronice"))
    {
        string id = "";
        string path = "";

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "@id"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    id = toLowerCase(partComand);
                }
            }
            else if(!strcasecmp(partComand, "@path"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    path = partComand;
                    path.erase(find(path.begin(), path.end(), '\"'));
                    path.erase(find(path.begin(), path.end(), '\"'));
                }
            }
            else 
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if(id != "")
        {
            if(path != "")
            {
                synchronice(id, path);
            }
            else
            {
                cout << "\tERROR EL PATH NO SE HA ESPECIFICADO..." << endl;
                getchar();
                return;
            }
            
        }
        else
        {
            cout << "\tERROR EL ID NO SE HA ESPECIFICADO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "rm"))
    {
        string id = "";
        vector<string> fileID;

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);
            
            if(!strcasecmp(partComand, "$id"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    id = toLowerCase(partComand);
                }
            }
            else if(strstr(toLowerCase(partComand).c_str(), "$fileid") != NULL)
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    fileID.push_back(partComand);
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if(id != "")
        {
            if(fileID.size() > 0)
            {
                rm(id, fileID);
            }
            else
            {
                cout << "\tERROR NO SE HA INGRESADO NINGUN ARCHIVO A ELIMINAR..." << endl;
                getchar();
                return;
            }
        }
        else
        {
            cout << "\tERROR EL PARAMETRO ID NO SE HA ESPECIFICADO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "cp"))
    {
        string id = "";
        string path = "";
        string dest = "";
        string iddest = "";

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$id"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    id = partComand;
                }
            }
            else if(!strcasecmp(partComand, "$path"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    path = partComand;
                    path.erase(find(path.begin(), path.end(), '\"'));
                    path.erase(find(path.begin(), path.end(), '\"'));
                }
            }
            else if(!strcasecmp(partComand, "$dest"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    dest = partComand;
                }
            }
            else if(!strcasecmp(partComand, "$iddest"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    iddest = partComand;
                    iddest.erase(find(iddest.begin(), iddest.end(), '\"'));
                    iddest.erase(find(iddest.begin(), iddest.end(), '\"'));
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if(id != "")
        {
            if(path != "")
            {
                if(dest != "")
                {
                    if(iddest != "")
                    {
                        cp(id, path, dest, iddest);
                    }
                    else
                    {
                        cout << "\tERROR EL PARAMETRO IDDEST NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                        getchar();
                        return;
                    }
                }
                else
                {
                    cout << "\tERROR EL PARAMETRO DEST NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                    getchar();
                    return;
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO PATH NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                getchar();
                return;
            }
        }
        else
        {
            cout << "\tERROR EL PARAMETRO ID NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "find"))
    {
        string id = "";
        string path = "";
        string name = "";

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$id"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    id = partComand;
                }
            }
            else if(!strcasecmp(partComand, "$path"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    path = partComand;
                    path.erase(find(path.begin(), path.end(), '\"'));
                    path.erase(find(path.begin(), path.end(), '\"'));
                }
            }
            else if(!strcasecmp(partComand, "$name"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    name = partComand;
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if(id != "")
        {
            if(path != "")
            {
                if(name != "")
                {
                    find(id, path, name);
                }
                else
                {
                    cout << "\tERROR EL PARAMETRO NAME NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                    getchar();
                    return;
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO PATH NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                getchar();
                return;
            }
        }
        else
        {
            cout << "\tERROR EL PARAMETRO ID NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "ls"))
    {
        string id = "";
        string path = "";
        char order = '\0';

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$id"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    id = partComand;
                }
            }
            else if(!strcasecmp(partComand, "$path"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    path = partComand;
                    path.erase(find(path.begin(), path.end(), '\"'));
                    path.erase(find(path.begin(), path.end(), '\"'));
                }
            }
            else if(!strcasecmp(partComand, "$order"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    order = partComand[0];
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if(id != "")
        {
            if(path != "")
            {
                if(order != '\0' && (order == 'f' || order == 'c' || order == 'r'))
                {
                    ls(id, path, order);
                }
                else
                {
                    cout << "\tERROR EL PARAMETRO ORDER NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                    getchar();
                    return;
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO PATH NO SE HA ESPECIFICADO..." << endl;
                getchar();
                return;
            }
        }
        else
        {
            cout << "\tERROR EL PARAMETRO ID NO SE HA ESPECIFICADO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "cat"))
    {
        string id = "";
        vector<string> filen;

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$id"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    id = partComand;
                }
            }
            else if(strstr(toLowerCase(partComand).c_str(), "$filen") != NULL)
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    filen.push_back(partComand);
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if(id != "")
        {
            if(filen.size() > 0)
            {
                cat(id, filen);
            }
            else
            {
                cout << "\tERROR NO SE HAN ESPECIFICADO FICHEROS A ENLAZAR..." << endl;
                getchar();
                return;
            }
        }
        else
        {
            cout << "\tERROR EL PARAMETRO ID NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "recovery"))
    {
        string id = "";

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$id"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    id = partComand;
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if(id != "")
        {
            recovery(id);
        }
        else
        {
            cout << "\tERROR EL PARAMETRO ID NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "loss"))
    {
        string id = "";

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$id"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    id = partComand;
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if(id != "")
        {
            loss(id);
        }
        else
        {
            cout << "\tERROR EL PARAMETRO ID NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "rep"))
    {
        string rep = "";
        string path = "";
        string id = "";

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$name"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    rep = toLowerCase(partComand);
                }
            }
            else if(!strcasecmp(partComand, "$path"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    path = partComand;
                    path.erase(find(path.begin(), path.end(), '\"'));
                    path.erase(find(path.begin(), path.end(), '\"'));
                }
            }
            else if(!strcasecmp(partComand, "$id"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    id = toLowerCase(partComand);
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                getchar();
                return;
            }
        }

        // Validaciones
        if(path != "")
        {
            if(id != "")
            {
                if(rep != "")
                {
                    if(!strcasecmp(rep.c_str(), "mbr"))
                    {
                        mbrRep(path, id);
                    }
                    else if(!strcasecmp(rep.c_str(), "disk"))
                    {
                        dskRep(path, id);
                    }
                    else if(!strcasecmp(rep.c_str(), "sb"))
                    {
                        sbRep(path, id);
                    }
                    else if(!strcasecmp(rep.c_str(), "bm_block"))
                    {
                        bm_blockRep(path, id);
                    }
                    else if(!strcasecmp(rep.c_str(), "bm_inode"))
                    {
                        bm_inodeRep(path, id);
                    }
                    else if(!strcasecmp(rep.c_str(), "journaling"))
                    {
                        journalingRep(path, id);
                    }
                    else if(!strcasecmp(rep.c_str(), "inode"))
                    {
                        inodeRep(path, id);
                    }
                    else if(!strcasecmp(rep.c_str(), "block"))
                    {
                        blockRep(path, id);
                    }
                    else if(!strcasecmp(rep.c_str(), "tree"))
                    {
                        treeRep(path, id);
                    }
                    else if(!strcasecmp(rep.c_str(), "file"))
                    {
                        fileRep(path, id);
                    }
                    else
                    {
                        cout << "\tERROR EL PARAMETRO NAME TIENE UN VALOR INCORRECTO..." << endl;
                        getchar();
                        return;
                    }
                }
                else
                {
                    cout << "\tERROR EL PARAMETRO NAME NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                    getchar();
                    return;
                }
            }
            else
            {
                cout << "\tERROR EL PARAMETRO ID NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                getchar();
                return;
            }
        }
        else
        {
            cout << "\tERROR EL PARAMETRO PATH NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
            getchar();
            return;
        }

    }
    else if(!strcasecmp(parametros[0].c_str(), "exec"))
    {
        string path = "";

        for(int i = 1; i < parametros.size(); i++)
        {
            char aux[50];
            strcpy(aux, parametros[i].c_str());
            partComand = strtok(aux, del);

            if(!strcasecmp(partComand, "$path"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    path = partComand;
                    path.erase(find(path.begin(), path.end(), '\"'));
                    path.erase(find(path.begin(), path.end(), '\"'));
                }
                else
                {
                    cout << "\tERROR EL PARAMETRO " << partComand << " ES INCORRECTO..." << endl;
                    getchar();
                    return;
                }
            }
        }

        // Validaciones
        if(path != "")
        {
            FILE * archivo;
            archivo = fopen(path.c_str(), "r");

            if(archivo == NULL)
            {
                cout << "\tERROR EL ARCHIVO NO SE PUDO ABRIR..." << endl;
                getchar();
                return;
            }

            string linea;
            char buffer[120];

            while (feof(archivo) == 0)
            {
                fgets(buffer, 120, archivo);
                linea = buffer;
                if(strstr(linea.c_str(), "/*") != NULL || (buffer[0] == '\r' && buffer[1] == '\n'))
                {
                    cout << "\t" << linea;
                    continue; 
                }

                //linea.erase(find(linea.begin(), linea.end(), '\r'));
                linea.erase(find(linea.begin(), linea.end(), '\n'));
                cout << "\t" << linea << endl;
                analizarComando(linea);
            }

            fclose(archivo);
        }
        else
        {
            cout << "\tERROR EL PARAMETRO PATH NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
            getchar();
            return;
        }
    }
    else if (!strcasecmp(parametros[0].c_str(), "pause"))
    {
        getchar();
    }
    else
    {
        cout << "\tERROR EL COMANDO ESPECIFICADO ES INCORRECTO..." << endl;
        getchar();
        return;
    }
}
