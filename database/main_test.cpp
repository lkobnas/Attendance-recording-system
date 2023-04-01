#include "studentDB.h"

int main(){
    StudentDB db;
    db.initDB();
    db.insertStudent("Test Name","20602322","cardID1234","fpID5678");
    return 0;
    
}
