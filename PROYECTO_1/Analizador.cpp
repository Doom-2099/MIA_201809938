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
                    unit = partComand[0];
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
                    type = partComand[0];
                }
            }
            else if(!strcasecmp(partComand, "@fit"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    fit = partComand[0];
                }
            }
            else if(!strcasecmp(partComand, "@delete"))
            {
                partComand = strtok(NULL, del);
                if(partComand != NULL)
                {
                    borrar = partComand;
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
                cout << "MOV" << endl;
                mov = true;
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
                                    cout << "\tERROR EL PARAMETRO ADD NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
                                    getchar();
                                    return;
                                }
                            }
                            else if(mov)
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
        else
        {
            cout << "\tERROR EL PARAMETRO PATH NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
            getchar();
            return;
        }
    }
    else if(!strcasecmp(parametros[0].c_str(), "unmount"))
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
            unmount(id);
        }
        else
        {
            cout << "\tERROR EL PARAMETRO ID NO SE HA ESPECIFICADO O TIENE UN VALOR INCORRECTO..." << endl;
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
