#ifndef SAVED_H__
#define SAVED_H__

#define SIZE 5

class SavedData{
//Initialize private data
private:
	bool save;
	clock_t start;
	float *info;
//Create public functions
public:
	SavedData();
	SavedData (const SavedData &obj);
	~SavedData();
	void saveGame(int, int, int, int, int*, int*, float);
	clock_t getStart();
	float* openSv();
	float* getSave();
	void setHiSc(float);
	void getHiSc();
	void begClck();
	float endClck();
	float getTime();
};
#endif