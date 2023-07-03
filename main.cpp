#include <iostream>
#include <clocale>
#include <vector>

#include "header.h"
#include "record.h"
#include "typedFile.h"
#include "serialCurrency.h"
#include "node.h"
#include "diskbtree.h"

using namespace std;

int main() {
//OBS: Alguns valores podem apresentar um erro de 0.01 quando imprimir.

   setlocale(LC_ALL, "Portuguese");
   diskbtree <serialCurrency,3> tree("data.dat","tree",1);
   vector<serialCurrency> vetTest;
   vector<serialCurrency> vetRemoveTest;
   serialCurrency c;
   node <serialCurrency,3> n1;
   int n;
   double x;

   c.setValue(5.50);
   vetTest.push_back(c);
   c.setValue(2.01);
   vetTest.push_back(c);
   c.setValue(-5.33);
   vetTest.push_back(c);
   c.setValue(10.05);
   vetTest.push_back(c);
   c.setValue(21.13);
   vetTest.push_back(c);
   c.setValue(1.88);
   vetTest.push_back(c);
   c.setValue(-3.14);
   vetTest.push_back(c);
   c.setValue(2.86);
   vetTest.push_back(c);
   c.setValue(37.55);
   vetTest.push_back(c);
   c.setValue(2.99);
   vetTest.push_back(c);
   c.setValue(34.31);
   vetTest.push_back(c);
   c.setValue(49.00);
   vetTest.push_back(c);
   c.setValue(38.26);
   vetTest.push_back(c);
   c.setValue(20.85);
   vetTest.push_back(c);
   c.setValue(23.02);
   vetTest.push_back(c);
   c.setValue(17.01);
   vetTest.push_back(c);
   c.setValue(14.92);
   vetTest.push_back(c);
   c.setValue(13.81);
   vetTest.push_back(c);
   c.setValue(39.89);
   vetTest.push_back(c);
   c.setValue(-13.22);
   vetTest.push_back(c);
   c.setValue(7.20);
   vetTest.push_back(c);

   c.setValue(5.50);
   vetRemoveTest.push_back(c);
   c.setValue(-5.33);
   vetRemoveTest.push_back(c);
   c.setValue(21.13);
   vetRemoveTest.push_back(c);
   c.setValue(1.88);
   vetRemoveTest.push_back(c);
   c.setValue(-3.14);
   vetRemoveTest.push_back(c);
   c.setValue(34.31);
   vetRemoveTest.push_back(c);
   c.setValue(38.26);
   vetRemoveTest.push_back(c);
   c.setValue(23.02);
   vetRemoveTest.push_back(c);
   c.setValue(37.55);
   vetRemoveTest.push_back(c);
   c.setValue(14.92);
   vetRemoveTest.push_back(c);
   c.setValue(39.89);
   vetRemoveTest.push_back(c);
   c.setValue(7.20);
   vetRemoveTest.push_back(c);

    while(true){
        cout <<"----------------------"<<endl;
        cout <<"  Árvore B em disco!"<<endl;
        cout <<"Escolha uma opção: "<<endl;
        cout <<"1 - Inserir chave."<<endl;
        cout <<"2 - Pesquisar Chave."<<endl;
        cout <<"3 - Excluir Chave."<<endl;
        cout <<"4 - Teste automatizado"<<endl;
        cout <<"5 - Mostrar Árvore"<<endl;
        cout <<"6 - Sair"<<endl;
        cout <<"----------------------"<<endl;
        cout <<"Opção: ";
        cin >> n;

        switch(n){
            case 1:
                cout << "Digite o valor (double): ";
                cin >> x;
                c.setValue(x);
                tree.insertKey(c);
                cout << "Valor "<<c<<" inserido com sucesso"<<endl<<endl;
                break;
            case 2:
                cout << "Digite o valor (double): ";
                cin >> x;
                c.setValue(x);
                n1 = tree.getRoot();
                tree.searchKey(n1,c);
                cout<<endl<<endl;
                break;
            case 3:
                cout << "Digite o valor (double): ";
                cin >> x;
                c.setValue(x);
                tree.removeKey(c);
                break;
            case 4:
                cout << "Iniciando teste automatizado"<<endl;
                for (int i = 0;i < vetTest.size();i++){
                    tree.insertKey(vetTest[i]);
                    cout << "Árvore após inserir "<< vetTest[i]<<endl;
                    tree.print();
                    cout<<endl;
                }
                cout << "Árvore após todas as inserções: "<<endl<<endl;
                tree.print();
                cout<<endl<<endl;
                for (int i = 0;i < vetRemoveTest.size();i++){
                    cout << "Árvore após remover "<< vetRemoveTest[i]<<endl;
                    tree.removeKey(vetRemoveTest[i]);
                    tree.print();
                    cout<<endl;
                }
                break;
            case 5:
                tree.print();
                break;
            case 6:
                return 0;
        }
    }
}
