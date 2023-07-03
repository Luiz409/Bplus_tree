#ifndef TYPEDFILE_H
#define TYPEDFILE_H

#include <fstream>
#include <type_traits>
using namespace std;

const ios::openmode mode = ios::in | ios::out | ios::binary;

template <class T, const unsigned int MIN_DEGREE>
class typedFile : private fstream {
   static_assert(is_base_of<serializable, T>::value, "T must be serializable");
   public:
      typedFile();
      typedFile(const string name, const string type, const unsigned int version, ios::openmode openmode = mode);
      virtual ~typedFile();
      bool open(const string name, const string type, const unsigned int version, ios::openmode openmode = mode);
      bool isOpen();
      bool close();
      void clear();
      bool readRecord(record<T, MIN_DEGREE> &r, unsigned long long int i);
      bool writeRecord(record<T, MIN_DEGREE> &r, unsigned long long int i);
      bool insertRecord(record<T, MIN_DEGREE> &r);
      bool deleteRecord(unsigned long long int i);
      unsigned long long int alocateNextPosition();
      unsigned long long int getFirstValid();
      unsigned long long int getFirstDeleted();
      unsigned long long int search(T data);
   protected:
      header head;
      bool readHeader(header &h);
      bool writeHeader(header &h);
      unsigned long long int index2pos(unsigned long long int i);
      unsigned long long int pos2index(unsigned long long int p);
};

//Construtor default não faz nada
template <class T, const unsigned int MIN_DEGREE>
typedFile<T, MIN_DEGREE>::typedFile() : fstream() {}

template <class T, const unsigned int MIN_DEGREE>
typedFile<T, MIN_DEGREE>::typedFile(const string name, const string type, const unsigned int version, ios::openmode openmode) : fstream(name.c_str(), mode) {
    if (isOpen()){
        this->readHeader(this->head);
        /*
        header h(type, version);
        if(!(this->head == h)){
            cout << "\nErro de versão.\n";
        }
        */
    } else {
       fstream::open(name.c_str(),ios::out);
       this->close();
       fstream::open(name.c_str(),openmode);
       this->head.setType(type);
       this->head.setVersion(version);
       this->head.setFirstValid(0);
       this->head.setFirstDeleted(0);

       this->writeHeader(this->head);

        }
    }


template <class T, const unsigned int MIN_DEGREE>
typedFile<T, MIN_DEGREE>::~typedFile() {}

template <class T, const unsigned int MIN_DEGREE>
bool typedFile<T, MIN_DEGREE>::open(const string name, const string type, const unsigned int version, ios::openmode openmode) {
    this->head.setType(type);
    this->head.setVersion(version);
    fstream::open(name, openmode);
    if(isOpen()){
        return this->writeHeader(this->head);
    }else{
        return false;
    }
}

template <class T, const unsigned int MIN_DEGREE>
bool typedFile<T, MIN_DEGREE>::isOpen() {
    return fstream::is_open();
}

template <class T, const unsigned int MIN_DEGREE>
bool typedFile<T, MIN_DEGREE>::close() {
    if (isOpen()){
        fstream::close();
        return true;
    } else {
        return false;
    }
}

template <class T, const unsigned int MIN_DEGREE>
void typedFile<T, MIN_DEGREE>::clear() {
    if (isOpen()){
        fstream::clear();
    }
}

template <class T, const unsigned int MIN_DEGREE>
bool typedFile<T, MIN_DEGREE>::readRecord(record<T, MIN_DEGREE> &r, unsigned long long int i) {
    if (isOpen()){
            char *var = new char[r.size()];
            this->clear();
            this->seekg(index2pos(i), ios::beg);
            this->read(var, r.size());
            string repr = string(var, r.size());
            r.fromString(repr);
            delete[] var;
            var = nullptr;
            return true;
        }
    else {
            return false;
    }
}

template <class T, const unsigned int MIN_DEGREE>
bool typedFile<T, MIN_DEGREE>::writeRecord(record<T, MIN_DEGREE> &r, unsigned long long int i) {
    if (isOpen()){
        string repr = r.toString();
        this->seekp(index2pos(i), ios::beg);
        this->write(repr.c_str(), r.size());
        return true;
    } else {
        return false;
    }
}

