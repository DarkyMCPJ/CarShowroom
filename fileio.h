#ifndef FILEIO_H
#define FILEIO_H

#include "car.h"

void initFileIfMissing(const char *filename);
int loadAllCars(const char *filename, Car **cars);
void saveAllCars(const char *filename, Car *cars, int count);
void copyFile(const char *src, const char *dest);

#endif // FILEIO_H
