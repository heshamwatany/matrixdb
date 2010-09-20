/*
 *  store.h
 *  MatrixDB
 *
 *  Created by PH on 4.6.10.
 *  Copyright 2010 Codesign LLC. All rights reserved.
 *
 */

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <tcutil.h>
#include <tctdb.h>
#import "matrix.h"

namespace matrixdb {

	class Store {
		
	private:
		TCTDB* tdb;
		bool opened;

		string generateUid() {
			std::stringstream ss;
			ss << random() << time(NULL);
			return ss.str();
		}
		
	public:
		Store(string fileName, bool autosync) {
			srandomdev();
			tdb = tctdbnew();
			if(tctdbopen(tdb, fileName.c_str(),
						 autosync ? (TDBOWRITER | TDBOCREAT | TDBOTSYNC) : (TDBOWRITER | TDBOCREAT))) {
				opened = true;
				tctdbsetindex(tdb, "__class__", TDBITLEXICAL | TDBITKEEP);
			} else {
				int ecode = tctdbecode(tdb);
				cout << "KVStore open error: " << tctdberrmsg(ecode) << endl;
			}
		}
		
		void indexField(const char* fieldName) {
			tctdbsetindex(tdb, fieldName, TDBITLEXICAL | TDBITKEEP);
		}
		
		void instantiate(Matrix* matrix) {
			TCMAP* map = tctdbget(tdb, matrix->getUid()->data(), matrix->getUid()->size());
			tcmapiterinit(map);
			const char* name;
			std::map<string, string>* strings = new std::map<string, string>();
			while((name = tcmapiternext2(map)) != NULL) {
				const char* value = tcmapget2(map, name);
				(*strings)[name] = value;
			}
			tcmapdel(map);
			matrix->setStrings(strings);
			matrix->__instantiated = true;
		}
		
		std::vector<Matrix*>* load(std::map<string, string>* filter, bool instantiate) {
			std::vector<Matrix*>* vector = new std::vector<Matrix*>;
			TDBQRY* qry = tctdbqrynew(tdb);
			for(std::map<string, string>::const_iterator it = filter->begin(); it != filter->end(); ++it) {
				string key = it->first;
				string value = it->second;
				tctdbqryaddcond(qry, key.c_str(), TDBQCSTREQ, value.c_str());
			}
			TCLIST* res = tctdbqrysearch(qry);
			for(int i = 0; i < tclistnum(res); i++){
				int rsiz;
				const char* rbuf = (const char*) tclistval(res, i, &rsiz);
				std::map<string, string>* strings = NULL;
				if (instantiate) {
					TCMAP* map = tctdbget(tdb, rbuf, rsiz);
					tcmapiterinit(map);
					const char* name;
					strings = new std::map<string, string>();
					while((name = tcmapiternext2(map)) != NULL) {
						const char* value = tcmapget2(map, name);
						(*strings)[name] = value;
					}
					tcmapdel(map);
				}
				Matrix* matrix = new Matrix(strings, new std::map<string, Matrix*>(), instantiate);
				matrix->setUid(string(rbuf, rsiz));
				vector->push_back(matrix);
			}
			tclistdel(res);
			tctdbqrydel(qry);
			return vector;
		}
		
		bool remove(Matrix* matrix) {
			return tctdbout2(tdb, matrix->getUid()->c_str());
		}
		
		bool save(Matrix* matrix) {
			TCMAP* map = tcmapnew();
			std::map<string, string>* strings = matrix->getStrings();
			for(std::map<string, string>::const_iterator it = strings->begin(); it != strings->end(); ++it) {
				string key = it->first;
				string value = it->second;
				tcmapput2(map, key.c_str(), value.c_str());
			}
			if (matrix->getUid()->size() == 0) {
				matrix->setUid(generateUid());
				//cout << "UID set: '" << matrix->getUid()->c_str() << "'" << endl;
			}
			const char* uid = matrix->getUid()->c_str();
			bool success = tctdbput(tdb, uid, strlen(uid), map);
			if (!success) {
				int ecode = tctdbecode(tdb);
				cout << "KVStore put error: " << tctdberrmsg(ecode) << " (" << ecode << ")" << endl;
			}
			tcmapdel(map);
			return success;
		}
		
		bool beginTransaction() {
			return tctdbtranbegin(tdb);
		}
		
		bool commit() {
			return tctdbtrancommit(tdb);
		}
		
		bool rollback() {
			return tctdbtranabort(tdb);
		}
		
		~Store() {
			if(opened && !tctdbclose(tdb)){
				int ecode = tctdbecode(tdb);
				cout << "KVStore close error: " << tctdberrmsg(ecode) << endl;
			}
			tctdbdel(tdb);
			
		}

	};

}
