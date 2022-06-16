#pragma once

#include <iostream>
#include "AdminDiscos.h"
#include "FuncAuxiliar.h"

using namespace std;

void mkfile(string id, string path, bool p, int size, string cont);
void mkdir(string id, string path, bool p);
void rm(string id, vector<string>files);
void cp(string id, string path, string dest, string iddest);
void find(string id, string path, string name);
void ls(string id, string path, char order);
void cat(string id, vector<string> filen);
void recovery(string id);
void loss(string id);
