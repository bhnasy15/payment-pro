#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#include "../Util/util.h"
#include "../Card/card.h"
#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData){
	time_t tm;
	time(&tm);
	strftime(termData->transactionDate, sizeof(termData->transactionDate), "%d/%m/%Y", localtime(&tm));
	return T_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData){
	
	char *cd;
	uint32_t c_mm = strtol(cardData.cardExpirationDate, &cd, 10); //month
	uint32_t c_yy = strtol(cd +1,NULL,10); //year
	char *td;
	strtol(termData.transactionDate, &td, 10); //day (unused)
	uint32_t t_mm = strtol(td +1, &td, 10); //month
	uint32_t t_yy = strtol(td +1, NULL, 10); //year
	
	if(t_mm > c_mm)
		if(t_yy > c_yy +2000)
			return EXPIRED_CARD;

	return T_OK;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData){
	int len = strlen(cardData->primaryAccountNumber);
	uint8_t sum = 0;
	for (int i = 1; i <= len ; i++) {
		if(i%2 == 0){
			int next = (cardData->primaryAccountNumber[len-i] - '0')*2;
			sum += next > 9 ? ( next/10 + next%10): next ;
			continue;
		}
		sum += cardData->primaryAccountNumber[len-i] - '0';
	}
	return sum % 10 == 0 ? T_OK : INVALID_CARD;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData){
	printf("plese Enter Transaction Amount: ");
	float tmp;
	if( !getFloat(&tmp) ||
		tmp <= 0)
		return INVALID_AMOUNT;
	termData->transAmount = tmp;
	return T_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float amount){
	if(amount <= 0)
		return INVALID_MAX_AMOUNT;
	termData->maxTransAmount = amount;
	return T_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData){
	if(termData->transAmount > termData->maxTransAmount)
		return EXCEED_MAX_AMOUNT;
	return T_OK;
}

