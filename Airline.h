#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"
typedef enum {
	eSource, eDest, eDate, eNotSorted
} eSortBy;
typedef struct
{
	char*		name;
	int			flightCount;
	Flight**	flightArr;
	int			planeCount;
	Plane*		planeArr;
	eSortBy sortBy;
}Airline;

void	initAirline(Airline* pComp);
int		addFlight(Airline* pComp,const AirportManager* pManager);
int		addPlane(Airline* pComp);
Plane*	FindAPlane(Airline* pComp);
void	printCompany(const Airline* pComp);
void	printFlightArr(Flight** arr, int size);
void	printPlanesArr(Plane* arr,int size);
void	doPrintFlightsWithPlaneType(const Airline* pComp);
void	freeFlightArr(Flight** arr, int size);
void	freePlanes(Plane* arr, int size);
void	freeCompany(Airline* pComp);
int compareFlightBySource(const void* a, const void* b);
int compareFlightByDest(const void* a, const void* b);
int compareFlightByDate(const void* d1, const void* d2);
int getSortingType();
void sortFlightsByType(Airline* al);
Flight* searchFlightInArray(Airline* al);
int	writeToBfile(Airline* pComp);

#endif

