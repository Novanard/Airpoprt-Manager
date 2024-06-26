#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "AirportManager.h"
#include "General.h"

int	initManager(AirportManager* pManager)
{
	L_init(pManager);
	pManager->ptr = &pManager->airportsList.head;
	return 1;
}

int	addAirport(AirportManager* pManager)
{
	Airport* pPort = (Airport*)calloc(1, sizeof(Airport));
	if (!pPort)
		return 0;

	if (!initAirport(pPort, pManager))
	{
		freeAirport(pPort);
		free(pPort);
		return 0;
	}
	NODE* newAP = (Airport*)malloc(sizeof(Airport));
	if (pManager->ptr == NULL || strcmp(pPort->code, ((Airport*)pManager->ptr)->code) < 0) {
		//The airport needs to be placed at the beginning
		newAP->key = pPort;
		newAP->next = pManager->ptr;
		pManager->ptr = newAP;
	}
	else { // We need to scan the list to find out where the correct place is
		NODE* tempNode = pManager->ptr;
		while (tempNode->next != NULL && strcmp(((Airport*)tempNode->next)->code, pPort->code) < 0)
			tempNode = tempNode->next;
		if (!L_insert(tempNode, pPort)) {
			freeAirport(pPort);
			free(pPort);
			return 0;
		}

		return 1;

	}
}

int  initAirport(Airport* pPort, AirportManager* pManager)
{
	while (1)
	{
		getAirportCode(pPort->code);
		if (checkUniqeCode(pPort->code, pManager))
			break;

		printf("This code already in use - enter a different code\n");
	}

	return initAirportNoCode(pPort);
}

Airport* findAirportByCode(const AirportManager* pManager, const char* code)
{
	NODE* currNode = pManager->airportsList.head.next;
	while (pManager->airportsList.head.next != NULL) {
		const Airport* testAP = (const Airport*)pManager->airportsList.head.next;
		if (strcmp(code, testAP->code) == 0) // FOUND
			return testAP;
		currNode=currNode->next;
	}
	return NULL;
}

int checkUniqeCode(const char* code,const AirportManager* pManager)
{
	Airport* port = findAirportByCode(pManager, code);

	if (port != NULL)
		return 0;

	return 1;
}


void printAirports(const AirportManager* pManager)
{
	int apCnt = 0;
	NODE* tmpNode = pManager->airportsList.head.next;
	while (tmpNode != NULL) {
		apCnt++;
		tmpNode = tmpNode->next;
	}
	printf("there are %d airports\n", apCnt);
	tmpNode = pManager->airportsList.head.next;
	while(tmpNode!=NULL){
		printAirport((Airport*)tmpNode->key);
		printf("\n");
		tmpNode = tmpNode->next;
	}
}

void	freeManager(AirportManager* pManager)
{
	freeAirportList(pManager);
}


void	freeAirportList(AirportManager* pManager)
{
	NODE* currNode = pManager->airportsList.head.next;
	while(currNode!=NULL){
		freeAirport((Airport*)currNode->key);
		currNode = currNode->next;
	}
	L_free(&pManager->airportsList,free);
}