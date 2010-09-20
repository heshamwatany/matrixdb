/*
 *  matrix.h
 *  MatrixDB
 *
 *  Created by PH on 4.6.10.
 *  Copyright 2010 Codesign LLC. All rights reserved.
 *
 */

#include <iostream>
#include <string>
#include <map>

using namespace std;

namespace matrixdb {
	
	class Matrix {
		
	private:
		string uid;
		map<string, string>* strings;
		map<string, Matrix*>* matrices;
		
	public:
		bool __instantiated;
		
		Matrix() {
			strings = new map<string, string>;
			matrices = new map<string, Matrix*>;
		}
		
		Matrix(map<string, string>* _strings, map<string, Matrix*>* _matrices, bool inst) {
			strings = _strings;
			matrices = _matrices;
			__instantiated = inst;
		}
		
		void set(string key, string value) {
			(*strings)[key] = value;
		}
		
		void set(string key, Matrix* matrix) {
			(*matrices)[key] = matrix;
		}
		
		string* getString(string key) {
			return strings->count(key) == 1 ? &(*strings)[key] : NULL;
		}
		
		Matrix* getMatrix(string key) {
			return (*matrices)[key];
		}
		
		string* getUid() {
			return &uid;
		}
		
		void setUid(string _uid) {
			uid = _uid;
		}
		
		map<string, string>* getStrings() {
			return strings;
		}
		
		void setStrings(map<string, string>* _strings) {
			strings = _strings;
		}
		
		void test() {
			cout << uid << endl;
			for(map<string, string>::const_iterator it = strings->begin(); it != strings->end(); ++it) {
				cout << it->first << ": '" << it->second << "'" << endl;
			}	
		}
		
		~Matrix() {
			delete strings;
			delete matrices;
		}
		
	};
	
}
