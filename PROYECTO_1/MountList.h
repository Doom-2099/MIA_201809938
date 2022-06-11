#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

template <class T> class Nodo
{
    private:
        T dato;
    
    public:
        Nodo *siguiente;

        Nodo()
        {
            siguiente = NULL;
        }

        T getDato()
        {
            return dato;
        }

        void setDato(T data)
        {
            dato = data;
        }
};


template <class T> class Lista
{
    public:
        Nodo<T> *root;

        Lista()
        {
            root = NULL;
        }

        void insert(T dato)
        {
            if (root == NULL)
            {
                Nodo<T> *nodo = new Nodo<T>();
                nodo->setDato(dato);
                root = nodo;
                cout << "PARTICION MONTADA" << endl;
                getchar();
            }
            else
            {
                Nodo<T> *nodo = new Nodo<T>();
                Nodo<T> *aux = root;
                nodo->setDato(dato);

                while (aux->siguiente != NULL)
                {
                    aux = aux->siguiente;
                }

                aux->siguiente = nodo;
                cout << "PARTICION MONTADA" << endl;
            }
        }

        bool isEmpty()
        {
            return root == NULL ? true : false;
        }

        void print()
        {
            Nodo<T> *aux = root;
            
            cout << "\t-------------- PARTICIONES MONTADAS -------------------" << endl;
            while (aux != NULL)
            {
                cout << "\t---------------- PARTICION " << aux->getDato().name << "---------------" << endl;
                cout << "\tID: " << aux->getDato().id << endl;
                cout << "\tPATH: " << aux->getDato().path << endl;
                aux = aux->siguiente;
            }
        }

        int getNum(string path)
        {
            int cont = 1;
            Nodo<T> *aux = root;

            while (aux != NULL)
            {
                if(!strcmp(aux->getDato().path.c_str(), path.c_str()))
                {
                    cont++;
                }
            }
            
            return cont;
        }

        int remove(string id)
        {
            Nodo<T> *aux = root;

            while (aux->siguiente != NULL)
            {
                if(!strcmp(aux->siguiente->getDato().id.c_str(), id.c_str()))
                {
                    aux->siguiente = aux->siguiente->siguiente;
                    return 1;
                }
            }
            
            return 0;
        }
};