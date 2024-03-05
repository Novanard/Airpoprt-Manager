#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Airline.h"
#include "Airport.h"
#include "General.h"

void	initAirline(Airline* pComp)
{
	//printf("-----------  Init Airline\n");
	pComp->name = getStrExactName("Enter Airline name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
	pComp->planeArr = NULL;
	pComp->planeCount = 0;
	pComp->sortBy = 3;
	
}

int	addFlight(Airline* pComp,const AirportManager* pManager)
{
	int apCnt = 0;
	NODE* currNode = pManager->airportsList.head.next;
	while (currNode != NULL) {
		apCnt++;
		currNode = currNode->next;
	}
	if (apCnt < 2)
	{
		printf("There are not enough airport to set a flight\n");
		return 0;
	}
	if(pComp->planeCount == 0)
	{
		printf("There is no plane in company\n");
		return 0;
	}

	Flight* pFlight = (Flight*)calloc(1, sizeof(Flight));
	if (!pFlight)
		return 0;
	
	Plane* thePlane = FindAPlane(pComp);
	printAirports(pManager);
	initFlight(pFlight, thePlane,pManager);

	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
	if (!pComp->flightArr)
	{
		free(pFlight);
		return 0;
	}
	pComp->flightArr[pComp->flightCount] = pFlight;
	pComp->flightCount++;
	return 1;
}

int		addPlane(Airline* pComp)
{
	pComp->planeArr = (Plane*)realloc(pComp->planeArr, (pComp->planeCount + 1) * sizeof(Plane));
	if (!pComp->planeArr)
		return 0;
	initPlane(&pComp->planeArr[pComp->planeCount], pComp->planeArr, pComp->planeCount);
	pComp->planeCount++;
	return 1;
}

Plane* FindAPlane(Airline* pComp)
{
	printf("Choose a plane from list, type its serial Number\n");
	printPlanesArr(pComp->planeArr,pComp->planeCount);
	int sn;
	Plane* temp = NULL;
	do {
		scanf("%d", &sn);
		temp = findPlaneBySN(pComp->planeArr,pComp->planeCount, sn);
		if (!temp)
			printf("No plane with that serial number! Try again!\n");
	} while (temp == NULL);
	 
	return temp;
}


void printCompany(const Airline* pComp)
{
	printf("Airline %s\n", pComp->name);
	printf("\n -------- Has %d planes\n", pComp->planeCount);
	printPlanesArr(pComp->planeArr, pComp->planeCount);
	printf("\n\n -------- Has %d flights\n", pComp->flightCount);
	printFlightArr(pComp->flightArr, pComp->flightCount);
}

void	printFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
		printFlight(arr[i]);
}

void	printPlanesArr(Plane* arr, int size)
{
	for (int i = 0; i < size; i++)
		printPlane(&arr[i]);
}

void	doPrintFlightsWithPlaneType(const Airline* pComp)
{
	ePlaneType type = getPlaneType();
	int count = 0;
	printf("Flights with plane type %s:\n", GetPlaneTypeStr(type));
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isPlaneTypeInFlight(pComp->flightArr[i], type))
		{
			printFlight(pComp->flightArr[i]);
			count++;
		}
	}
	if(count == 0)
		printf("Sorry - could not find a flight with plane type %s:\n", GetPlaneTypeStr(type));
	printf("\n");
}

void	freeFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
		free(arr[i]);
}

void	freePlanes(Plane* arr, int size)
{
	for (int i = 0; i < size; i++)
		freePlane(&arr[i]);
}

void	freeCompany(Airline* pComp)
{
	freeFlightArr(pComp->flightArr, pComp->flightCount);
	free(pComp->flightArr);
	free(pComp->planeArr);
	free(pComp->name);
}
int compareFlightBySource(const void* a, const void* b) {
	const Flight* f1 = (const Flight**)a;
	const Flight* f2 = (const Flight**)b;
	return (strcmp(f1->sourceCode, f2->sourceCode));

}
int compareFlightByDest(const void* a, const void* b) {
	const Flight* f1 = (const Flight**)a;
	const Flight* f2 = (const Flight**)b;
	return (strcmp(f1->destCode, f2->destCode));

}
int compareFlightByDate(const void* d1, const void* d2) {
	const Flight* f1 = (const Flight**)d1;
	const Flight* f2 = (const Flight**)d2;
	if (f1->date.year != f2->date.year) {
		return f1->date.year - f2->date.year;
	}
	if (f1->date.month != f2->date.month) {
		return f1->date.month - f2->date.month;
	}
	// If month is also the same, compare day
	return f1->date.day - f2->date.day;
}
int getSortingType(Airline* al) {
	printf("Enter Your Desired Sorting Type:\n0-By Source Code\n1-By Dest Code\n2-By Date");
	int choice;
	do {
		scanf("%d", &choice);
	} while (choice < 0 || choice>2);
	al->sortBy = choice;
	return choice;
}
void sortFlightsByType(Airline* al) {
	int type = al->sortBy;
	switch (type) {
	case 0:
		qsort(al->flightArr, al->flightCount, sizeof(Flight), compareFlightBySource);
		break;
	case 1:
		qsort(al->flightArr, al->flightCount, sizeof(Flight), compareFlightByDest);
		break;
	case 2:
		qsort(al->flightArr, al->flightCount, sizeof(Flight), compareFlightByDate);
		break;
	DEFAULT:
		break;
	}
}
Flight* searchFlightInArray(Airline* al) {
	Flight* f = (Flight*)malloc(sizeof(Flight));
	if (al->sortBy == 3) {
		printf("The array is not sorted, can not binary search");
		return;
	}
	else if (al->sortBy == 2) { // search by Date
		printf("Enter the flight date you wish to search\n");
		Date* d=(Date*)malloc(sizeof(Date));
		getCorrectDate(&d);
		f=bsearch(&d, al->flightArr, al->flightCount, sizeof(Flight), compareFlightByDate);
		if (f != NULL)
			return f;
	}
	else if (al->sortBy == 1) { // Search by Dest Code
		char* dest = getStrExactName("Enter the dest code you want to search\n");
		f=bsearch(dest, al->flightArr, al->flightCount, sizeof(Flight), compareFlightByDest);
		if (f != NULL)
			return f;

	}
	else { // Search by source
		char* source = getStrExactName("Enter the source code you want to search\n");
		f=bsearch(source, al->flightArr, al->flightCount, sizeof(Flight), compareFlightBySource);
		if (f != NULL)
			return f;
	}
}


