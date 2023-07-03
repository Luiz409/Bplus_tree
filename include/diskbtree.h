#ifndef DISKBTREE_H_INCLUDED
#define DISKBTREE_H_INCLUDED
#include<iostream>
#include <string>
#include <vector>
#include <queue>
#include "typedFile.h"
#include "node.h"
#include "record.h"

using namespace std;


template <class T,const unsigned int MIN_DEGREE>
class diskbtree: private typedFile<T, MIN_DEGREE>{
    static_assert(is_base_of<serializable, T>::value, "T must be serializable");
public:
    diskbtree();
    diskbtree(const string name, const string type, const unsigned int version);
    ~diskbtree();
    bool insertKey(T key);
    node<T,MIN_DEGREE> getRoot(){return this->root;};
    bool searchKey(node <T, MIN_DEGREE> r,T key);
    void printTree(); //Usado somente para testes.
    void print();
    bool removeKey(T key);
private:

    node<T,MIN_DEGREE> root;

    bool insertInNodeNonFull(node<T,MIN_DEGREE>& x,unsigned long long int index,T key, int iX);
    void splitChild(node<T,MIN_DEGREE>& x,int i, int iX);

    bool writeNode(node<T,MIN_DEGREE> x, unsigned long long int i);
    node<T,MIN_DEGREE> readNode(unsigned long long int i);

    //Auxiliar do removeKey();
    bool removeAux(node <T, MIN_DEGREE> r,T key, unsigned long long int index);

    //Auxiliar do print();
    void printAux(node<T, MIN_DEGREE> x, vector<string> &v, unsigned int lvl);

    static const unsigned int MAX = 2 * MIN_DEGREE - 1;
    static const unsigned int MIN = MIN_DEGREE - 1;
    static const unsigned int NOT_FOUND = -1;
};

template <class T, const unsigned int MIN_DEGREE>
diskbtree<T, MIN_DEGREE>::~diskbtree(){
    typedFile<T, MIN_DEGREE>::close();
}

//Construtor default não faz nada.
template <class T, const unsigned int MIN_DEGREE>
diskbtree<T, MIN_DEGREE>::diskbtree():typedFile<T, MIN_DEGREE>::typedFile() {}


//Construtor parametrizado cria um arquivo tipado e determina uma raiz vazia.
template <class T, const unsigned int MIN_DEGREE>
diskbtree<T, MIN_DEGREE>::diskbtree(const string name, const string type, const unsigned int version):typedFile<T, MIN_DEGREE>::typedFile(name, type, version){
    if(typedFile<T, MIN_DEGREE>::isOpen()){
        unsigned int i;
        i = typedFile<T, MIN_DEGREE>::getFirstValid();
        if(i == 0){
            node<T, MIN_DEGREE> n;
            n.setLeaf(true);
            n.setSize(0);
            this->root = n;

            record<T, MIN_DEGREE> r;
            r.setData(n);
            typedFile<T, MIN_DEGREE>::insertRecord(r);
        }
        else{
            this->root = this->readNode(i);
        }
    }
    else{
        cout<<endl<<"Error"<<endl;
    }
}

template <class T, const unsigned int MIN_DEGREE>
bool diskbtree<T, MIN_DEGREE>::insertKey(T key){
    node<T, MIN_DEGREE>r;
    unsigned long long int i;
    i = typedFile<T, MIN_DEGREE>::getFirstValid();
    r = this->readNode(i);
    if (r.getSize() == MAX){
        node<T, MIN_DEGREE> s;
        s.setLeaf(false);
        s.setSize(0);
        s.setChildren(0,i);

        record<T, MIN_DEGREE> r1;
        r1.setData(s);
        typedFile<T, MIN_DEGREE>::insertRecord(r1);     //Inserindo a raiz no arquivo tipado
        i = typedFile<T, MIN_DEGREE>::getFirstValid();
        this->splitChild(s,0,i);
        this->insertInNodeNonFull(s,i,key,i);
        this->root = s;
    }
    else{
        this->insertInNodeNonFull(r,i,key,i);
        this->root = r;
    }
}