template <class T, const unsigned int MIN_DEGREE>
bool typedFile<T, MIN_DEGREE>::insertRecord(record<T, MIN_DEGREE> &r) {
    if (isOpen()){
        unsigned long long int recycle = this->getFirstDeleted();
        if (recycle != 0){
            record<T, MIN_DEGREE> r1;
            this->readRecord(r1, recycle);
            this->head.setFirstDeleted(r1.getNext());
            r.undel();
            r.setNext(this->head.getFirstValid());
            this->writeRecord(r, recycle);
            this->head.setFirstValid(recycle);
            return this->writeHeader(this->head);

        } else {
            r.setNext(this->head.getFirstValid());
            this->seekp(0, ios::end);
            unsigned long long int pos = this->tellp();
            this->writeRecord(r, pos2index(pos));
            this->head.setFirstValid(pos2index(pos));
            return this->writeHeader(this->head);
            }
    } else {
        return false;
    }
}

template <class T, const unsigned int MIN_DEGREE>
bool typedFile<T, MIN_DEGREE>::deleteRecord(unsigned long long int i) {
    if(isOpen()){
        if(this->getFirstValid() == i){
            record<T, MIN_DEGREE> r;
            this->readRecord(r,i);
            this->head.setFirstValid(r.getNext());
            this->head.setFirstDeleted(i);
            r.del();
            this->writeRecord(r,i);
            return this->writeHeader(this->head);

        }else{
           unsigned long long int x;
           unsigned long long int y;
           x = this->getFirstValid();
           record<T, MIN_DEGREE> r2;
           while(x != i){
                this->readRecord(r2,x);
                y = x;
                x = r2.getNext();
           }
           record<T, MIN_DEGREE> r3;
           this->readRecord(r3,x);
           r2.setNext(r3.getNext());
           r3.del();
           this->head.setFirstDeleted(x);
           this->writeRecord(r2,y);
           this->writeRecord(r3,x);
           return this->writeHeader(this->head);
        }
    }
}

template <class T, const unsigned int MIN_DEGREE>
unsigned long long int typedFile<T, MIN_DEGREE>::alocateNextPosition() {
     if (isOpen()){
        unsigned long long int recycle = this->getFirstDeleted();
        if (recycle != 0){
            record<T, MIN_DEGREE> r1;
            this->readRecord(r1, recycle);
            this->head.setFirstDeleted(r1.getNext());
            return recycle;
        } else {
            seekp(0,ios::end);
            unsigned long long int pos = fstream::tellp();
            return pos2index(pos);
            }
    } else {
        return false;
    }
}

template <class T, const unsigned int MIN_DEGREE>
unsigned long long int typedFile<T, MIN_DEGREE>::getFirstValid() {
    this->readHeader(this->head);
    return this->head.getFirstValid();
}

template <class T, const unsigned int MIN_DEGREE>
unsigned long long int typedFile<T, MIN_DEGREE>::getFirstDeleted() {
    this->readHeader(this->head);
    return head.getFirstDeleted();
}

template <class T, const unsigned int MIN_DEGREE>
unsigned long long int typedFile<T, MIN_DEGREE>::search(T data) {
    if(isOpen()){
        unsigned long long int i = this->getFirstValid();
        record<T, MIN_DEGREE> r;
        this->readRecord(r,i);
        T d;
        d = r.getData();
        while(d != data){
            i = r.getNext();
            this->readRecord(r,i);
            d = r.getData();
        }
        return i;
    }
}

template <class T, const unsigned int MIN_DEGREE>
bool typedFile<T, MIN_DEGREE>::readHeader(header &h) {
    if (isOpen()){
        char *aux = new char [h.size()];
        this->clear();
        this->seekg(0, ios::beg);
        this->read(aux, h.size());
        string repr = string(aux, h.size());
        h.fromString(repr);
        delete[] aux;
        aux = nullptr;
        return true;
    } else {
        return false;
    }
}

template <class T, const unsigned int MIN_DEGREE>
bool typedFile<T, MIN_DEGREE>::writeHeader(header &h) {
    if (isOpen()){
        string repr = h.toString();
        this->seekp(0, ios::beg);
        this->write(repr.c_str(), h.size());
        return true;
    } else {
        return false;
    }
}

template <class T, const unsigned int MIN_DEGREE>
unsigned long long int typedFile<T, MIN_DEGREE>::index2pos(unsigned long long int i) {
    record<T, MIN_DEGREE> r;
    return (head.size()+(i-1)*r.size());
}

template <class T, const unsigned int MIN_DEGREE>
unsigned long long int typedFile<T, MIN_DEGREE>::pos2index(unsigned long long int p) {
    record<T, MIN_DEGREE> r;
    return (p - head.size())/r.size()+1;
}
#endif // TYPEDFILE_H
