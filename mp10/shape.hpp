#ifndef SHAPE_H_
#define SHAPE_H_

#include<iostream>
#include<cmath>
#include<string>
#include<algorithm>
#include<list>
#include<fstream>
#include <iomanip>    

using namespace std;

/*
Shape
 - first create constructor that just assigns the paramater to the member
 - getName() returns the name member
Rectangle
 - same constructor and getters approach
 - then write + and - operator overload and make sure to cast 0 to type T, use getters and setters of passed functions
Circle
 - same approach as rectangle, but no need for type T
 - use M_PI for pi in cmath
Sphere
 - same approach, but make sure to cast to double or use 4.0 instead of 4.

then write createShapes() function which reads from a text file using ifstream and goes line by line, 
depending on what the shape name is, pass in the correct amount of variables and create the class and add it to the list

then write maxArea using iterators to go through entire list by derefrencing iterator and then accessing getArea(), and continually update max_area if need to.
do same for maxVolume()
*/

//Base class
//Please implement Shape's member functions
//constructor, getName()
class Shape{
public:
	//Base class' constructor should be called in derived classes'
	//constructor to initizlize Shape's private variable 
  	Shape(string name) {
		name_ = name; 
	}
	
  	string getName() {
		return name_;
	}
	
  	virtual double getArea() const = 0;
  	virtual double getVolume() const = 0;
	
private:
  string name_;

};

//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-

template <class T>
class Rectangle : public Shape{
public:
  	Rectangle<T>(T width = 0, T length = 0):Shape("Rectangle") {
		width_ = width;
		length_ = length;
	}
	
  	double getArea() const {
		return (double) (width_ * length_); // cast to double because don't know what type acutally is
	}
	
  	double getVolume() const {
		return (double) 0; // no volume for 2d, but still cast to double
	}
	
	Rectangle<T> operator + (const Rectangle<T>& rec) {
		return Rectangle<T>(width_ + rec.getWidth(), length_ + rec.getLength()); // use getters because its a private variable
	}
	
	Rectangle<T> operator - (const Rectangle<T>& rec) {
		return Rectangle<T>(max((T) 0, width_ - rec.getWidth()), max((T) 0, length_ - rec.getLength())); // cast 0 to whatever type is being used through (T), formula provided in canvas
	} 
	
	T getWidth() const { 
		return width_;
	}
	
	T getLength() const { 
		return length_;
	}
private:
	T width_;
    T length_;

};

//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Circle : public Shape{
public:
  	Circle(double radius):Shape("Circle") {
		radius_ = radius;
	}
  	double getArea() const{
		return M_PI * radius_ * radius_; // M_PI is pi available in cmath
	}
	
 	double getVolume() const{
		return (double) 0;
	}
	
  	Circle operator + (const Circle& cir){
		return Circle(radius_ + cir.getRadius()); // use getters like before
	}
	
	Circle operator - (const Circle& cir){
		return Circle(max((double) 0, radius_ - cir.getRadius())); // formula in canvas
	} 

	double getRadius() const { 
		return radius_;
	}
	
private:
	double radius_;

};



//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Sphere : public Shape{
public:
  	Sphere(double radius):Shape("Sphere") {
		radius_ = radius;
	}

  	double getVolume() const {
		return (double) ((4.0 / 3.0) * M_PI * radius_ * radius_ * radius_); // use 4.0 / 3.0 instead of 4/3 to get a double output
	}	
	
  	double getArea() const {
		return (double) (4.0 * M_PI * radius_ * radius_);
	}

	Sphere operator + (const Sphere& sph) {
		return Sphere(radius_ + sph.getRadius()); 
	}

	Sphere operator - (const Sphere& sph) {
		return Sphere(max((double) 0, radius_ - sph.getRadius())); // same stuff
	} 
	
	double getRadius() const {
		return radius_;
	}

private:
	double radius_;

};

//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
class RectPrism : public Shape{
public:
  	RectPrism(double width, double length, double height):Shape("RectPrism") {
		width_ = width;
		length_ = length;
		height_ = height;
	}
	
  	double getVolume() const {
		return width_ * length_ * height_;
	}
  	
	double getArea() const {
		return (double) (2.0 * (width_ * length_ + width_ * height_ + length_ * height_));
	}
	
	RectPrism operator + (const RectPrism& rectp){
		return RectPrism(width_ + rectp.getWidth(), length_ + rectp.getLength(), height_ + rectp.getHeight());
	}
	
	RectPrism operator - (const RectPrism& rectp){
        return RectPrism(max(0.0, width_ - rectp.getWidth()),
                         max(0.0, length_ - rectp.getLength()),
                         max(0.0, height_ - rectp.getHeight())); // formula provided in canvas
	}	
	
	double getWidth() const { 
		return width_;
	}
	
	double getLength() const { 
		return length_;
	}
	
	double getHeight() const { 
		return height_;
	}
  
private:
  	double length_;
  	double width_;
  	double height_;
};

// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects 
static list<Shape*> CreateShapes(char* file_name) {
	//@@Insert your code here
	list<Shape*> shapes;
	ifstream ifs (file_name, ifstream::in); // input file stream
	string l;
	while (ifs >> l){ // as long as there is a new name to read
		if (l == "Rectangle") { // depending on what name input the correct parameters
            double width, length;
            if (ifs >> width >> length) { // need to verify this for <T> or <double>
                shapes.push_back(new Rectangle<double>(width, length)); // create new instance and add it to the list
            }
        } else if (l == "Circle") {
            double radius;
            if (ifs >> radius) {
                shapes.push_back(new Circle(radius));
            }
        } else if (l == "Sphere") {
            double radius;
            if (ifs >> radius) {
                shapes.push_back(new Sphere(radius));
            }
        } else if (l == "RectPrism") {
            double width, length, height;
            if (ifs >> width >> length >> height) {
                shapes.push_back(new RectPrism(width, length, height));
            }
        }
	}
	ifs.close();

	return shapes;
}

// call getArea() of each object 
// return the max area
static double MaxArea(list<Shape*> shapes){
	double max_area = 0;
	//@@Insert your code here
	for (list<Shape*>::iterator it = shapes.begin(); it != shapes.end(); it++) 
		max_area = max(max_area, (*it)->getArea()); // derefrence iterator and then access area, continually update max_area

	return max_area;
}

// call getVolume() of each object 
// return the max volume
static double MaxVolume(list<Shape*> shapes){
	double max_volume = 0;
	//@@Insert your code here
	for (list<Shape*>::iterator it = shapes.begin(); it != shapes.end(); it++) 
		max_volume = max(max_volume , (*it)->getVolume()); // same for volume
	
	return max_volume;
}
#endif