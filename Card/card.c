#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#include "../Util/util.h"
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData){

	uint8_t tmp[MAX_CHAR +1] = "";
	printf("Plese enter Card Holder Name: ");

	if(	!getInput("%[a-zA-Z ]", tmp) ||
		strlen(tmp) < MIN_CHAR ||
		 strlen(tmp) > MAX_CHAR )
		return WRONG_NAME;

	memcpy(cardData->cardHolderName, tmp, sizeof(tmp));
	return C_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData){
	
	uint8_t tmp[EXP_DATE +1] = "";
	printf("Plese enter Expiry Date in format \"MM/YY\": ");

	if( !getInput("%5[0-9/]", tmp) ||
		strcmp(tmp, "") == 0 ||
		 strlen(tmp) != EXP_DATE ||
		 tmp[2] != '/')
		return WRONG_EXP_DATE;

	uint32_t mm = strtol(tmp,NULL,10);
	if( mm > 12)
		return WRONG_EXP_DATE;

	memcpy(cardData->cardExpirationDate, tmp, sizeof(tmp));
	return C_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData){

	uint8_t tmp[MAX_PAN +1];
	printf("Plese enter Primary Account Number: ");

	if( !getInput("%[0-9]", tmp) ||
		strcmp(tmp, "") == 0 ||
		strlen(tmp) < MIN_PAN ||
		 strlen(tmp) > MAX_PAN )
		return WRONG_PAN;

	memcpy(cardData->primaryAccountNumber, tmp, sizeof(tmp));
	return C_OK;
}

