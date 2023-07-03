#include "serialCurrency.h"
#include <cmath>

serialCurrency::serialCurrency() : serializable() {
    this->x = 0;
    this->y = 0;
}

serialCurrency::serialCurrency(double n) : serializable() {
    this->x = trunc(n);
    n = n-this->x;
    n = n*100;
    this->y = n;
    if (this->y >= 100){
        this->x++;
        this->y = this->y-100;
    }
}

serialCurrency::serialCurrency(const serialCurrency &other) {
    this->x = other.getX();
    this->y = other.getY();
}

serialCurrency::~serialCurrency() {}

serialCurrency serialCurrency::operator=(const serialCurrency &other) {
    serialCurrency aux(other);
    this->x = other.getX();
    this->y = other.getY();
    return aux;
}

bool serialCurrency::operator==(const serialCurrency &other) const {
    return (this->x == other.getX() && this->y == other.getY());
}

bool serialCurrency::operator<(const serialCurrency &other) const {
   if(this->x == other.getX()){
        return (this->y < other.getY());
   }else{
        return (this->x < other.getX());
   }
}

bool serialCurrency::operator<=(const serialCurrency &other) const {
   if(this->x == other.getX()){
        return (this->y <= other.getY());
   }else{
        return (this->x <= other.getX());
   }
}

bool serialCurrency::operator>(const serialCurrency &other) const {
    if(this->x == other.getX()){
        return (this->y > other.getY());
   }else{
        return (this->x > other.getX());
   }
}

bool serialCurrency::operator>=(const serialCurrency &other) const {
   if(this->x == other.getX()){
        return (this->y >= other.getY());
   }else{
        return (this->x >= other.getX());
   }
}

bool serialCurrency::operator!=(const serialCurrency &other) const {
    if(this->x == other.getX()){
        return (this->y != other.getY());
   }else{
        return (this->x != other.getX());
   }
}

serialCurrency serialCurrency::operator+(const serialCurrency& other) const {
    double n;
    n = (this->x + other.getX()) + ((this->y + other.getY())/10);
    serialCurrency aux(n);
    if(aux.getY() >=100){
        aux.setX(aux.getX()+1);
        aux.setY(aux.getY()-100);
    }
    return aux;
}

serialCurrency serialCurrency::operator-(const serialCurrency& other) const {
    double n;
    n = (this->x - other.getX()) + ((this->y - other.getY())/10);
    serialCurrency aux(n);
    return aux;
}

serialCurrency serialCurrency::operator*(const serialCurrency& other) const { //Error
    double n;
    n = (this->x * other.getX()) + ((this->y * other.getY())/10);
    serialCurrency aux(n);
    if(aux.getY() >=100){
        aux.setX(aux.getX()+1);
        aux.setY(aux.getY()-100);
    }
    return aux;
}

serialCurrency serialCurrency::operator/(const serialCurrency& other) const { //Error
    double n;
    n = (this->x/other.getX()) + ((this->y/other.getY())/10);
    serialCurrency aux(n);
    return aux;
}

serialCurrency serialCurrency::operator++() {
    this->x+1;
}

serialCurrency serialCurrency::operator--() {
    this->x-1;
}

void serialCurrency::setX(int x) {
    this->x = x;
}

int serialCurrency::getX() const {
    return this->x;
}

void serialCurrency::setY(int y) {
    this->y = y;
}

int serialCurrency::getY() const {
    return this->y;
}

void serialCurrency::setValue(double n) {
    this->x = trunc(n);
    n = n-this->x;
    n = n*100;
    this->y = n;
    if (this->y >= 100){
        this->x++;
        this->y = this->y-100;
    }
}

string serialCurrency::toString() {
    string repr = "";
    repr += string(reinterpret_cast<char*>(&this->x), sizeof(x));
    repr += string(reinterpret_cast<char*>(&this->y), sizeof(y));
    return repr;
}

void serialCurrency::fromString(string repr) {
    int pos = 0;
    repr.copy(reinterpret_cast<char*>(&this->x), sizeof(x), pos);
    pos += sizeof(x);
    repr.copy(reinterpret_cast<char*>(&this->y), sizeof(y), pos);
}

unsigned long long int serialCurrency::size() const {
    return sizeof(x)+sizeof(y);
}
