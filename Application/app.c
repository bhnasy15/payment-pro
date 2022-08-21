#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#include "../Util/util.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"
#include "app.h"

void getCardData(ST_cardData_t *cd){
	int cname = getCardHolderName(cd);
	while(cname != 0){
		printf("Invalid Name, try again?(y/n):");
		char q;
		if(!getInput("%1c",&q) || q != 'y')
			exit(1);
		cname = getCardHolderName(cd);
	}

	int cexp = getCardExpiryDate(cd);
	while(cexp != 0){
		printf("Invalid date, try again?(y/n):");
		char q;
		if(!getInput("%1c",&q) || q != 'y')
			exit(1);
		cexp = getCardExpiryDate(cd);
	}

	int cpan = getCardPAN(cd);
	while(cpan != 0){
		printf("Invalid card number, try again?(y/n):");
		char q;
		if(!getInput("%1c",&q) || q != 'y')
			exit(1);
		cpan = getCardPAN(cd);
	}
}

void appStart(void){
	
	ST_cardData_t cd;
	getCardData(&cd);
	
	ST_terminalData_t td;
	//get card data 
	int tdate= getTransactionDate(&td);
	int texp= isCardExpired(cd, td);
	if (texp !=0 || tdate != 0) {
		printf("declined expired card!\n");
		exit(1);
	}
	int tmaxamount = setMaxAmount(&td, AMOUNT_LIMIT);
	int tamount = getTransactionAmount(&td);
	int tbelow = isBelowMaxAmount(&td);
	if (tbelow != 0) {
		printf("declined amount exceeding limit of %d!\n", AMOUNT_LIMIT);
		exit( 1);
	}
	
	ST_transaction_t tr = { .cardHolderData = cd, .terminalData = td};
	int trec = recieveTransactionData(&tr);
	switch (trec) {
		case 0:
			ST_accountsDB_t account;
			float balance = -1;
			if(getAccount(tr.cardHolderData.primaryAccountNumber, &account))
				balance = account.balance;
			printf("successful transaction - balance = %f\n", balance);
			break;
		case 1:
			printf("decliend insuffecient funds!\n");
			break;
		case 2:
			printf("decliend invalid account!\n");
			break;
		default:
			printf("unknown error\n");
			break;
	}

}

int main(){

	while(1){
		appStart();
		printf("\nwould you like to make another transaction (y/n)? ");
		char cr;
		if(!getInput( "%1c", &cr) || cr != 'y')
			break;
	}
	return 0;
}

