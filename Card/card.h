#ifndef CARD_H
#define CARD_H
#define MAX_CHAR 24
#define MIN_CHAR 20
#define EXP_DATE 5
#define MAX_PAN 19
#define MIN_PAN 16

typedef struct ST_cardData_t
{
	uint8_t cardHolderName[MAX_CHAR +1];
	uint8_t primaryAccountNumber[MAX_PAN +1];
	uint8_t cardExpirationDate[EXP_DATE +1];
}ST_cardData_t;

typedef enum EN_cardError_t
{
	C_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

EN_cardError_t getCardHolderName(ST_cardData_t *cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);
EN_cardError_t getCardPAN(ST_cardData_t *cardData);


#endif // !CARD_H
