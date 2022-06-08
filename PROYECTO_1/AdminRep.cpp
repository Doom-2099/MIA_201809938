#include "AdminRep.h"

void dskRep(string path)
{   

    // ------------------------
    // ---- RECUPERAR MBR -----
    // ------------------------
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

    /* vector<Particion> particiones;
    vector<BusyBlock> bblocks = getBusyBlocks(particiones);
    vector<FreeBlock> fblocks = getFreeBlocks(bblocks, mbr.mbr_size, aux); */

    FILE *reporte = NULL;
    reporte = fopen("rep.dot", "w+");
    if(reporte == NULL)
    {
        // ERROR
    }
    else
    {
        string header = "digraph G{\ndsk[ shape = box; label = <\n";
        header.append("<table border=\'0\' cellborder=\'2\' width=\'600\' height=\'75\' color=\'LIGHTSTEELBLUE\'>\n");
        header.append("<tr>\n<td height=\'75\' width=\'100\'> MBR </td>\n");

        // ------------------------
        // ---- RECORRER PARTICIONES PARA EL DISCO
        // ------------------------


        header.append("</tr>\n</table>\n>\n];\n}");
        fputs(header.c_str(), reporte);
        fclose(reporte);
        system("dot -Tpng rep.dot -o rep.png");
        remove("rep.png");
    }
}

void mbrRep()
{
    /* code */
}