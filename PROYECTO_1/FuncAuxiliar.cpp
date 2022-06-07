#include "FuncAuxiliar.h"

vector<string> split(string str, char pattern)
{
    int init = 0;
    int found = 0;
    string splitted;
    vector<string> results;

    while (found >= 0)
    {
        found = str.find(pattern, init);
        splitted = str.substr(init, found - init);
        init = found + 1;
        results.push_back(splitted);
    }

    return results;
}

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
        if (strcmp(part[i].part_name, "nn") != 0)
        {
            BusyBlock bb;
            bb.start = part[i].part_start;
            bb.end = bb.start + part[i].part_size;
            busyblocks.push_back(bb);
        }
    }

    sort(busyblocks.begin(), busyblocks.end(), [](BusyBlock &b1, BusyBlock &b2)
         { return b1.start < b2.start; });

    cout << "\t--------------- BUSY BLOCKS --------------------" << endl;

    for(int i = 0; i < busyblocks.size(); i++)
    {
        cout << "\t" << busyblocks[i].start << "\t" << (busyblocks[i].end - busyblocks[i].start) << endl;
    }

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

    cout << "\t--------------- FREE BLOCKS --------------------" << endl;

    for(int i = 0; i < freeBlocks.size(); i++)
    {
        cout << "\t" << freeBlocks[i].start << "\t" <<freeBlocks[i].size <<endl;
    }

    return freeBlocks;
}