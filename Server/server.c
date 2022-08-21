#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#include "../Util/util.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "server.h"

char *path_tr = "Transactions DB.txt";
char *path_ac = "Accounts DB.txt";

ST_accountsDB_t accounts[255] = { [0 ... 254] = { .balance = 1000,
	.primaryAccountNumber = "0000000000000505"}};

//ST_transaction_t transactions[255] = { [0 ... 254] = { .transState = 3, .transactionSequenceNumber = 0 }};

bool saveAccount(ST_accountsDB_t *account){
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	ssize_t line_size;
	bool isSaved = false;
	uint8_t seq[MAX_PAN+1] = "";
	int seek = 0;

	FILE *file = fopen(path_ac, "r+");
	if(!file)
		perror(path_ac);

	line_size = getline(&line_buf, &line_buf_size, file);

	while (line_size >= 0 && !isSaved){
		char *step;

		step = strchr(line_buf, ':');
		strncpy(seq, line_buf, strlen(line_buf) - strlen(step));

		if(strcmp(seq, account->primaryAccountNumber) == 0){
			fseek(file,seek,SEEK_SET);
			fprintf(file,"%s:%f",account->primaryAccountNumber,account->balance);
			isSaved = true;
			break;
		}
		seek += strlen(line_buf);
		line_size = getline(&line_buf, &line_buf_size, file);
	}

	free(line_buf);
	line_buf = NULL;

	if(fclose(file))
		perror(path_ac);

	return isSaved;
}
bool getAccount(uint8_t *pan, ST_accountsDB_t *account){
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	ssize_t line_size;
	bool isFound = false;
	uint8_t seq[MAX_PAN+1] = "";

	FILE *file = fopen(path_ac, "r");
	if(!file)
		perror(path_ac);

	line_size = getline(&line_buf, &line_buf_size, file);

	while (line_size >= 0 && !isFound){
		char *step;

		step = strchr(line_buf, ':');
		strncpy(seq, line_buf, strlen(line_buf) - strlen(step));

		if(strcmp(seq, pan) == 0){
			//step0->step
			strcpy(account->primaryAccountNumber, seq);
			
			//step->NULL
			//step = strchr(step0 +1, ':');
			account->balance = strtof(step+1, NULL);
			isFound = true;
			break;
		}
		line_size = getline(&line_buf, &line_buf_size, file);
	}

	free(line_buf);
	line_buf = NULL;

	if(fclose(file))
		perror(path_ac);

	return isFound;
	//for (int i = 0; i < sizeof(accounts)/sizeof(accounts[0]); i++) {
		//if(pan == *accounts[i].primaryAccountNumber){
			//return i;
		//}
	//}
	//return -1;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData){
	ST_accountsDB_t account;
	float balance = -1;
	if(getAccount(&transData->cardHolderData.primaryAccountNumber, &account))
		balance = account.balance;
	time_t t;
	srand((unsigned) time(&t));
	transData->transactionSequenceNumber = rand();
	if(!append(path_tr, "%d:%s:%s:%s:%f:%s:%f:%d:",
			transData->transactionSequenceNumber, //%ld
			transData->cardHolderData.cardHolderName, //%s
			transData->cardHolderData.primaryAccountNumber, //%s
			transData->cardHolderData.cardExpirationDate, //%s
			transData->terminalData.transAmount, //%f
			transData->terminalData.transactionDate, //%s
			balance, //%f
			transData->transState)) //%d
		return SAVING_FAILED;
	return S_OK;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber,
		ST_transaction_t *transData){
			
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	ssize_t line_size;
	bool isFound = false;
	uint32_t seq;

	FILE *file = fopen(path_tr, "r");
	if(!file)
		perror(path_tr);

	line_size = getline(&line_buf, &line_buf_size, file);

	while (line_size >= 0 && !isFound){
		if(sscanf(line_buf, "%d", &seq) == 1 && seq == transactionSequenceNumber){
			char *step0;
			char *step;
			transData->transactionSequenceNumber = strtol(line_buf, &step0, 10);

			//step0->step
			step = strchr(step0 +1, ':');
			strncpy(transData->cardHolderData.cardHolderName,
					step0+1, strlen(step0+1) - strlen(step));

			//step->step0
			step0 = strchr(step +1, ':');
			strncpy(transData->cardHolderData.primaryAccountNumber,
					step+1, strlen(step+1) - strlen(step0));

			//step0->step
			step = strchr(step0 +1, ':');
			strncpy(transData->cardHolderData.cardExpirationDate,
					step0+1, strlen(step0+1) - strlen(step));

			//step->step0
			//step0 = strchr(step +1, ':');
			transData->terminalData.transAmount = strtof(step +1, &step0);

			//step0->step
			step = strchr(step0 +1, ':');
			strncpy(transData->terminalData.transactionDate,
					step0+1, strlen(step0+1) - strlen(step));

			//step->step0
			//step = strchr(step0 +1, ':');
			ST_accountsDB_t account;
			if(getAccount(&transData->cardHolderData.primaryAccountNumber, &account))
					account.balance = strtof(step+1, &step0);

			//step->NULL
			transData->transState = strtol(step0, NULL, 10);
			isFound = true;
			break;
		}
		line_size = getline(&line_buf, &line_buf_size, file);
	}

	free(line_buf);
	line_buf = NULL;

	if(fclose(file))
		perror(path_tr);

	return isFound ? S_OK : TRANSACTION_NOT_FOUND;
}

