#ifndef CLEANERWINDOW_HH
#define CLEANERWINDOW_HH

#include <QtGui>
#include <QObject>
#include <QThread> 
#include "Object.hh"
#include <map>
#include <fstream>

using namespace std;

class WorkerThread; 
class PopMerger; 

enum TaskType {LoadFile = 0,
	       Statistics,
	       AutoMap, 
	       Convert,
	       NumTasks}; 

class CleanerWindow : public QMainWindow {
  Q_OBJECT
  
public:
  CleanerWindow (QWidget* parent = 0); 
  ~CleanerWindow ();
  
  QPlainTextEdit* textWindow; 
  WorkerThread* worker;
  void openDebugLog (string fname);
  void closeDebugLog (); 
  void loadFile (string fname, TaskType autoTask = NumTasks);
						 
						 
						 
public slots:

  void loadFile (); 
  void getStats ();
  void autoMap (); 
  void convert ();
  void message (QString m); 
  
private:

};

struct ObjectSorter {
  ObjectSorter (string k) {keyword = k;} 
  string keyword;
};
struct ObjectAscendingSorter : public ObjectSorter {
public:
  ObjectAscendingSorter (string k) : ObjectSorter(k) {}
  bool operator() (Object* one, Object* two) {return (one->safeGetFloat(keyword) < two->safeGetFloat(keyword));} 
private:
};
struct ObjectDescendingSorter : public ObjectSorter {
public:
  ObjectDescendingSorter (string k) : ObjectSorter(k) {}
  bool operator() (Object* one, Object* two) {return (one->safeGetFloat(keyword) > two->safeGetFloat(keyword));} 
private:
};

double calcAvg (Object* ofthis); 

class WorkerThread : public QThread {
  Q_OBJECT
public:
  WorkerThread (string fname, TaskType aTask = NumTasks); 
  ~WorkerThread ();
  void setTask(TaskType t) {task = t;} 

protected:
  void run (); 
  
private:
  // Misc globals 
  string targetVersion;
  string sourceVersion; 
  string fname; 
  Object* vicGame;
  Object* hoiGame;
  TaskType task; 
  Object* configObject;
  TaskType autoTask;

  // Conversion processes
  bool convertProvinceOwners (); 
  bool moveCapitals ();
  bool moveIndustry ();
  bool moveResources ();   
  
  // Infrastructure 
  void loadFile (string fname); 
  void getStatistics ();
  void autoMap (); 
  void convert ();   
  void configure ();

  // Initialisers
  bool createCountryMap ();
  bool createOutputDir ();
  bool createProvinceMap ();
  void initialiseVicSummaries (); 
  void loadFiles (); 
  void setupDebugLog ();
  
  // Helpers:
  void assignCountries (Object* vicCountry, Object* hoiCountry);
  double calculateVicProduction (Object* vicProvince, string resource); 
  void cleanUp (); 
  Object* loadTextFile (string fname);
  Object* selectHoiProvince (Object* vicProv);
  void setPointersFromHoiCountry (Object* hc);
  void setPointersFromVicCountry (Object* vc);
  void setPointersFromVicProvince (Object* vp);
  
  // Maps
  map<Object*, Object*> vicCountryToHoiCountryMap;
  map<Object*, Object*> hoiCountryToVicCountryMap;
  map<string, string> vicTagToHoiTagMap;
  map<string, string> hoiTagToVicTagMap;
  map<string, Object*> hoiTagToHoiCountryMap;
  map<string, Object*> vicProvIdToVicProvMap; 
  map<Object*, objvec> vicCountryToHoiProvsMap;
  map<Object*, objvec> hoiCountryToHoiProvsMap;
  map<Object*, objvec> vicProvToHoiProvsMap;
  map<Object*, objvec> hoiProvToVicProvsMap; 

  // Lists
  objvec vicProvinces;
  objvec vicCountries;  
  objvec hoiProvinces;
  objvec hoiCountries;
  
  // Input info
  Object* provinceMapObject;
  Object* countryMapObject;
  Object* provinceNamesObject; 
  Object* customObject; 
}; 

#endif
