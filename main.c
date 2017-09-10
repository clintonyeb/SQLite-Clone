#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct InputBuffer_t {
    char* buffer;
    size_t bufferLength;
    ssize_t  inputLength;
};

typedef struct InputBuffer_t InputBuffer;

InputBuffer* newInputBuffer(){
    InputBuffer* data = malloc(sizeof(InputBuffer));
    data -> buffer = NULL;
    data -> bufferLength = 0;
    data -> inputLength = 0;

    return data;
}

void printPrompt(){
    printf("db>> ");
}

void readInput(InputBuffer* InputBuffer) {
    ssize_t bytesRead = getline(&(InputBuffer -> buffer), &(InputBuffer -> bufferLength), stdin);

    if(bytesRead <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    InputBuffer -> inputLength = bytesRead - 1;
    InputBuffer -> buffer[bytesRead - 1] = 0;
}

enum MetaResult_t {
  META_COMMAND_SUCCESS,
  META_COMMAND_FAILED
};

typedef enum MetaResult_t MetaResult;

enum PrepareResult_t {
  PREPARE_SUCCESS,
  PREPARE_FAILED
};

typedef enum PrepareResult_t PrepareResult;

MetaResult doMetaCommand(InputBuffer* buffer){
  if (strcmp(buffer -> buffer, ".exit") == 0){
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_FAILED;
  }
}

enum StatementType_t {
  STATEMENT_INSERT,
  STATEMENT_SELECT
};
typedef enum StatementType_t StatementType;

struct Statement_t {
  StatementType type;
};

typedef struct Statement_t Statement;

PrepareResult prepareStatement(InputBuffer* inputBuffer, Statement* statement){
  if(strncmp(inputBuffer -> buffer, "insert", 6) == 0) {
    statement -> type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if(strcmp(inputBuffer -> buffer, "select") == 0) {
    statement -> type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
   }

  return PREPARE_FAILED;
}

int executeStatement(Statement* statement) {
  switch(statement -> type){
  case STATEMENT_INSERT:
    printf("This is where we would insert data.\n");
    break;
  case STATEMENT_SELECT:
    printf("This is where we would select data\n");
    break;
  }
}

int main(int argc, char* argv[]) {
    InputBuffer* inputBuffer = newInputBuffer();
    while(true) {
        printPrompt();
        readInput(inputBuffer);

	if(inputBuffer -> buffer[0] == '.'){
	  switch(doMetaCommand(inputBuffer)){
	  case META_COMMAND_SUCCESS:
	    continue;
	  case META_COMMAND_FAILED:
	    printf("Unrecognized command '%s'\n", inputBuffer->buffer);
	    continue;
	  }
	}

	Statement statement;

	switch(prepareStatement(inputBuffer, &statement)){
	case PREPARE_SUCCESS:
	  break;
	case PREPARE_FAILED:
	  printf("Unrecognized keyword at start of '%s'\n", inputBuffer -> buffer);
	  continue;
	}

	executeStatement(&statement);
	printf("Statement executed.\n");
    }
}
