#include "FuncAuxiliar.h"

string toLowerCase(string str)
{
    for_each(str.begin(), str.end(), [](char &c)
             { c = tolower(c); });

    return str;
}

bool is_dir(string path)
{
    DIR * dir;
    if(dir = opendir(path.c_str()))
    {
        closedir(dir);
        return true;
    }
    return false;

}

bool is_file(string path)
{
    FILE * file;

    if (file = fopen(path.c_str(), "r"))    
    {
        fclose(file);
        return true;
    }
    return false;
}

int calculate_size(int size, char unit)
{
    if (unit == 'b')
    {
        return size;
    }
    else if (unit == 'k')
    {
        return size * 1024;
    }
    else if (unit == 'm')
    {
        return size * 1024 * 1024;
    }

    return 0;
}

vector<BusyBlock> getBusyBlocks(vector<Particion> part)
{
    vector<BusyBlock> busyblocks;

    for (int i = 0; i < part.size(); i++)
    {
        if (part[i].part_name[0] != 'n')
        {
            BusyBlock bb;
            bb.start = part[i].part_start;
            bb.end = bb.start + part[i].part_size;
            busyblocks.push_back(bb);
        }
    }

    sort(busyblocks.begin(), busyblocks.end(), [](BusyBlock &b1, BusyBlock &b2)
         { return b1.start < b2.start; });

    return busyblocks;
}

vector<FreeBlock> getFreeBlocks(vector<BusyBlock> blocks, int sizeDsk, int startDsk)
{
    vector<FreeBlock> freeBlocks;
    int start = startDsk;

    if(blocks.size() == 0)
    {
        FreeBlock fb;
        fb.start = start;
        fb.end = sizeDsk;
        fb.size = fb.end - fb.start;
        freeBlocks.push_back(fb);

        start = sizeDsk;
    }
    else
    {
        for(int i = 0; i < blocks.size(); i++)
        {
            if(start < blocks[i].start)
            {
                FreeBlock fb;
                fb.start = start;
                fb.end = blocks[i].end + 1;
                fb.size = fb.end - fb.start;
                freeBlocks.push_back(fb);
            }
            else
            {
                start = blocks[i].end + 1;
            }
        }

        if(start < sizeDsk)
        {
            FreeBlock fb;
            fb.start = start;
            fb.end = sizeDsk;
            fb.size = fb.end - fb.start;
            freeBlocks.push_back(fb);
        }
    }

    return freeBlocks;
}

string getPath(string path, string name)
{
    string ruta = "";
    char aux[50];
    strcpy(aux, name.c_str());
    char *partName = strtok(aux, ".");
    ruta = path;
    ruta.append(partName);
    ruta.append("-1.dsk");
    return ruta;
}

string getPathWithName(string path)
{
    string ruta = "";
    char aux[100];
    strcpy(aux, path.c_str());
    char *partName = strtok(aux, ".");
    ruta = partName;
    ruta.append("-1.dsk");
    return ruta;
}

int getNum(vector<PrtMount> list, string path)
{
    int cont = 1;

    for(int i = 0; i < list.size(); i++)
    {
        if(!strcmp(list[i].path.c_str(), path.c_str()))
        {
            cont++;
        }
    }

    return cont;
}

FILE* obtenerFile(string path, bool raid)
{
    if(raid)
    {
        return fopen(path.c_str(), "rb+");
    }
    else
    {
        return fopen(getPathWithName(path).c_str(), "rb+");
    }
}

string getDate()
{
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
    auto str = oss.str();

    return str;
}

Tree recorrerFS(FILE *disco, int pos_inode, string padre)
{
    Tree tree;
    string nodos;
    string nodo;
    string link;
    TablaInodos inodo;
    fseek(disco, pos_inode, SEEK_SET);
    fread(&inodo, sizeof(TablaInodos), 1, disco);

    for(int i = 0; i < 12; i++)
    {
        if(inodo.i_block[i] != -1)
        {
            BloqueCarpeta carpeta;
            fseek(disco, inodo.i_block[i], SEEK_SET);
            fread(&carpeta, sizeof(BloqueCarpeta), 1, disco);

            nodos.append("node");
            nodos.append(to_string(inodo.i_block[i]));

            nodo.append("node");
            nodo.append(to_string(inodo.i_block[i]));

            nodos.append("[label=\"");
            nodos.append(carpeta.b_content[0].b_name);
            nodos.append("\"];\n");

            if(padre != "")
            {
                link.append(padre);
                link.append("->");
                link.append(nodo);
                link.append(";\n");
            }

            tree.nodes.append(nodos);
            tree.links.append(link);
            link  = "";

            if(carpeta.b_content[0].b_inodo != -1)
            {
                Tree aux;
                aux = recorrerFS(disco, carpeta.b_content[0].b_inodo, nodo);
                tree.nodes.append(aux.nodes);
                tree.links.append(aux.links);
            }
            nodo = "";
        }
    }

    return tree;
}

string constructJson(FILE *disco, int pos_inode)
{
    TablaInodos inodo;
    string nodo;
    string nodos;
    fseek(disco, pos_inode, SEEK_SET);
    fread(&inodo, sizeof(TablaInodos), 1, disco);

    for(int i = 0; i < 12; i++)
    {
        if(inodo.i_block[i] != -1)
        {
            BloqueCarpeta carpeta;
            fseek(disco, inodo.i_block[i], SEEK_SET);
            fread(&carpeta, sizeof(BloqueCarpeta), 1, disco);

            nodo.append("{\n\"name\":\"");
            nodo.append(carpeta.b_content[0].b_name);
            nodo.append("\",\n");
            nodo.append("\"type\":\"0\",\n");
            nodo.append("\"fecha\":\"");
            nodo.append(inodo.i_ctime);
            nodo.append("\",\n");
            nodo.append("\"propietario\":\"\",\n");
            nodo.append("\"colaboradores\":[],");
            nodo.append("\"hijos\":[\n");
            
            if(carpeta.b_content[0].b_inodo != -1)
            {
                nodo.append(constructJson(disco, carpeta.b_content[0].b_inodo));    
            }

            nodo.append("]\n");

            if(inodo.i_block[i + 1] != -1 && (i + 1) < 12)
            {
                nodo.append("},\n");
            }
            else
            {
                nodo.append("}\n");
            }

            nodos.append(nodo);
            nodo = "";
        }
    }

    return nodos;
}