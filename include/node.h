#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include<iostream>
#include <vector>


using namespace std;

template <class T, const unsigned int MIN_DEGREE>
class node: public serializable {
static_assert(is_base_of<serializable, T>::value, "T must be serializable");
public:
    node();
    node(bool l);
    node(const node<T, MIN_DEGREE> &other);
    ~node();

    node<T, MIN_DEGREE> operator=(const node<T, MIN_DEGREE> &other);

    bool isleaf() const { return this->leaf; }
    void setLeaf(bool l){ this->leaf = l; }
    int getSize() const { return this->n; }
    void setSize(unsigned int x){ this->n = x; }
    void setChildren(unsigned int i, unsigned int index){ this->children[i] = index; }
    unsigned int getChildren(unsigned int i){ return this->children[i]; }
    T getkey(unsigned int i){ return this->keys[i]; }
    void setKey(unsigned int i, T k){this->keys[i]=k;};
    void printNode();
    virtual string toString();
    virtual void fromString(string repr);
    virtual string toCSV() { return ""; }
    virtual void fromCSV(string repr) {}
    virtual string toJSON() { return ""; }
    virtual void fromJSON(string repr) {}
    virtual string toXML() { return ""; }
    virtual void fromXML(string repr) {}
    virtual unsigned long long int size() const;

    static const unsigned int MAX = 2 * MIN_DEGREE - 1;
    static const unsigned int MIN = MIN_DEGREE - 1;
    static const unsigned int NOT_FOUND = -1;

private:
    vector<T> keys;
    vector<unsigned int> children;
    bool leaf;
    unsigned int n;
};

template <class T, const unsigned int MIN_DEGREE>
node<T, MIN_DEGREE>::node(): serializable (){
   this->setLeaf(true);
   this->setSize(0);
   keys.resize(MAX);
   children.resize(MAX + 1);

   for (unsigned int i = 0; i < children.size(); i++) {
      this->setChildren(i,NOT_FOUND);
   }
}

template <class T, const unsigned int MIN_DEGREE>
node<T, MIN_DEGREE>::node(bool l): serializable(){
   this->setLeaf(l);
   this->setSize(0);

   keys.resize(MAX);
   children.resize(MAX + 1);

   for (unsigned int i = 0; i < children.size(); i++) {
      this->setChildren(i,NOT_FOUND);
   }
}

template <class T, const unsigned int MIN_DEGREE>
node<T, MIN_DEGREE>::~node(){}

template <class T, const unsigned int MIN_DEGREE>
node<T, MIN_DEGREE>::node(const node<T, MIN_DEGREE> &other): serializable (){
    this->setLeaf(other.isleaf());
    this->setSize(other.getSize());
    keys.resize(MAX);
    children.resize(MAX + 1);

    this->children = other.children;
    this->keys = other.keys;
}

template <class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>:: printNode(){
    cout << " [";
    T aux;

    for (unsigned int i = 0; i<this->getSize(); i++){
        if(i < this->getSize()-1){
            aux = this->keys[i];
            cout << aux<<" ,";
        }else{
            aux = this->keys[i];
            cout << aux;
        }

    }
    cout << "] ";
}

template <class T, const unsigned int MIN_DEGREE>
string node<T, MIN_DEGREE>::toString() {

    string repr = "";
    repr += string(reinterpret_cast<char*>(&this->leaf), sizeof(this->leaf));
    repr += string(reinterpret_cast<char*>(&this->n), sizeof(this->n));

    for(unsigned int i=0; i< keys.size(); i++){
        repr += keys[i].toString();
    }

    for(unsigned int i=0; i< children.size(); i++){
        repr += string(reinterpret_cast<char*>(&this->children[i]), sizeof(this->children[i]));
    }
    return repr;
}

template <class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::fromString(string repr) {
    unsigned int pos = 0;

    repr.copy(reinterpret_cast<char*>(&this->leaf), sizeof(this->leaf), pos);
    pos += sizeof(this->leaf);

    repr.copy(reinterpret_cast<char*>(&this->n), sizeof(this->n), pos);
    pos += sizeof(this->n);

    string data_string = "";
    for(unsigned int i=0; i< keys.size(); i++){
        data_string = repr.substr(pos,keys[i].size());
        keys[i].fromString(data_string);
        pos += keys[i].size();
        data_string.clear();
    }

    for(unsigned int i=0; i< children.size(); i++){
        repr.copy(reinterpret_cast<char*>(&this->children[i]), sizeof(this->children[i]), pos);
        pos += sizeof(this->children[i]);
    }
}

template <class T, const unsigned int MIN_DEGREE>
unsigned long long int node<T, MIN_DEGREE>::size() const {
    return keys.size()*keys[0].size() + children.size()*sizeof(children[0]) + sizeof(leaf) + sizeof(n);
}

template <class T, const unsigned int MIN_DEGREE>
node<T, MIN_DEGREE> node<T, MIN_DEGREE>::operator=(const node<T, MIN_DEGREE>& other) {
    node<T, MIN_DEGREE> aux(other);
    this->children = other.children;
    this->keys = other.keys;    //?????
    this->leaf = other.isleaf(); //??????
    this->n = other.getSize();
    return aux;
}

#endif // NODE_H_INCLUDED
