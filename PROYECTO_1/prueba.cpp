#include <iostream>

using namespace std;

int main()
{
    // ------------------------
    // ---- RECUPERAR MBR -----
    // ------------------------
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
        header.append("<tr>\n");


        header.append("</tr>\n</table>\n>\n];\n}");
        fputs(header.c_str(), reporte);
        fclose(reporte);
    }

    return 0;
}