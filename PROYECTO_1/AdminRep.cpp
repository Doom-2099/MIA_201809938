#include "AdminRep.h"

void dskRep(string path, string id)
{
    vector<PrtMount> list = getList();

    string ruta;
    PrtMount prt;
    int i = 0;
    while(i < list.size())
    {
        if(!strcmp(id.c_str(), list[i].id.c_str()))
        {
            ruta = list[i].path;
            break;
        }
        i++;
    }

    if(i == list.size())
    {
        cout << "\tERROR LA PARTICION NO SE ENCUENTRA MONTADA..." << endl;
        getchar();
        return;
    }
    
    // ------------------------
    // ---- RECUPERAR MBR -----
    // ------------------------
    MBR mbr;
    int aux;
    FILE *disco = NULL;
    disco = fopen(ruta.c_str(), "rb+"); // Abrir Disco Orginal

    if(disco == NULL)
    {
        ruta = getPathWithName(ruta);
        disco = fopen(ruta.c_str(), "rb+"); // Abrir Disco Copia

        if(disco == NULL)
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

    sort(particiones.begin(), particiones.end(), [](Particion &p1, Particion &p2)
         { return p1.part_start < p2.part_start; });

    FILE *reporte = NULL;
    reporte = fopen("repDisk.dot", "w+");
    if (reporte == NULL)
    {
        // ERROR
        cout << "\tERROR NO SE PUDO HACER EL REPORTE DEL DISCO..." << endl;
        getchar();
        return;
    }
    else
    {
        string header = "digraph G{\ndsk[ shape = box; label = <\n";
        header.append("<table border=\'0\' cellborder=\'2\' width=\'600\' height=\'75\' color=\'LIGHTSTEELBLUE\'>\n");
        header.append("<tr>\n<td height=\'75\' width=\'100\'> MBR </td>\n");

        // ------------------------------------------------
        // ---- RECORRER PARTICIONES PARA EL DISCO --------
        // ------------------------------------------------
        for (int i = 0; i < particiones.size(); i++)
        {
            if(particiones[i].part_fit != 'n')
            {
                if(particiones[i].part_type == 'p')
                {
                    if(aux < particiones[i].part_start)
                    {
                        header.append("<td height=\'75\' width=\'100\'> Libre </td>\n");
                        header.append("<td height=\'75\' width=\'100\'>");
                        header.append(particiones[i].part_name);
                        header.append("</td>\n");
                        aux = particiones[i].part_start + particiones[i].part_size;
                    }
                    else
                    {
                        header.append("<td height=\'75\' width=\'100\'>");
                        header.append(particiones[i].part_name);
                        header.append("</td>\n");
                        aux = particiones[i].part_start + particiones[i].part_size;
                    }   
                }
                else
                {
                    header.append("<td height=\'75\' width=\'100\'>\n");
                    header.append("<table border=\'0\' cellborder=\'2\' width=\'100%\' height=\'75\' color=\'LIGHTSTEELBLUE\'>\n");
                    header.append("<tr>\n<td height=\'45\' colspan=\'15\'> EXTENDIDA </td>\n</tr>\n<tr>");

                    EBR ebr;
                    disco = fopen(ruta.c_str(), "rb+");
                    fseek(disco, particiones[i].part_start, SEEK_SET);
                    aux = ebr.part_start;

                    while(true)
                    {
                        fread(&ebr, sizeof(EBR), 1, disco);

                        if(aux < ebr.part_start)
                        {
                            header.append("<td height=\'75\' width=\'100\'> Libre </td>");
                            aux = ebr.part_start;
                        }

                        if(ebr.part_size == 0)
                        {
                            header.append("<td height=\'75\' width=\'100\'> EBR </td>");
                            aux = ftell(disco);
                        }
                        else if(ebr.part_size > 0)
                        {
                            header.append("<td height=\'75\' width=\'100\'> EBR </td>");
                            header.append("<td height=\'75\' width=\'100\'> Logica </td>");
                            aux = ebr.part_start + ebr.part_size;
                        }

                        if(ebr.part_next != -1)
                        {
                            fseek(disco, ebr.part_next, SEEK_SET);
                        }
                        else
                        {
                            fclose(disco);
                            header.append("</tr>\n</table>\n</td>\n");
                            break;
                        }
                    }

                    aux = particiones[i].part_start + particiones[i].part_size;
                }
            }
        }

        if(aux < mbr.mbr_size)
        {
            header.append("<td height=\'75\' width=\'100\'> Libre </td>");
        }

        header.append("</tr>\n</table>\n>\n];\n}");
        fputs(header.c_str(), reporte);
        fclose(reporte);
        try
        {   
            string cmd = "dot -Tpng repDisk.dot -o ";
            cmd.append(path);
            system(cmd.c_str());
            remove("repDisk.dot");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void mbrRep(string path, string id)
{
    vector<PrtMount> list = getList();

    string ruta;
    PrtMount prt;
    int i = 0;
    while(i < list.size())
    {
        if(!strcmp(id.c_str(), list[i].id.c_str()))
        {
            ruta = list[i].path;
            break;
        }
        i++;
    }

    if(i == list.size())
    {
        cout << "\tERROR LA PARTICION NO SE ENCUENTRA MONTADA..." << endl;
        getchar();
        return;
    }

    // ------------------------
    // ---- RECUPERAR MBR -----
    // ------------------------
    MBR mbr;
    int aux;
    FILE *disco = NULL;
    disco = fopen(ruta.c_str(), "rb+"); // Abrir Disco Orginal

    if(disco == NULL)
    {
        ruta = getPathWithName(ruta);
        disco = fopen(ruta.c_str(), "rb+"); // Abrir Disco Copia

        if(disco == NULL)
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

    FILE *reporte = NULL;
    reporte = fopen("repMBR.dot", "w+");
    if (reporte == NULL)
    {
        // ERROR
        cout << "\tERROR NO SE PUDO HACER EL REPORTE DEL DISCO..." << endl;
        getchar();
        return;
    }
    else
    {
        string header = "digraph G{\ndsk[ shape = box; label = <\n";
        header.append("<table border=\'0\' cellborder=\'2\' width=\'600\' height=\'75\' color=\'LIGHTSTEELBLUE\'>\n");

        // TITULO MBR
        header.append("<tr>\n<td height=\'25\' colspan=\'3\'> ----- MBR ----- </td>\n</tr>\n");

        // Disk Signature
        header.append("<tr>\n<td height=\'25\' width=\'50\'> Signature </td>\n");
        header.append("<td height=\'25\' width=\'100\'> ");
        header.append(to_string(mbr.mbr_disk_signature));
        header.append("</td>\n</tr>\n");

        // Disk Size
        header.append("<tr>\n<td height=\'25\' width=\'50\'> Size </td>\n");
        header.append("<td height=\'25\' width=\'100\'> ");
        header.append(to_string(mbr.mbr_size));
        header.append("</td>\n</tr>\n");

        // Disk Fecha
        header.append("<tr>\n<td height=\'25\' width=\'50\'> Date </td>\n");
        header.append("<td height=\'25\' width=\'100\'> ");
        header.append(mbr.mbr_fecha_creacion);
        header.append("</td>\n</tr>\n");

        // ------------------------------------------------
        // ---- RECORRER PARTICIONES PARA EL DISCO --------
        // ------------------------------------------------
        for (int i = 0; i < particiones.size(); i++)
        {
            if (particiones[i].part_type == 'e')
            {
                // TITULO EXTENDIDA
                header.append("<tr>\n<td height=\'25\' colspan=\'3\'> ----- EXTENDIDA ----- </td>\n</tr>\n");

                // Partition Name
                header.append("<tr>\n<td height=\'25\' width=\'50\'> Name </td>\n");
                header.append("<td height=\'25\' width=\'100\'> ");
                header.append(particiones[i].part_name);
                header.append("</td>\n</tr>\n");

                // Partition Start
                header.append("<tr>\n<td height=\'25\' width=\'50\'> Start </td>\n");
                header.append("<td height=\'25\' width=\'100\'> ");
                header.append(to_string(particiones[i].part_start));
                header.append("</td>\n</tr>\n");

                // Partition Size
                header.append("<tr>\n<td height=\'25\' width=\'50\'> Size </td>\n");
                header.append("<td height=\'25\' width=\'100\'> ");
                header.append(to_string(particiones[i].part_size));
                header.append("</td>\n</tr>\n");

                // Partition Status
                header.append("<tr>\n<td height=\'25\' width=\'50\'> Status </td>\n");
                header.append("<td height=\'25\' width=\'100\'> ");
                header += particiones[i].part_status;
                header.append("</td>\n</tr>\n");

                // Partition Type
                header.append("<tr>\n<td height=\'25\' width=\'50\'> Type </td>\n");
                header.append("<td height=\'25\' width=\'100\'> ");
                header += particiones[i].part_type;
                header.append("</td>\n</tr>\n");

                // Partition Fit
                header.append("<tr>\n<td height=\'25\' width=\'50\'> Fit </td>\n");
                header.append("<td height=\'25\' width=\'100\'> ");
                header += particiones[i].part_fit;
                header.append("</td>\n</tr>\n");

                EBR ebr;
                disco = fopen(ruta.c_str(), "rb+");
                fseek(disco, particiones[i].part_start, SEEK_SET);

                while (true)
                {
                    fread(&ebr, sizeof(EBR), 1, disco);

                    if (ebr.part_fit != 'n')
                    {
                        // TITULO LOGICA
                        header.append("<tr>\n<td height=\'25\' colspan=\'3\'> ----- LOGICA ----- </td>\n</tr>\n");

                        // Partition Name
                        header.append("<tr>\n<td height=\'25\' width=\'50\'> Name </td>\n");
                        header.append("<td height=\'25\' width=\'100\'> ");
                        header.append(ebr.part_name);
                        header.append("</td>\n</tr>\n");

                        // Partition Start
                        header.append("<tr>\n<td height=\'25\' width=\'50\'> Start </td>\n");
                        header.append("<td height=\'25\' width=\'100\'> ");
                        header.append(to_string(ebr.part_start));
                        header.append("</td>\n</tr>\n");

                        // Partition Size
                        header.append("<tr>\n<td height=\'25\' width=\'50\'> Size </td>\n");
                        header.append("<td height=\'25\' width=\'100\'> ");
                        header.append(to_string(ebr.part_size));
                        header.append("</td>\n</tr>\n");

                        // Partition Next
                        header.append("<tr>\n<td height=\'25\' width=\'50\'> Next </td>\n");
                        header.append("<td height=\'25\' width=\'100\'> ");
                        header.append(to_string(ebr.part_next));
                        header.append("</td>\n</tr>\n");

                        // Partition Fit
                        header.append("<tr>\n<td height=\'25\' width=\'50\'> Fit </td>\n");
                        header.append("<td height=\'25\' width=\'100\'> ");
                        header += ebr.part_fit;
                        header.append("</td>\n</tr>\n");
                    }

                    if (ebr.part_next != -1)
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
            else if (particiones[i].part_type == 'p')
            {
                // TITULO PRIMARIA
                header.append("<tr>\n<td height=\'25\' colspan=\'3\'> ----- PRIMARIA ----- </td>\n</tr>\n");

                // Partition Name
                header.append("<tr>\n<td height=\'25\' width=\'50\'> Name </td>\n");
                header.append("<td height=\'25\' width=\'100\'> ");
                header.append(particiones[i].part_name);
                header.append("</td>\n</tr>\n");

                // Partition Start
                header.append("<tr>\n<td height=\'25\' width=\'50\'> Start </td>\n");
                header.append("<td height=\'25\' width=\'100\'> ");
                header.append(to_string(particiones[i].part_start));
                header.append("</td>\n</tr>\n");

                // Partition Size
                header.append("<tr>\n<td height=\'25\' width=\'50\'> Size </td>\n");
                header.append("<td height=\'25\' width=\'100\'> ");
                header.append(to_string(particiones[i].part_size));
                header.append("</td>\n</tr>\n");

                // Partition Status
                header.append("<tr>\n<td height=\'25\' width=\'50\'> Status </td>\n");
                header.append("<td height=\'25\' width=\'100\'> ");
                header += particiones[i].part_status;
                header.append("</td>\n</tr>\n");

                // Partition Type
                header.append("<tr>\n<td height=\'25\' width=\'50\'> Type </td>\n");
                header.append("<td height=\'25\' width=\'100\'> ");
                header += particiones[i].part_type;
                header.append("</td>\n</tr>\n");

                // Partition Fit
                header.append("<tr>\n<td height=\'25\' width=\'50\'> Fit </td>\n");
                header.append("<td height=\'25\' width=\'100\'> ");
                header += particiones[i].part_fit;
                header.append("</td>\n</tr>\n");
            }
        }

        header.append("</table>\n>\n];\n}");
        fputs(header.c_str(), reporte);
        fclose(reporte);
        
        try
        {
            string cmd = "dot -Tpng repMBR.dot -o ";
            cmd.append(path);
            system(cmd.c_str());
            remove("repMBR.dot");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void sbRep(string path, string id)
{
    vector<PrtMount> list = getList();

    PrtMount prt;
    int i = 0;
    while(i < list.size())
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
        cout << "\tERROR LA PARTICION NO SE ENCUENTRA MONTADA..." << endl;
        getchar();
        return;
    }

    FILE * disco;
    disco = fopen(prt.path.c_str(), "rb+");

    if(disco == NULL)
    {
        disco = fopen(getPathWithName(prt.path).c_str(), "rb+"); // Abrir Disco Copia

        if(disco == NULL)
        {
            cout << "\tERROR LA RUTA ES INCORRECTA..." << endl;
            getchar();
            return;
        }
    }

    SuperBloque sb;
    fseek(disco, prt.part_start, SEEK_SET);
    fread(&sb, sizeof(SuperBloque), 1, disco);

    string header = "digraph G{\ndsk[ shape = box; label = <\n";
    header.append("<table border=\'0\' cellborder=\'2\' width=\'600\' height=\'75\' color=\'LIGHTSTEELBLUE\'>\n");
    header.append("<tr>\n<td height=\'25\' colspan=\'3\'> ----- SUPERBLOQUE ----- </td>\n</tr>\n");

    // File System
    header.append("<tr>\n<td height=\'25\' width=\'50\'> FileSystem Type </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_filesystem_type));
    header.append("</td>\n</tr>\n");

    // Inodes Count
    header.append("<tr>\n<td height=\'25\' width=\'50\'> Inodes Count </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_inodes_count));
    header.append("</td>\n</tr>\n");

    // Blocks Count
    header.append("<tr>\n<td height=\'25\' width=\'50\'> Blocks Count </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_blocks_count));
    header.append("</td>\n</tr>\n");

    // Free Inodes Count
    header.append("<tr>\n<td height=\'25\' width=\'50\'> Free Inodes </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_free_inodes_count));
    header.append("</td>\n</tr>\n");

    // Free Blocks Count
    header.append("<tr>\n<td height=\'25\' width=\'50\'> Free Blocks </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_free_blocks_count));
    header.append("</td>\n</tr>\n");

    // Mount Time
    header.append("<tr>\n<td height=\'25\' width=\'50\'> Mount Time </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(sb.s_mtime);
    header.append("</td>\n</tr>\n");

    // Unmount Time
    header.append("<tr>\n<td height=\'25\' width=\'50\'> Unmount Time </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(sb.s_umtime);
    header.append("</td>\n</tr>\n");

    // Mount Count
    header.append("<tr>\n<td height=\'25\' width=\'50\'> Mount Count </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_mnt_count));
    header.append("</td>\n</tr>\n");

    // Magic 
    header.append("<tr>\n<td height=\'25\' width=\'50\'> Magic </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_magic));
    header.append("</td>\n</tr>\n");

    // Inode Size
    header.append("<tr>\n<td height=\'25\' width=\'50\'> Inode Size </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_inode_size));
    header.append("</td>\n</tr>\n");

    // Block Size
    header.append("<tr>\n<td height=\'25\' width=\'50\'> Block Size </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_block_size));
    header.append("</td>\n</tr>\n");

    // First Inode
    header.append("<tr>\n<td height=\'25\' width=\'50\'> First Inode </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_first_ino));
    header.append("</td>\n</tr>\n");

    // First Block
    header.append("<tr>\n<td height=\'25\' width=\'50\'> First Block </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_first_blo));
    header.append("</td>\n</tr>\n");

    // BitMap Inode
    header.append("<tr>\n<td height=\'25\' width=\'50\'> BitMap Inode </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_bm_inode_start));
    header.append("</td>\n</tr>\n");

    // BitMap Block
    header.append("<tr>\n<td height=\'25\' width=\'50\'> BitMap Block </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_bm_block_start));
    header.append("</td>\n</tr>\n");

    // BitMap Start
    header.append("<tr>\n<td height=\'25\' width=\'50\'> Inode Start </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_inode_start));
    header.append("</td>\n</tr>\n");

    // BitMap Start
    header.append("<tr>\n<td height=\'25\' width=\'50\'> Block Start </td>\n");
    header.append("<td height=\'25\' width=\'100\'> ");
    header.append(to_string(sb.s_block_start));
    header.append("</td>\n</tr>\n");

    header.append("</table>\n>\n];\n}");
    fclose(disco);

    FILE *reporte = NULL;
    reporte = fopen("repSB.dot", "w+");
    if (reporte == NULL)
    {
        // ERROR
        cout << "\tERROR NO SE PUDO HACER EL REPORTE DEL DISCO..." << endl;
        getchar();
        return;
    }
    else
    {
        fputs(header.c_str(), reporte);
        fclose(reporte);

        try
        {
            string cmd = "dot -Tpng repSB.dot -o ";
            cmd.append(path);
            system(cmd.c_str());
            remove("repSB.dot");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void bm_blockRep(string path, string id)
{  
    // Reporte BitMap Bloques
    vector<PrtMount> list = getList();
    PrtMount prt;

    int i = 0;
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
        cout << "\tERROR LA PARTICION NO SE ENCUENTRA MONTADA..." << endl;
        getchar();
        return;
    }

    FILE * disco;
    disco = fopen(prt.path.c_str(), "rb+");

    if(disco == NULL)
    {
        disco = fopen(getPathWithName(prt.path).c_str(), "rb+"); // Abrir Disco Copia
        if(disco == NULL)
        {
            cout << "\tERROR LA RUTA ES INCORRECTA..." << endl;
            getchar();
            return;
        }
    }

    SuperBloque sb;

    fseek(disco, prt.part_start, SEEK_SET);
    fread(&sb, sizeof(SuperBloque), 1, disco);
    fseek(disco, sb.s_bm_block_start, SEEK_SET);

    int x = 5;
    int y = 0;

    while (true)
    {
        if((sb.s_blocks_count % x) == 0)
        {
            y = sb.s_blocks_count / x;
            break;
        }
        x++;
    }

    char bit = '0';
    int cont = 1;
    string header = "digraph G{\ndsk[ shape = box; label = <\n";
    header.append("<table border=\'0\' cellborder=\'2\' width=\'600\' height=\'75\' color=\'LIGHTSTEELBLUE\'>\n");

    for(int i = 0; i < x; i++)
    {
        header.append("<tr>\n");

        for (int j = 0; j < y; j++)
        {
            header.append("<td height=\'20\' width=\'20\'>");
            fread(&bit, sizeof(bit), 1, disco);
            header += bit;
            header.append(" B ");
            header.append(to_string(cont));
            header.append("</td>\n");
            cont++;
        }

        header.append("</tr>\n");
    }

    fclose(disco);
    header.append("</table>\n>\n];\n}");

    FILE *reporte = NULL;
    reporte = fopen("repBM_B.dot", "w+");
    if (reporte == NULL)
    {
        // ERROR
        cout << "\tERROR NO SE PUDO HACER EL REPORTE DEL DISCO..." << endl;
        getchar();
        return;
    }
    else
    {
        fputs(header.c_str(), reporte);
        fclose(reporte);

        try
        {
            string cmd = "dot -Tpng repBM_B.dot -o ";
            cmd.append(path);
            system(cmd.c_str());
            remove("repBM_B.dot");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void bm_inodeRep(string path, string id)
{
    // Reporte BitMap Inodos
    vector<PrtMount> list = getList();
    PrtMount prt;

    int i = 0;
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
        cout << "\tERROR LA PARTICION NO SE ENCUENTRA MONTADA..." << endl;
        getchar();
        return;
    }

    FILE * disco;
    disco = fopen(prt.path.c_str(), "rb+");

    if(disco == NULL)
    {
        disco = fopen(getPathWithName(prt.path).c_str(), "rb+"); // Abrir Disco Copia

        if(disco == NULL)
        {
            cout << "\tERROR LA RUTA ES INCORRECTA..." << endl;
            getchar();
            return;
        }
    }

    SuperBloque sb;

    fseek(disco, prt.part_start, SEEK_SET);
    fread(&sb, sizeof(SuperBloque), 1, disco);
    fseek(disco, sb.s_bm_inode_start, SEEK_SET);

    int x = 5;
    int y = 0;

    while (true)
    {
        if((sb.s_inodes_count % x) == 0)
        {
            y = sb.s_inodes_count / x;
            break;
        }
        x++;
    }

    char bit;
    int cont = 1;
    string header = "digraph G{\ndsk[ shape = box; label = <\n";
    header.append("<table border=\'0\' cellborder=\'2\' width=\'600\' height=\'75\' color=\'LIGHTSTEELBLUE\'>\n");

    for(int i = 0; i < x; i++)
    {
        header.append("<tr>\n");

        for (int j = 0; j < y; j++)
        {
            header.append("<td height=\'20\' width=\'20\'>");
            fread(&bit, sizeof(bit), 1, disco);
            header += bit;
            header.append(" Y ");
            header.append(to_string(cont));
            header.append("</td>\n");
            cont++;
        }

        header.append("</tr>\n");
    }

    header.append("</table>\n>\n];\n}");
    fclose(disco);

    FILE *reporte = NULL;
    reporte = fopen("repBM_I.dot", "w+");
    if (reporte == NULL)
    {
        // ERROR
        cout << "\tERROR NO SE PUDO HACER EL REPORTE DEL DISCO..." << endl;
        getchar();
        return;
    }
    else
    {
        fputs(header.c_str(), reporte);
        fclose(reporte);

        try
        {
            string cmd = "dot -Tpng repBM_I.dot -o ";
            cmd.append(path);
            system(cmd.c_str());
            remove("repBM_I.dot");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void journalingRep(string path, string id)
{
    // Codigo Reporte Journaling
    vector<PrtMount> list = getList();
    PrtMount prt;

    int i = 0;
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
        cout << "\tERROR LA PARTICION NO SE ENCUENTRA MONTADA..." << endl;
        getchar();
        return;
    }

    FILE * disco;
    disco = fopen(prt.path.c_str(), "rb+");

    if(disco == NULL)
    {
        disco = fopen(getPathWithName(prt.path).c_str(), "rb+"); // Abrir Disco Copia

        if(disco == NULL)
        {
            cout << "\tERROR LA RUTA ES INCORRECTA..." << endl;
            getchar();
            return;
        }
    }

    Journal j;
    fseek(disco, prt.part_start, SEEK_SET);
    fseek(disco, sizeof(SuperBloque), SEEK_CUR);
    int aux = 0;
    string header = "digraph G{\n";
    do
    {
        fread(&j, sizeof(Journal), 1, disco);

        header.append("nodo");
        header.append(to_string(aux));
        aux++;
        header.append("[ shape = box; label = <\n");
        header.append("<table border=\'0\' cellborder=\'2\' width=\'100\' height=\'75\' color=\'LIGHTSTEELBLUE\'>\n");
        header.append("<tr>\n<td height=\'25\' colspan=\'3\'> ----- JOURNALING ----- </td>\n</tr>\n");
        header.append("<tr>\n");

        switch (j.journal_tipo_op)
        {
            case '1':
                header.append("<td height=\'30\' width=\'30\'>");
                header.append("Operacion");
                header.append("</td>\n");
                header.append("<td height=\'30\' width=\'30\'>");
                header.append("Nueva Carpeta");
                header.append("</td>\n");
                break;

            case '2':
                header.append("<td height=\'30\' width=\'30\'>");
                header.append("Operacion");
                header.append("</td>\n");
                header.append("<td height=\'30\' width=\'30\'>");
                header.append("Nuevo Archivo");
                header.append("</td>\n");
                break;
        }

        header.append("</tr>\n");
        header.append("<tr>\n");

        switch (j.journal_tipo)
        {
            case '0':
                header.append("<td height=\'30\' width=\'30\'>");
                header.append("Tipo");
                header.append("</td>\n");
                header.append("<td height=\'30\' width=\'30\'>");
                header.append("Archivo");
                header.append("</td>\n");
                break;

            case '1':
                header.append("<td height=\'30\' width=\'30\'>");
                header.append("Tipo");
                header.append("</td>\n");
                header.append("<td height=\'30\' width=\'30\'>");
                header.append("Carpeta");
                header.append("</td>\n");
                break;
        }

        header.append("</tr>\n");

        header.append("<tr>\n");
        header.append("<td height=\'30\' width=\'30\'>");
        header.append("Nombre");
        header.append("</td>\n");
        header.append("<td height=\'30\' width=\'30\'>");
        header.append(j.journal_nombre);
        header.append("</td>\n");
        header.append("</tr>\n");

        header.append("<tr>\n");

        switch (j.journal_contenido)
        {
            case '0':
                header.append("<td height=\'30\' width=\'30\'>");
                header.append("Contenido");
                header.append("</td>\n");
                header.append("<td height=\'30\' width=\'30\'>");
                header.append("No Hay");
                header.append("</td>\n");
                break;

            case '1':
                header.append("<td height=\'30\' width=\'30\'>");
                header.append("Contenido");
                header.append("</td>\n");
                header.append("<td height=\'30\' width=\'30\'>");
                header.append("Si Hay");
                header.append("</td>\n");
                break;
        }

        header.append("</tr>\n");

        header.append("<tr>\n");
        header.append("<td height=\'30\' width=\'30\'>");
        header.append("Fecha");
        header.append("</td>\n");
        header.append("<td height=\'30\' width=\'30\'>");
        header.append(j.journal_fecha);
        header.append("</td>\n");
        header.append("</tr>\n");

        header.append("</table>\n>\n];\n");

    } while (j.next != -1);
    
    header.append("}");
    fclose(disco);

    FILE *reporte = NULL;
    reporte = fopen("repJ.dot", "w+");
    if (reporte == NULL)
    {
        // ERROR
        cout << "\tERROR NO SE PUDO HACER EL REPORTE DEL DISCO..." << endl;
        getchar();
        return;
    }
    else
    {
        fputs(header.c_str(), reporte);
        fclose(reporte);

        try
        {
            string cmd = "dot -Tpng repJ.dot -o ";
            cmd.append(path);
            system(cmd.c_str());
            remove("repJ.dot");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void inodeRep(string path, string id)
{
    
}