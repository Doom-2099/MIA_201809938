#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <dirent.h>
#include <fstream>
#include <string.h>
#include "Structs.h"

using namespace std;

string toLowerCase(string str);
bool is_dir(string path);
bool is_file(string path);
int calculate_size(int size, char unit);
vector<BusyBlock> getBusyBlocks(vector<Particion> part);
vector<FreeBlock> getFreeBlocks(vector<BusyBlock> blocks, int sizeDsk, int startDsk);
string getPath(string path, string name);
string getPathWithName(string path);