//A remoção infelizmente atende apenas ao caso 1.
template <class T, const unsigned int MIN_DEGREE>
bool diskbtree<T, MIN_DEGREE>::removeKey(T key){
    node<T, MIN_DEGREE>r;
    unsigned long long int i;
    i = typedFile<T, MIN_DEGREE>::getFirstValid();
    r = this->readNode(i);
    this->removeAux(r,key,i);
}

//O auxiliar da remoção é basicamente um searchKey() que remove ao invés de mostrar a chave.
template <class T, const unsigned int MIN_DEGREE>
bool diskbtree<T, MIN_DEGREE>::removeAux(node <T, MIN_DEGREE> r,T key, unsigned long long int index){
    int i = 0;
    while(i <= r.getSize()-1 && key > r.getkey(i)){
        i++;
    }
    if(i <= r.getSize()-1 &&  key == r.getkey(i)){
        if(r.isleaf()){
            for (int j = i; j < r.getSize(); j++)
            {
                r.setKey(j,r.getkey(j+1));
            }
            r.setSize(r.getSize()-1);
            record<T, MIN_DEGREE> r1;
            r1.setData(r);
            typedFile<T, MIN_DEGREE>::writeRecord(r1,index);

            cout <<"Chave excluída"<<endl;
            return true;
        }
        else{
            cout<<"Não foi possível remover"<<endl;
            return true;
        }
    }
    if(r.isleaf()){
        cout << "Chave não encontrada"<<endl;
        return true;
    }
    else{
        unsigned long long int p = r.getChildren(i);
        r = this->readNode(p);
        removeAux(r,key,p);
    }
}

//Função padrão de acordo com o material didático
template <class T, const unsigned int MIN_DEGREE>
bool diskbtree<T, MIN_DEGREE>::insertInNodeNonFull(node<T,MIN_DEGREE>& x,unsigned long long int index,T key, int iX){
    int i = x.getSize() - 1;

    if(x.isleaf()){
        while(i >= 0 && key < x.getkey(i)){
            x.setKey(i + 1, x.getkey(i));
            --i;
        }
        x.setKey(i+1, key);
        x.setSize(x.getSize()+1);
        return this->writeNode(x,index);
    }
    else{
        while(i >= 0 && key < x.getkey(i)){
            i--;
        }
        i++;
        node<T, MIN_DEGREE> n;
        n = this->readNode(x.getChildren(i));
        if(n.getSize() == MAX){
            this->splitChild(x,i,iX);
            if (key > x.getkey(i)){
                i++;
            }
            n = this->readNode(x.getChildren(i));
        }
        this->insertInNodeNonFull(n,x.getChildren(i),key,x.getChildren(i));
        return true;
    }
}

//Função padrão de acordo com o material didático
template <class T, const unsigned int MIN_DEGREE>
void diskbtree<T, MIN_DEGREE>::splitChild(node<T,MIN_DEGREE>& x, int i, int iX){
    node <T, MIN_DEGREE> y,z;
    y = this->readNode(x.getChildren(i));
    z.setLeaf(y.isleaf());
    z.setSize(MIN);

    for (unsigned int j = 0; j < MIN; j++){
        z.setKey(j,y.getkey(j+MIN_DEGREE));
    }

    if(!y.isleaf()){
        for (unsigned int j = 0; j < MIN_DEGREE; j++){
            z.setChildren(j, y.getChildren(j + MIN_DEGREE));
        }
    }

    y.setSize(MIN);

//  for (int j = x.getSize()-1; j > i  ; j--){
    for (int j = x.getSize(); j > i  ; j--){
        x.setChildren(j+1, x.getChildren(j));
    }

    record<T, MIN_DEGREE> r1;
    r1.setData(z);
    unsigned long long int p = typedFile<T, MIN_DEGREE>::alocateNextPosition();
    typedFile<T, MIN_DEGREE>::writeRecord(r1,p);
    x.setChildren(i+1,p);

//  for (int j = x.getSize()-1; j > i; j--){
    for (int j = x.getSize()-1; j >= i; j--){
        x.setKey(j+1, x.getkey(j));
    }

    x.setKey(i, y.getkey(MIN_DEGREE-1));
    x.setSize(x.getSize() + 1);

    record<T, MIN_DEGREE> r2;
    r2.setData(y);
    typedFile<T, MIN_DEGREE>::writeRecord(r2,x.getChildren(i));

    record<T, MIN_DEGREE> r3;
    r3.setData(x);
    typedFile<T, MIN_DEGREE>::writeRecord(r3,iX);
}