//EN_serverError_t saveTransaction(ST_transaction_t *transData){
	//int index = 0;
	//int size = sizeof(transactions)/sizeof(transactions[0]);
	//while(index < size && transactions[index].transactionSequenceNumber != 0)
		//index++;
	//if(index >= size)
		//return SAVING_FAILED;
	//srand(index);
	//transData->transactionSequenceNumber = rand();
	//memcpy( &transactions[index], transData, sizeof(struct ST_transaction_t));
	//return S_OK;
//}

//EN_serverError_t getTransaction(uint32_t transactionSequenceNumber,
		//ST_transaction_t *transData){
	//for (int i = 0; i < sizeof(transactions)/sizeof(transactions[0]); i++) {
		//if(transactionSequenceNumber == transactions[i].transactionSequenceNumber){
			//memcpy(transData, &transactions[i], sizeof(struct ST_transaction_t));
			//return S_OK;
		//}
	//}
	//return TRANSACTION_NOT_FOUND;
//}

EN_serverError_t isValidAccount(ST_cardData_t *cardData){
	if(isValidCardPAN(cardData) != 0)
		return DECLINED_STOLEN_CARD;
		
	ST_accountsDB_t account;
	if(!getAccount(&cardData->primaryAccountNumber, &account))
		return ACCOUNT_NOT_FOUND;
	return S_OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData){
	return isBelowMaxAmount(termData) == 0 ?
		 S_OK : LOW_BALANCE;
}

EN_transState_t recieveTransactionData(ST_transaction_t *transData){
	if(isValidAccount(&transData->cardHolderData) != 0)
		return DECLINED_STOLEN_CARD;

	ST_accountsDB_t account;
	getAccount(&transData->cardHolderData.primaryAccountNumber, &account);

	if(isAmountAvailable(&transData->terminalData) != 0 ||
			transData->terminalData.transAmount > account.balance)
		return DECLINED_INSUFFECIENT_FUND;

	transData->transState = APPROVED;
	account.balance -= transData->terminalData.transAmount;

	if(saveAccount(&account) && saveTransaction(transData) !=0){
		transData->transState = INTERNAL_SERVER_ERROR;
		account.balance += transData->terminalData.transAmount;
		return INTERNAL_SERVER_ERROR;
	}

	return transData->transState;
}