//Função padrão de acordo com o material didático.
template <class T, const unsigned int MIN_DEGREE>
bool diskbtree<T, MIN_DEGREE>::searchKey(node <T, MIN_DEGREE> r,T key){
    int i = 0;
    while(i <= r.getSize()-1 && key > r.getkey(i)){
        i++;
    }
    if(i <= r.getSize()-1 &&  key == r.getkey(i)){
        cout << "Nó da chave: ";
        r.printNode();
        cout << ", chave encontrada na posicao "<<i<<endl;
        return true;
    }
    if(r.isleaf()){
        cout << "Chave não encontrada"<<endl;
        return true;
    }
    else{
        r = this->readNode(r.getChildren(i));
        searchKey(r,key);
    }
}

//Função para testes
template <class T, const unsigned int MIN_DEGREE>
void diskbtree<T, MIN_DEGREE>::printTree(){
    queue <node <T, MIN_DEGREE>> print;

    node <T, MIN_DEGREE> r;
    r = readNode(typedFile<T, MIN_DEGREE>::getFirstValid());

    print.push(r);
    int i = 0;
    while(print.front().getSize() > 0){
        print.front().printNode();
        for(int i = 0; i <= print.front().getSize(); i++){
            if (print.front().getChildren(i)!= NOT_FOUND){
                print.push(readNode(print.front().getChildren(i)));
            }
        }
        print.pop();
        i++;
    }
}

//Função necessária para utilizar a árvore B junto com o arquivo tipado
template <class T, const unsigned int MIN_DEGREE>
node<T, MIN_DEGREE> diskbtree<T, MIN_DEGREE>::readNode(unsigned long long int i){
    record<T, MIN_DEGREE> r;
    typedFile<T, MIN_DEGREE>::readRecord(r, i);
    return r.getData();
}

//Função necessária para utilizar a árvore B junto com o arquivo tipado
template <class T, const unsigned int MIN_DEGREE>
bool diskbtree<T, MIN_DEGREE>::writeNode(node<T,MIN_DEGREE> x, unsigned long long int i){
    record<T, MIN_DEGREE> r(x);
    return typedFile<T, MIN_DEGREE>::writeRecord(r,i);
}

//Função padrão de acordo com o material didático (memTree)
template <class T, const unsigned int MIN_DEGREE>
void diskbtree<T, MIN_DEGREE>::print(){
    int lvl = 0;
    vector<string> levels(1);
    printAux(this->root, levels, lvl);

    for (string s : levels){
        cout << s << endl;
    }
}

//Função padrão de acordo com o material didático (memTree)
template <class T, const unsigned int MIN_DEGREE>
void diskbtree<T, MIN_DEGREE>::printAux(node<T, MIN_DEGREE> x, vector<string> &v, unsigned int lvl){
    string aux = "[";
    int i = 0;

    if (v.size() < lvl + 1){
        v.resize(lvl + 1);
    }

    if (!x.isleaf()){
        for (i = 0; i <= x.getSize(); i++){
            if (x.getChildren(i) != NOT_FOUND){
                node<T, MIN_DEGREE> n1 = this->readNode(x.getChildren(i));
                printAux(n1, v, lvl + 1);
            }
        }
    }

    for (i = 0; i < x.getSize(); i++){
        if(x.getkey(i).getY() < 0){
            int aux2 = x.getkey(i).getY()*-1;
            if(aux2 < 10 && aux2 >= 0){
                aux += to_string(x.getkey(i).getX()) + ".0"+to_string(aux2)+", ";
            }
            else{
                aux += to_string(x.getkey(i).getX()) + "."+to_string(aux2)+", ";
            }
        }else{
        if(x.getkey(i).getY() < 10 && x.getkey(i).getY() >= 0){
            aux += to_string(x.getkey(i).getX()) + ".0"+to_string(x.getkey(i).getY())+", ";
        }
        else{
            aux += to_string(x.getkey(i).getX()) + "."+to_string(x.getkey(i).getY())+", ";
        }
        }
    }

    if (aux.length() > 1){
        aux += "\b\b] ";
    }
    else{
        aux += "] ";
    }

    v[lvl] += aux;
}

#endif // DISKBTREE_H_INCLUDED
