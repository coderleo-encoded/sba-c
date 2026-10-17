/*
+---------------+---------------+---------------+---------------+
| Student Name: Leonardo Michael Harrison                       |                                                                                             
+---------------+---------------+---------------+---------------+
| Name of Teacher: Mr. F. Powell                                |
| Centre: Kingston College                                      |
| Centre #: 100057                                              |
| Candidate #: 1000570930                                       |
| Date of Completion: 4/17/25                                   |
+---------------+---------------+---------------+---------------+

+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| Project Title:                                                    "DIGITALIZED INVENTORY MANAGEMENT & LOGISTICS OF HEMSS"                                     |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| Synopsis of Project:                  Harrison Electrical and Mechanical Sales and Services (HEMSS) plans to digitize its operations with a secure,           |
|                                   computerized system to streamline inventory, invoicing, and supplier management while addressing inefficiencies             |
|                                   caused by its current manual processes.                                                                                     |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
*/

/* 
+---------------+---------------+---------------+
|               EXIT CODE GLOSSARY              |
+---------------+---------------+---------------+
|   [0] ---> no errors occured                  |
|   [-1] ---> file error occured                |
|   [-2] ---> record limit exceeded             |
|   [-3] ---> duplicate record found            |
|   [-4] ---> file modification failed          |
+---------------+---------------+---------------+
*/

/* PRE-PROCESSOR DIRECTIVES */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

// Based on operating system
#ifdef _WIN32   
    #include <windows.h>    
#else
    #include <unistd.h>
#endif

/* SYSTEM LENGTHS */
#define MAX_PASSWORD_LENGTH 100                 

// Invoice & Supplier Struct
#define MAX_RECORD_SIZE  250                    // enough to hold any record
#define ID_LENGTH 9                                 
#define MAX_NAME_LENGTH 21                      
#define MAX_PRODUCT_LENGTH 26                 

// Only For Invoice Struct
#define MAX_DATE_LENGTH 9                       // [dd/mm/yy] + '\0'

// Only For Supplier Struct
#define MAX_PHONE_NUMBER_LENGTH 13              
#define MAX_EMAIL_LENGTH 41                     

/* INVENTORY STRUCTS */
typedef struct                                
{
    char invoiceID[ID_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char productPurchased[MAX_PRODUCT_LENGTH];
    char datePurchased[MAX_DATE_LENGTH];
    int qty;
    float priceJMD;
    
} Invoice;

typedef struct 
{
    char supplierID[ID_LENGTH];
    char surname[MAX_NAME_LENGTH];
    char contact[MAX_PHONE_NUMBER_LENGTH];
    char supplierEmail[MAX_EMAIL_LENGTH];
    char productSupplied[MAX_PRODUCT_LENGTH];
    int qty;
    float priceJMD;

} Supplier;

/* GLOBAL VARIABLES */                        
int bootCount;                                 
char password[MAX_PASSWORD_LENGTH];         

/* FUNCTION PROTOYPES */
// For all
void pauseForSeconds(int seconds);
void clearScreen (void);
void clearInputBuffer(void);
void printEntryError(void);
int fileError(char fileName[], FILE *file);
void handleInventoryErrors(int inventoryOption, int exitCode, char inventoryFileName[]);
int getValidQty(const char prompt[], int minQty);
int getValidPrice(const char prompt[], float minPrice);
void getBackupCode(void);
void enterInventory(const char prompt[], int count, char *field, int invoiceID, int supplierID, int space, int noSpace, int textOnly, const int maxLength);
int writeInventoryRecords(int inventoryOption, Invoice *customerInfo, Supplier *supplierInfo, FILE *inventoryFile);
int duplicateInventory(char primaryKey[], FILE *inventoryFile);
int validText(char stringHolder[], int noSpace, int textOnly);
int validID(char *field, int invoiceID, int supplierID, int stringLength);

// In main()
void initializeStructs(Invoice *customerInfo, Supplier *supplierInfo);
int updateBootCount(char bootFileName[], FILE *bootFile);
void setupPassword(char passwordFileName[], FILE *passwordFile);
int updatePassword(char passwordFileName[], FILE *passwordFile);
int mainMenu(char passwordFileName[], Invoice *customerInfo, Supplier *supplierInfo, FILE *passwordFile);
int validCurrentPassword(char passwordFileName[], FILE *passwordFile);

// In mainMenu()
int inventoryMenu(char passwordFileName[], Invoice *customerInfo, Supplier *supplierInfo, FILE *passwordFile);
int resetPassword(char passwordFileName[], FILE *passwordFile);

// In inventoryMenu()
void enterPassword(char passwordFileName[], FILE *passwordFile);
int createInventory(int inventoryOption, char inventoryFileName[], Invoice *customerInfo, Supplier *supplierInfo, FILE *inventoryFile);
int modifyInventory(int inventoryOption, char fileName[], Invoice *customerInfo, Supplier *supplierInfo, FILE *inventoryFile);
int searchInventory(char inventoryFileName[], int inventoryOption, Invoice *invoiceInfo, Supplier *supplierInfo, FILE *inventoryFile);
int viewInventory(char inventoryFileName[], FILE *inventoryFile);
int makeInventoryHeader(char inventoryFileName[], int inventoryOption, FILE *inventoryFile);
void handleBulkEntry(int inventoryOption, int bulkRecords, Invoice *customerInfo, Supplier *supplierInfo, FILE *inventoryFile);
char* saveModifyRecord(int inventoryOption, char record[], Invoice *customerInfo, Supplier *supplierInfo);
void trimSpaces(char stringHolder[]);
void showModifyOptions(int inventoryOption);
void handleModifyEntry(int inventoryOption, Invoice *customerInfo, Supplier *supplierInfo, FILE *inventoryFile);

int main(void)
{
    Invoice customerInfo;
    Supplier supplierInfo;

    /* DECLARATIONS FOR SYSTEM MANAGEMENT */                                                      
    FILE *bootFile = NULL, *passwordFile = NULL;
    char passwordFileName[] = "password.txt";
    char bootFileName[] = "boot_count.txt";
    char exitOption;
    int exitCode = 0;

    /* PREPARE THE SYSTEM FOR INVENTORY MANAGEMENT */
    initializeStructs(&customerInfo, &supplierInfo);
    exitCode = updateBootCount(bootFileName, bootFile);
    setupPassword(passwordFileName, passwordFile);  
    exitCode = updatePassword(passwordFileName, passwordFile);

    /* HANDLE SYSTEM BOOT ERRORS */
    if (exitCode)                                                                           
    {
        return exitCode;
    }
    else if (!exitCode)                                                                         
    {
        printf("\n+----------------------------------------------------------------------------------+\n");
        printf("|                                    SYSTEM MESSAGE                                |\n");
        printf("+----------------------------------------------------------------------------------+\n");
        printf("|                                                                                  |\n");
        printf("|                          System has successfully booted :)                       |\n");
        printf("|                                                                                  |\n");
        printf("+----------------------------------------------------------------------------------+\n");
        pauseForSeconds(1.5);
    }

    /* LOAD THE MAIN MENU */
    exitCode = mainMenu(passwordFileName, &customerInfo, &supplierInfo, passwordFile);   

    /* HANDLE ERRORS AFTER EXITING MAIN MENU */
    if (exitCode)
    {
        pauseForSeconds(10);
        return exitCode;
    }
    else
    {
        do 
        {   
            clearScreen();      
            printf("\n+-------------------------------------------------------------------------+\n");
            printf("|                             SYSTEM MESSAGE                              |\n");
            printf("+-------------------------------------------------------------------------+\n");
            printf("| It seems you have recently finished your intended process.              |\n");
            printf("| Kindly exit the system (Y) or return to main menu (N).                  |\n");
            printf("+-------------------------------------------------------------------------+\n");      
            printf("\nEXIT OPTION (Y/N): ");

            if (scanf(" %c", &exitOption) != 1)                                                     // no need to call clearInputBuffer(), [" %c"] clears buffer
            {
                printEntryError();
                pauseForSeconds(1);
                continue;                                                                           
            }

            // Handle user exit option
            exitOption = toupper(exitOption);                                                       // toupper() converts to uppercase & returns it [<ctype.h>]
            switch (exitOption)
            {
                case 'Y': 
                    return exitCode;
                    break;
                case 'N': 
                    exitCode = mainMenu(passwordFileName, &customerInfo, &supplierInfo, passwordFile);   
                    break;
                default:
                    break;
            }

        } while (exitOption != 'Y' && exitOption != 'N');
    }
}

void pauseForSeconds(int seconds)
{
    #ifdef _WIN32
        seconds *= 1000;    // converted to milliseconds
        Sleep(seconds);   
    #else   
        sleep(seconds);     // same function ---> different syntax for Linux || MacOS.
    #endif
}

void clearScreen(void)
{
    #ifdef _WIN32
        system("cls");     
    #else
        system("clear");     // same function ---> different syntax Linux or MacOs.
    #endif
}

void clearInputBuffer(void)
{
    int c;

    /* CLEAR STANDARD INPUT BUFFER */               // no input left to read ---> getchar() returns EOF (-1)
                                                    // getchar() returns ASCII value of character read
    while ((c = getchar()) != '\n' && c != EOF);    // removes '\n' left behind by scanf() || fgets() truncation
}

void printEntryError(void)
{
    fprintf(stderr, "\n+----------------------------------------------------------------+\n");
    fprintf(stderr, "|                        ERROR MESSAGE                           |\n");
    fprintf(stderr, "+----------------------------------------------------------------+\n");
    fprintf(stderr, "| Invalid entry. Kindly adhere to the prompt and try again.      |\n");
    fprintf(stderr, "+----------------------------------------------------------------+\n");
}

int fileError(char fileName[], FILE *file)
{
    int exitCode = 0;
    
    if (!file)
    {
        exitCode = -1;
        fprintf(stderr, "\n+---------------------------------------------------------------------------------+\n");
        fprintf(stderr, "|                                 ERROR MESSAGE                                   |\n");
        fprintf(stderr, "+---------------------------------------------------------------------------------+\n");
        fprintf(stderr, "| Unexpectedly, the system couldn't open the file for the intended process.       |\n");
        fprintf(stderr, "| Likely, the file doesn't exist or is already in use.                            |\n");
        fprintf(stderr, "+---------------------------------------------------------------------------------+\n");       
        fprintf(stderr, "+---------------------------------------------------------------------------------+\n");       
        fprintf(stderr, "| See details below.                                                              |\n");
        fprintf(stderr, "+---------------------------------------------------------------------------------+\n");       
        fprintf(stderr, "\nFILENAME: \"%s\"", fileName);    
        perror("\nERROR DETAILS");                                          // will append the reason for FILE failure.
    }

    return exitCode;
}

void handleInventoryErrors(int inventoryOption, int exitCode, char inventoryFileName[])
{
    const char *errorHeader = "\n+---------------------------------------------------------------------+\n";
    const char *errorTitle =  "|                             ERROR MESSAGE                           |\n";
    const char *errorFooter = "+---------------------------------------------------------------------+\n";

    fprintf(stderr, "%s", errorHeader);
    fprintf(stderr, "%s", errorTitle);
    fprintf(stderr, "%s", errorFooter);

    /* HANDLE INVENTORY MENU OPTIONS (1-4) */ 
    switch (inventoryOption)
    {
        // Cases 1 - 4 --> Handles Invoice Errors
        case 1:
        case 2:
        case 3:
        case 4:
            if (exitCode == -2)
            {
                fprintf(stderr, "| The invoice record entered exceeds the character limit.             |\n");
                fprintf(stderr, "| Please ensure your input is within the allowed size.                |\n");                
            }
            else if (exitCode == -3)
            {
                fprintf(stderr, "| The invoice file appears to be empty!                               |\n");
                fprintf(stderr, "| Let's add some data first, then you can proceed with changes.       |\n");
            }
            break;  

        // Cases 5 - 8 --> Handles Supplier Errors
        case 5: 
        case 6:
        case 7:
        case 8:
            if (exitCode == -2)
            {   
                fprintf(stderr, "| The supplier record entered exceeds the character limit.            |\n");
                fprintf(stderr, "| Please ensure your input is within the allowed size.                |\n");  
            }
            else if (exitCode == -3)
            {
                fprintf(stderr, "| The supplier file appears to be empty!                              |\n");
                fprintf(stderr, "| Let's add some data first, then you can proceed with changes.       |\n");
            }
            break;
    }

    // Same message is printed, regardless of case
    if (exitCode == -4)
    {
        fprintf(stderr, "| Weirdly, the inventory file was prevented from modification.        |\n");
        fprintf(stderr, "| Likely, the file doesn't exist or is in use.                        |\n");  
        fprintf(stderr, "+---------------------------------------------------------------------+\n");     
        fprintf(stderr, "+---------------------------------------------------------------------+\n");       
        fprintf(stderr, "| See details below.                                                  |\n");  
        fprintf(stderr, "+---------------------------------------------------------------------+\n");       
        fprintf(stderr, "\nFILENAME: \"%s\"", inventoryFileName);
        perror("\nERROR DETAILS");                                          // appends reason for FILE error
    }
    else                                                                    // closes the error message box with its footer
    {         
        fprintf(stderr, "%s", errorFooter);                    
    }              
}

int getValidQty(const char prompt[], int minQty) 
{
    int qty;

    do
    {
        printf("%s", prompt);

        if (scanf("%d", &qty) != 1 || qty < minQty || qty > INT_MAX)        // INT_MAX ---> architecture dependent <limits.h>
        {
            printEntryError();
            clearInputBuffer();
            continue;
        }
        else
        {
            clearInputBuffer();
        }

    } while (qty < minQty);

    return qty; 
}
int getValidPrice(const char prompt[], float minPrice) 
{
    float price;

    do
    {
        printf("%s", prompt);
        if (scanf("%f", &price) != 1 || price < minPrice || isinf(price))   // isinf() --> floating point number is too large <math.h>
        {
            printEntryError();
            clearInputBuffer();
            continue;
        }
        else
        {
            clearInputBuffer();                                                                                      
        }

    } while (price < minPrice);

    return price;
}

void getBackupCode(void) 
{
    int backupCode, validBackupCode = 0, i;
    const int backupCodes[5] = {982145673, 374829156, 561437289, 837164520, 192847365};     // all 9 digit backup codes
    
    clearScreen();

    /* GET VALID BACKUP CODE */
    do 
    {
        printf("\n+----------------------------------------------------------------+\n");
        printf("|                         BACKUP CODE ENTRY                      |\n");
        printf("+----------------------------------------------------------------+\n");
        printf("| To continue, please enter one of the five (5) backup codes     |\n");
        printf("| which were provided to you (the manager) beforehand.           |\n");
        printf("+----------------------------------------------------------------+\n");  
        backupCode = getValidQty("\nBACKUP CODE: ", 192847365);                             

        // Check whether the backup code entered is valid
        for (i = 0; i < 5; i++)
        {
            // Backup code entered matches 
            if (backupCodes[i] == backupCode)
            {
                validBackupCode = 1;
                break;
            }
        }

        if (validBackupCode)
        {
            printf("\n+----------------------------------------------------------------+\n");
            printf("|                         SYSTEM MESSAGE                         |\n");
            printf("+----------------------------------------------------------------+\n");
            printf("| Successfully entered the correct backup code!                  |\n");
            printf("+----------------------------------------------------------------+\n");
        }
        else
        {
            printEntryError();
        }

    } while (!validBackupCode);
}

void enterInventory(const char prompt[], int count, char *field, int invoiceID, int supplierID, int space, int noSpace, int textOnly, const int maxLength) 
{
    char dynamicPrompt[100];       
    int validInput, stringLength;

    /* DYNAMICALLY HANDLE PROMPT */
    snprintf(dynamicPrompt, sizeof(dynamicPrompt), prompt, count);                  // format prompt with count

    /* HANDLE INVENTORY INPUT */
    do 
    {   
        validInput = 1;                                                             // assume that every new input is valid

        // Request field input from user
        printf("%s", dynamicPrompt);  
        fgets(field, maxLength, stdin);                                             // exceeds limit --> truncates remaining characters                                           

        // Manage input buffer
        if (strchr(field, '\n') == NULL)                                            // fgets() buffer limit exceeded ---> '\n' is not appended
        {   
            clearInputBuffer();                                                     // '\n' is cleared from input buffer for future inputs
        }
        field[strcspn(field, "\n")] = 0;                                            // buffer limit was not exceeded ---> '\n' was appended
        stringLength = strlen(field);                                               // strlen() after removing '\n'

        /* VALIDATE BASED ON INPUT TYPE */
        // For further details about input type ---> handleBulkEntry() || handleFieldEntry()
        if (noSpace)
        {
            if (textOnly && !validText(field, noSpace, textOnly))                   // validate as no space, text-only
                validInput = 0;
            else if (!validText(field, noSpace, 0))                                 // validate as no space
                validInput = 0;
        }
        else if (space)
        {
            if (!validText(field, 0, textOnly))                                     // !noSpace [0] is passed ---> validate as space, text-only
                validInput = 0;
        }
        else
        {
            if (invoiceID && !validID(field, invoiceID, 0, stringLength))           // validate as invoice ID
                validInput = 0;
            else if (supplierID && !validID(field, 0, supplierID, stringLength))    // validate as supplier ID
                validInput = 0;
        }

    } while (!validInput); 
}

int writeInventoryRecords(int inventoryOption, Invoice *customerInfo, Supplier *supplierInfo, FILE *inventoryFile) 
{
    double totalPrice = 0;                                                          // double has more digits than float ---> used for maximum precision  
    int recordSize = 0, exitCode = 0;

    switch (inventoryOption)
    {
        /* SIZE OF INVOICE RECORD */
        // Case 1 || 2 ---> Entered or Modified Record
        case 1:
        case 2:
            totalPrice = customerInfo->priceJMD * customerInfo->qty;
            recordSize = strlen(customerInfo->invoiceID) 
            + strlen(customerInfo->firstName) 
            + strlen(customerInfo->lastName) 
            + strlen(customerInfo->productPurchased)
            + strlen(customerInfo->datePurchased)
            + snprintf(NULL, 0, "%d", customerInfo->qty)   
            + snprintf(NULL, 0, "%.2f", customerInfo->priceJMD)
            + snprintf(NULL, 0, "%.2f", totalPrice)                                 // snprintf() counts each character 
            + 9;                                                                    // Added for the '|' seperators

            /* PRINT THE CORRECTLY SIZED RECORD */
            // Exit once invalid Invoice Record 
            if (recordSize > MAX_RECORD_SIZE)
            {
                exitCode = -2;
                handleInventoryErrors(inventoryOption, exitCode, NULL);             // NULL: fileName ---> not needed for this error
            }
            else
            {
                fprintf(inventoryFile, "| %-13s | %-25s | %-25s | %-30s | %-17s | %-15d | $%-13.2f| $%-21.2f|\n", 
                        customerInfo->invoiceID, customerInfo->firstName, customerInfo->lastName,customerInfo->productPurchased, 
                        customerInfo->datePurchased, customerInfo->qty, customerInfo->priceJMD, totalPrice);
            }
            break;
        
        /* SIZE OF SUPPLIER RECORD */
        // Case 5 || 6 ---> Entered or Modified Record
        case 5:
        case 6:
            recordSize = strlen(supplierInfo->supplierID) 
            + strlen(supplierInfo->surname) 
            + strlen(supplierInfo->contact) 
            + strlen(supplierInfo->supplierEmail) 
            + strlen(supplierInfo->productSupplied)
            + snprintf(NULL, 0, "%d", supplierInfo->qty)   
            + snprintf(NULL, 0, "%.2f", supplierInfo->priceJMD)                        
            + 9;

            // Exit once invalid Supplier Record 
            if (recordSize > MAX_RECORD_SIZE)
            {
                exitCode = -2;
                handleInventoryErrors(inventoryOption, exitCode, NULL);        
            }
            else 
            {                   
                fprintf(inventoryFile, "| %-13s | %-25s | %-25s | %-40s | %-30s | %-15d | $%-14.2f|\n",
                        supplierInfo->supplierID, supplierInfo->surname, supplierInfo->contact, 
                        supplierInfo->supplierEmail, supplierInfo->productSupplied, supplierInfo->qty, 
                        supplierInfo->priceJMD);
            }
            break;
    }

    return exitCode;
}

int duplicateInventory(char primaryKey[], FILE *inventoryFile)
{
    char buffer[MAX_RECORD_SIZE];
    int dupe = 0;

    /* SEARCH FILE FOR SAME PRIMARY KEY [ID]*/ 
    rewind(inventoryFile);
    while (fgets(buffer, MAX_RECORD_SIZE, inventoryFile))
    {   
        if (strstr(buffer, primaryKey))                     // else ---> returns NULL
        {
            dupe = 1;
        }
    }

    if (dupe)
    {
        printf("\n+-------------------------------------------------------------+\n");
        printf("|                  DUPLICATE RECORD DETECTED                  |\n");
        printf("+-------------------------------------------------------------+\n");
        printf("| The primary key [ID] you entered already exists.            |\n");
        printf("| Please enter a unique ID.                                   |\n");
        printf("+-------------------------------------------------------------+\n");
        pauseForSeconds(3);
        clearScreen();
    }

    return dupe;
}

int validText(char stringHolder[], int noSpace, int textOnly)  
{
    int validText = 1, stringLength = strlen(stringHolder), i;
    char c;           

    /* TRAVERSE THE INPUT STRING */ 
    for (i = 0; i < stringLength; i++)      
    {
        // Loop through each array character
        c = stringHolder[i];

        /* VALIDATE ERRONEOUS INPUT */ 
        // The character is non-readable (beyond ASCII range)
        if ((unsigned char)c < 32 || (unsigned char)c > 126)
        {
            validText = 0;       
            break;
        }

        /* VALIDATE PLAUSIBLE INPUT */ 
        // The string is not allowed empty space
        if (noSpace)
        {
            if (textOnly)
            {
                if (c == ' ' || !isalpha(c))                // space or non-alphabetic character is invalid [first/last names]
                {
                    validText = 0;
                    break;
                }
            }
            else if (c == ' ')                              // only space is invalid [IDs, emails, etc.]                
            {
                validText = 0;
                break;
            }
        }
        else if (textOnly)                                                          
        {
            if (!isalpha(c) && !isspace(c))                 // only letters and spaces are allowed [product names] 
            {
                validText = 0;
                break;
            }
        }
    }

    if (!validText)
    {
        printEntryError();
    } 

    return validText;
}

int validID(char *field, int invoiceID, int supplierID, int stringLength) 
{
    int noSpace = 1, validID = 1, i;                        // ---> Assume valid ID 

    // ["XXXX"]: check for digits in ID
    for (i = 4; i < 8; i++)                                                                 
    {
        // If any character is not a digit
        if (!isdigit(field[i]))                                                   
        {
            validID = 0;                                                      
            break;                                                               
        }
    }

    /* VALIDATE ID */
    // Check whether the ID is correctly formatted
    if (invoiceID && (strncmp(field, "INV-", 4) != 0 || stringLength != ID_LENGTH - 1 || !validText(field, noSpace, 0))) 
    {
        validID = 0;                                        // ---> Invalid ID format
    } 
    else if (supplierID && (strncmp(field, "SUP-", 4) != 0 || stringLength != ID_LENGTH - 1 || !validText(field, noSpace, 0))) 
    {
        validID = 0;                                                         
    }

    if (!validID)                                                             
    {
        printEntryError();                                                     
    }

    return validID;                                                           
}


void initializeStructs(Invoice *customerInfo, Supplier *supplierInfo) 
{
    /* INVOICE STRUCT */
    // Ensure no garbage value is stored in each struct member
    memset(customerInfo->invoiceID, 0, ID_LENGTH);                                                  // [1] memset() ---> requires char[] (char *)
    memset(customerInfo->firstName, 0, MAX_NAME_LENGTH);                                            // [2] memset() assigns null terminator [0 ≡ '\0'] to each element
    memset(customerInfo->lastName, 0, MAX_NAME_LENGTH);                                             // [3] memset() ---> requires a buffer limit 
    memset(customerInfo->productPurchased, 0, MAX_PRODUCT_LENGTH);
    memset(customerInfo->datePurchased, 0, MAX_DATE_LENGTH);
    customerInfo->qty = 0;
    customerInfo->priceJMD = 0.0;

    /* SUPPLIER STRUCT */
    memset(supplierInfo->supplierID, 0, ID_LENGTH);
    memset(supplierInfo->surname, 0, MAX_NAME_LENGTH);
    memset(supplierInfo->contact, 0, MAX_PHONE_NUMBER_LENGTH);
    memset(supplierInfo->supplierEmail, 0, MAX_EMAIL_LENGTH);
    memset(supplierInfo->productSupplied, 0, MAX_PRODUCT_LENGTH);
    supplierInfo->qty = 0;
    supplierInfo->priceJMD = 0;
}

int updateBootCount(char bootFileName[], FILE *bootFile) 
{
    int exitCode = 0;

    /* HANDLE FILE FOR TRACKING SYSTEM BOOT */
    // Attempt to open the file                                                                     // ["r+"] mode: read/write existing file only

    bootFile = fopen(bootFileName, "r+");                                                
    if (!bootFile)                                                                        
    {
        // Attempt to create the file                                                               // ["w"] mode: create new file or truncate existing
        bootFile = fopen(bootFileName, "w");                                              
        if ((exitCode = fileError(bootFileName, bootFile)) == -1)
        {
            return exitCode;                                                              
        }
        else
        {
            bootCount = 1;                                                                          // store the boot count, starting at [1]
            fprintf(bootFile, "%d", bootCount);                                           
        }
    }
    // Attempt to read previous system boot count from file                                         // ["r+"] mode: read current boot count
    else if (fscanf(bootFile, "%d", &bootCount) != 1)                                     
    {
        bootCount = 1;                                                                              // file is empty or invalid → restart count
    }
    else                                                                                  
    {
        bootCount++;                                                                                // increment valid boot count by [1]
    }

    /* UPDATE SYSTEM BOOT COUNT */
    rewind(bootFile);                                                                               // ["r+"] mode: reposition to start before writing
    fprintf(bootFile, "%d", bootCount);                                                   
    fclose(bootFile);                                                                    

    return exitCode;
}

void setupPassword(char passwordFileName[], FILE *passwordFile) 
{
    char passNew[MAX_PASSWORD_LENGTH], confirmPassNew[MAX_PASSWORD_LENGTH]; 
    int noSpace = 1, runSetup = 0;                                                                                

    memset(passNew, 0, MAX_PASSWORD_LENGTH);                                                                      
    memset(confirmPassNew, 0, MAX_PASSWORD_LENGTH);                                                              

    /* DETERMINE IF PASSWORD SETUP IS REQUIRED */
    runSetup = (bootCount == 1 || (bootCount > 1 && !validCurrentPassword(passwordFileName, passwordFile)));       // first boot or invalid stored password

    /* BEGIN PASSWORD SETUP IF NECESSARY */
    if (runSetup)
    {
        getBackupCode();                                                                                           // verify user identity with backup
        clearScreen();                                                                                             // reset console display

        do 
        {
            /* PROMPT USER TO ENTER NEW PASSWORD */
            printf("\n+-------------------------------------------------------------+\n");
            printf("|                   SETUP YOUR PASSWORD                       |\n");
            printf("+-------------------------------------------------------------+\n");
            printf("| - Minimum 8 characters.                                     |\n");
            printf("| - No spaces allowed.                                        |\n");
            printf("+-------------------------------------------------------------+\n");     
            printf("\nNEW PASSWORD: ");

            fgets(passNew, MAX_PASSWORD_LENGTH, stdin);                                                          
            passNew[strcspn(passNew, "\n")] = 0;                                                                   // remove '\n' for comparison

            // CHECK PASSWORD RULES: MINIMUM LENGTH & NO SPACES
            if (strlen(passNew) < (size_t)8 || !validText(passNew, noSpace, 0))                                    // passwords are not text only ---> 0 is passed                    
            {   
                printEntryError();
                continue;                                                                                          // restart loop
            }

            /* PROMPT USER TO CONFIRM NEW PASSWORD */
            printf("\n+-------------------------------------------------------------+\n");
            printf("|                   CONFIRM YOUR PASSWORD                     |\n");
            printf("+-------------------------------------------------------------+\n");
            printf("| - Minimum 8 characters.                                     |\n");
            printf("| - No spaces allowed.                                        |\n");
            printf("+-------------------------------------------------------------+\n");     
            printf("\nNEW PASSWORD: ");

            fgets(confirmPassNew, MAX_PASSWORD_LENGTH, stdin);                                                
            confirmPassNew[strcspn(confirmPassNew, "\n")] = 0;                                                   

            // ENSURE BOTH PASSWORDS MATCH
            if (strcmp(passNew, confirmPassNew) != 0)
            {
                printEntryError();
                continue;                                                                                        
            } 

        } while (strcmp(passNew, confirmPassNew) != 0);                                                            // repeat until valid and matching
    }

    /* STORE NEW PASSWORD TO GLOBAL VARIABLE */
    // Set global password for first session
    strcpy(password, passNew);                                                                                     // updatePassword() is called for other sessions
}

int updatePassword(char passwordFileName[], FILE *passwordFile) 
{
    int exitCode = 0;

    /* ATTEMPT TO OPEN PASSWORD FILE FOR READ/WRITE */
    passwordFile = fopen(passwordFileName, "r+");                                                   // mode ["r+"] ---> open existing password file for update
    if (!passwordFile)                                                                              
    {
        /* CREATE NEW PASSWORD FILE */
        passwordFile = fopen(passwordFileName, "w");                                                // mode ["w"] ---> create new file
        if ((exitCode = fileError(passwordFileName, passwordFile)))                          
        {
            return exitCode;
        }
        else                                                                                        // file created successfully
        {
            setupPassword(passwordFileName, passwordFile);                                          // prompt user to enter new password
            fprintf(passwordFile, "%s", password);                                                  // store new password
        }
    }
    else                                                                                           
    {
        /* RESET CURSOR TO FILE START */
        rewind(passwordFile);                                                                       // ["r+"] requires manual cursor control

        /* READ EXISTING PASSWORD FROM FILE */
        if (fscanf(passwordFile, "%99s", password) != 1 && !strlen(password))                       // if no password exists in file
        {
            setupPassword(passwordFileName, passwordFile);                                          // prompt user to set password
            fprintf(passwordFile, "%s", password);                                                  // write it to file
        } 
        else                                                                                  
        {
            rewind(passwordFile);                                                                   // return to beginning to overwrite
            fprintf(passwordFile, "%s", password);                                                  // update with current (possibly reset) password
        }
    }

    fclose(passwordFile);
    return exitCode;
}


int mainMenu(char passwordFileName[], Invoice *customerInfo, Supplier *supplierInfo, FILE *passwordFile) 
{
    int menuOption, exitCode = 0;

    do
    {
        clearScreen();
        printf("\n+----------------------------------------------------------------------------------+\n");
        printf("|                                    MAIN MENU                                     |\n");
        printf("+----------------------------------------------------------------------------------+\n");
        printf("|                                                                                  |\n");
        printf("|  [1] Inventory Management                                                        |\n");
        printf("|                                                                                  |\n");
        printf("|----------------------------------------------------------------------------------|\n");
        printf("|                                                                                  |\n");
        printf("|  [2] Reset Password                                                              |\n");
        printf("|                                                                                  |\n");
        printf("|----------------------------------------------------------------------------------|\n");
        printf("|                                                                                  |\n");
        printf("|  [3] Exit System                                                                 |\n");
        printf("|                                                                                  |\n");
        printf("+----------------------------------------------------------------------------------+\n");
        menuOption = getValidQty("\nMENU OPTION: ", 1);                                    // min qty ---> 1

        switch (menuOption)
        {
            case 1:
                exitCode = inventoryMenu(passwordFileName, customerInfo, supplierInfo, passwordFile);
                break;  
                    
            case 2:
                exitCode = resetPassword(passwordFileName, passwordFile);
                break;

            case 3:
                exitCode = 0;
                break;

            default:
                printEntryError();
                pauseForSeconds(1);
                break;
        }

    } while (menuOption < 1 || menuOption > 3);

    return exitCode;
}

int validCurrentPassword(char passwordFileName[], FILE *passwordFile) 
{
    char buffer[MAX_PASSWORD_LENGTH];
    int exitCode = 0, validPassword = 1;

    /* OPEN PASSWORD FILE & HANDLE ERRORS */                                
    // Attempt to open the password file for reading                                              
    passwordFile = fopen(passwordFileName, "r");                                                          
    if ((exitCode = fileError(passwordFileName, passwordFile)) == -1)                                 
    {
        return exitCode;
    }

    /* VALIDATE THE PASSWORD STORED IN FILE */
    if (fscanf(passwordFile, "%s", buffer) != 1)                                        // file is empty or fscanf() couldn't read trash
    {
        validPassword = 0;
        fprintf(stderr, "\n+----------------------------------------------------------------+\n");
        fprintf(stderr, "|                          ERROR MESSAGE                         |\n");
        fprintf(stderr, "+----------------------------------------------------------------+\n");
        fprintf(stderr, "| Unexpectedly, the password stored is invalid or missing.       |\n");
        fprintf(stderr, "| Please reset your password.                                    |\n");
        fprintf(stderr, "+----------------------------------------------------------------+\n");
        pauseForSeconds(5);                                                                                    
    }
    
    fclose(passwordFile);
    return validPassword;
}

int inventoryMenu(char passwordFileName[], Invoice *customerInfo, Supplier *supplierInfo, FILE *passwordFile) 
{
    FILE *inventoryFile = NULL;    

    char inventoryFileName[50];                                                         // holds space for the chosen inventory 
    static int validEntries;                                                            // static variables ---> retain state until next boot
    int invalidEntries, inventoryOption, exitCode = 0;

    /* GET A VALID INVENTORY MENU OPTION */
    do 
    {
        // Request password entry for security reasons
        if (validEntries == 0 || invalidEntries >= 25)   
        {
            enterPassword(passwordFileName, passwordFile);
        }

        printf("\n+-----------------------------------------------------------+\n");
        printf("|                       INVENTORY MENU                      |\n");
        printf("+-----------------------------------------------------------+\n");
        printf("|  [1] Enter Customer Invoice                               |\n");
        printf("|  [2] Modify Customer Invoice                              |\n");
        printf("|  [3] Search Customer Record                               |\n");
        printf("|  [4] View Customer Invoice                                |\n");
        printf("+-----------------------------------------------------------+\n");   
        printf("+-----------------------------------------------------------+\n");   
        printf("|  [5] Enter Supplier Info                                  |\n");
        printf("|  [6] Modify Supplier Info                                 |\n");
        printf("|  [7] Search Supplier Record                               |\n");
        printf("|  [8] View Supplier Info                                   |\n");
        printf("|  [9] Return to Main Menu                                  |\n");
        printf("+-----------------------------------------------------------+\n");   
        inventoryOption = getValidQty("\nMENU OPTION: ", 1);                                // min qty ---> 1

       /* ASSIGNMENT OF RELEVANT FILE NAME */
       if (inventoryOption >= 1 && inventoryOption <= 4) strcpy(inventoryFileName, "invoice.txt");
       if (inventoryOption >= 5 && inventoryOption <= 8) strcpy(inventoryFileName, "supplier_info.txt");

        switch (inventoryOption)
        {
            // Case 1 || 5 
            case 1: 
            case 5:
                validEntries++;
                exitCode = createInventory(inventoryOption, inventoryFileName, customerInfo, supplierInfo, inventoryFile);
                break;

            // Case 2 || 6
            case 2:
            case 6:
                validEntries++;
                exitCode = modifyInventory(inventoryOption, inventoryFileName, customerInfo, supplierInfo, inventoryFile); 
                break;

            // Case 3 || 7 
            case 3:
            case 7:
                validEntries++;
                exitCode = searchInventory(inventoryFileName, inventoryOption, customerInfo, supplierInfo, inventoryFile);
                break;

            // Case 4 || 8
            case 4:
            case 8:
                validEntries++;
                exitCode = viewInventory(inventoryFileName, inventoryFile);
                break;

            case 9:
                exitCode = 0;
                printf("\n+----------------------------------------------------------------+\n");
                printf("|                         SYSTEM MESSAGE                         |\n");
                printf("+----------------------------------------------------------------+\n");
                printf("| Returning to main menu...                                      |\n");
                printf("+----------------------------------------------------------------+\n");        
                pauseForSeconds(3);
                break;

            default:
                invalidEntries++;
                printEntryError();
                pauseForSeconds(3);
                clearScreen();
                break;       
        }

    } while (inventoryOption != 9);

    return exitCode;
}

int resetPassword(char passwordFileName[], FILE *passwordFile) 
{   
    int resetMethod, exitCode = 0;

    /* GET VALID NEW PASSWORD */
    do 
    {
        printf("\n+----------------------------------------------------------------+\n");
        printf("|                         RESET PASSWORD                         |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| [1] Enter current password                                     |\n");
        printf("| [2] Use backup codes                                           |\n");
        printf("+----------------------------------------------------------------+\n");       
        resetMethod = getValidQty("\nRESET METHOD: ", 1);                                   // min qty ---> 1
        switch (resetMethod)
        {
            case 1:
                enterPassword(passwordFileName, passwordFile);
                setupPassword(passwordFileName, passwordFile);
                break;

            case 2:
                getBackupCode();
                setupPassword(passwordFileName, passwordFile);
                break;
        }

    } while (resetMethod < 1 || resetMethod > 2);

    /* UPDATE NEW PASSWORD */
    exitCode = updatePassword(passwordFileName, passwordFile);
    if (!exitCode)
    {
        clearScreen();
        printf("+------------------------------------------------------------+\n");
        printf("|                       SYSTEM MESSAGE                       |\n");
        printf("+------------------------------------------------------------+\n");
        printf("|                 Password reset successful!                 |\n");
        printf("+------------------------------------------------------------+\n");
        pauseForSeconds(1);
    }

    return exitCode;
}

void enterPassword(char passwordFileName[], FILE *passwordFile)            
{   
    char passInput[MAX_PASSWORD_LENGTH], confirmPassInput[MAX_PASSWORD_LENGTH]; 
    int wrongPassInput = 0;

    /* VALIDATE ENTERED PASSWORD */
    do 
    {
        clearScreen();
        printf("\n+----------------------------------------------------------------+\n");
        printf("|                         USER LOGIN                             |\n");
        printf("+----------------------------------------------------------------+\n");
        printf("| Enter your current password (no spaces)                        |\n");
        printf("+----------------------------------------------------------------+\n"); 
        printf("\nPASSWORD: ");

        fgets(passInput, MAX_PASSWORD_LENGTH, stdin);
        passInput[strcspn(passInput, "\n")] = 0;  

        printf("\n+----------------------------------------------------------------+\n");
        printf("|                         USER LOGIN                             |\n");
        printf("+----------------------------------------------------------------+\n");
        printf("| Confirm your current password (no spaces)                      |\n");
        printf("+----------------------------------------------------------------+\n"); 
        printf("\nCONFIRM PASSWORD: ");

        fgets(confirmPassInput, MAX_PASSWORD_LENGTH, stdin);
        confirmPassInput[strcspn(confirmPassInput, "\n")] = 0;  

        /* REPEAT UNTIL PASSWORDS MATCH */
        if (strcmp(passInput, confirmPassInput) != 0 || strcmp(passInput, password) != 0)
        {
            wrongPassInput++;
            clearScreen();
            printEntryError();
            pauseForSeconds(1);

            // Check whether user is required to password reset
            if (wrongPassInput >= 25)
            {
                printf("\n+----------------------------------------------------------------+\n");
                printf("|                        SYSTEM MESSAGE                          |\n");
                printf("+----------------------------------------------------------------+\n");
                printf("| Too many failed attempts.                                      |\n");
                printf("| For security reasons, kindly reset your password.              |\n");
                printf("+----------------------------------------------------------------+\n");
                resetPassword(passwordFileName, passwordFile);
            }

            continue;
        }

    } while (strcmp(passInput, confirmPassInput) != 0 || strcmp(passInput, password) != 0);
    
    printf("\n+----------------------------------------------------------------+\n");
    printf("|                         SYSTEM MESSAGE                         |\n");
    printf("+----------------------------------------------------------------+\n");
    printf("| Correct Password. Welcome!                                     |\n");
    printf("+----------------------------------------------------------------+\n");    
}

int createInventory(int inventoryOption, char inventoryFileName[], Invoice *customerInfo, Supplier *supplierInfo, FILE *inventoryFile)  
{
    int bulkRecords, exitCode = 0;

    /* MAKE INVENTORY FILE HEADER */
    exitCode = makeInventoryHeader(inventoryFileName, inventoryOption, inventoryFile);              // creates header if non-existent

    /* PREPARE INVENTORY FILE */
    inventoryFile = fopen(inventoryFileName, "a+");                                                 // ["a+"] ---> read/append
    if ((exitCode = fileError(inventoryFileName, inventoryFile)))
    {
        return exitCode;
    }  
                                    
    /* PROMPT USER TO ENTER RECORD(S) */
    clearScreen();
    printf("\n+----------------------------------------------------------------+\n");
    printf("|                        ENTER INVENTORY                         |\n");
    printf("+----------------------------------------------------------------+\n");
    printf("| Kindly enter the number of records you wish to                 |\n");
    printf("| add to the file.                                               |\n");
    printf("+----------------------------------------------------------------+\n");            
    bulkRecords = getValidQty("\nNO. OF BULK ENTRIES: ", 1);                                        // [1] ---> mininum # of bulk entries 
    handleBulkEntry(inventoryOption, bulkRecords, customerInfo, supplierInfo, inventoryFile);       // also loops until valid input(s)

    printf("\n+----------------------------------------------------------------+\n");
    printf("|                         SYSTEM MESSAGE                         |\n");
    printf("+----------------------------------------------------------------+\n");
    printf("| Your inventory has been successfully generated.                |\n");
    printf("| Thank you for your patience.                                   |\n");
    printf("+----------------------------------------------------------------+\n");

    fclose(inventoryFile);                                                                  
    return exitCode;
}

int modifyInventory(int inventoryOption, char inventoryFileName[], Invoice *customerInfo, Supplier *supplierInfo, FILE *inventoryFile)  
{
    FILE *tempInventoryFile = NULL;
    char *savedRecord;
    char tempFileName[50], record[MAX_RECORD_SIZE];
    int exitCode = 0;

    /* DYNAMICALLY NAME THE BLANK FILE */
    sprintf(tempFileName, "temp_%s", inventoryFileName);                                                                    // "temp_" precedes the file name passed

    /* CREATE A BLANK FILE */
    tempInventoryFile = fopen(tempFileName, "w");
    if ((exitCode = fileError(tempFileName, tempInventoryFile)) == -1) 
    { 
        return exitCode; 
    }

    /* START MODIFICATION PROCESS */
    // Get Record to Modify
    if ((exitCode = searchInventory(inventoryFileName, inventoryOption, customerInfo, supplierInfo, inventoryFile)))        // ask user ---> save fields & record
    {
        return exitCode;
    } 
    savedRecord = saveModifyRecord(inventoryOption, NULL, customerInfo, supplierInfo);                                      // return saved record
        
    // Read from file [after search]
    inventoryFile = fopen(inventoryFileName, "r");                                                                  
    if ((exitCode = fileError(inventoryFileName, inventoryFile)))
    {
        return exitCode;
    }

    // Copy Exisiting Inventory
    rewind(inventoryFile);
    while (fgets(record, MAX_RECORD_SIZE, inventoryFile))
    {
        record[strcspn(record, "\n")] = 0;                                                                                  // remove '\n' for comparison
        if (strcmp(record, savedRecord) == 0)                                                                               // file record ≡ record to modify
        {
            handleModifyEntry(inventoryOption, customerInfo, supplierInfo, tempInventoryFile);                              // user updates desired field(s)
        }
        else
        {
            fprintf(tempInventoryFile, "%s", record);                                                                       // unchanged records printed
        }
    }   

    /* SWAP FILES TO APPLY CHANGES */
    // Close files before proceeding
    fclose(inventoryFile);
    fclose(tempInventoryFile);

    // Attempt to remove (original) and rename (temp)
    if (remove(inventoryFileName) != 0)
    {
        exitCode = -4;
        handleInventoryErrors(inventoryOption, exitCode, inventoryFileName);
    }
    else if (rename(tempFileName, inventoryFileName) != 0)
    {
        exitCode = -4;
        handleInventoryErrors(inventoryOption, exitCode, tempFileName);
    }

    return exitCode;
}

int searchInventory(char inventoryFileName[], int inventoryOption, Invoice *customerInfo, Supplier *supplierInfo, FILE *inventoryFile)  
{
    char *savedRecord;
    char currentLabel[15], currentIDFormat[ID_LENGTH], primaryKey[ID_LENGTH], record[MAX_RECORD_SIZE];                                       
    int invoiceID = 0, supplierID = 0; 
    int stringLength, found, exitCode = 0, i;          

    inventoryFile = fopen(inventoryFileName, "r");
    if ((exitCode = fileError(inventoryFileName, inventoryFile)))
    {
        return exitCode;
    }

    /* DYNAMICALLY HANDLE PROMPT KEYWORDS */    
    switch (inventoryOption)
    {   
        // Case 2 || 6 ---> Search to Modify
        // Case 3 || 7 ---> Search to Display Record
        case 2:                                                                     
        case 3: 
            invoiceID = 1;
            strcpy(currentLabel, "INVOICE");
            strcpy(currentIDFormat, "INV-XXXX");
            break;

        case 6:
        case 7:
            supplierID = 1;
            strcpy(currentLabel, "SUPPLIER");
            strcpy(currentIDFormat, "SUP-XXXX");
            break;
    }

    do 
    {   
        found = 0;

        printf("\n+----------------------------------------------------------------+\n");
        printf("|                         SEARCH FOR RECORD                      |\n");
        printf("+----------------------------------------------------------------+\n");
        printf("| Kindly enter the primary key of the desired record.            |\n");
        printf("+----------------------------------------------------------------+\n");
        printf("\n%s ID [%s]: ", currentLabel, currentIDFormat);

        fgets(primaryKey, ID_LENGTH, stdin);                 
        primaryKey[strcspn(primaryKey, "\n")] = 0;                                  // remove '\n' before calculating length
        stringLength = strlen(primaryKey);

        // Check whether ID is valid
        printf("Primary Key: %s", primaryKey);                                           

        if (!validID(primaryKey, invoiceID, supplierID, stringLength))                                    
        {
            continue;
        }

        /* SEARCH FILE FOR PRIMARY KEY [ID] */ 
        rewind(inventoryFile);
        while (fgets(record, MAX_RECORD_SIZE, inventoryFile))
        {
            if (strstr(record, primaryKey))
            {
                found = 1;

                // Store the found record 
                savedRecord = saveModifyRecord(inventoryOption, record,                        // also updates struct (backend) for other functions
                                                customerInfo, supplierInfo); 
                break;       
            }
        }
        printf("\n");     

        if (found)
        {
            // Wait for user input to print found record
            clearInputBuffer();
            printf("\n\nPress any key to view record...");
            getchar();

            // Print header
            rewind(inventoryFile);
            for (i = 1; i <= 3; i++)
            {
                fgets(record, MAX_RECORD_SIZE, inventoryFile);
                printf("%s", record);
            }
            printf("%s\n", savedRecord);
        }
        else
        {
            printf("\n+----------------------------------------------------------------+\n");
            printf("|                        ERROR MESSAGE                           |\n");
            printf("+----------------------------------------------------------------+\n");
            printf("| The entered primary key does not exist within the file.        |\n");
            printf("| Please try again.                                              |\n");
            printf("+----------------------------------------------------------------+\n");
            pauseForSeconds(1);
            clearInputBuffer();
        }

    } while (!found);

    fclose(inventoryFile);
    return exitCode;
}

int viewInventory(char inventoryFileName[], FILE *inventoryFile)    
{
    int exitCode = 0;
    char record[MAX_RECORD_SIZE];                                 

    inventoryFile = fopen(inventoryFileName, "r");
    if ((exitCode = fileError(inventoryFileName, inventoryFile)))
    {
        return exitCode;
    }

    printf("\n");                                                       // cleaner output after other function calls

    /* DISPLAY THE ENTIRE FILE */
    rewind(inventoryFile);
    while (fgets(record, MAX_RECORD_SIZE, inventoryFile))
    {
        printf("%s", record);
    }

    /* WAIT FOR USER INPUT */
    printf("\n\nPress any key to continue...");
    getchar();  

    return exitCode;
}

int makeInventoryHeader(char inventoryFileName[], int inventoryOption, FILE *inventoryFile) 
{    
    char buffer[MAX_RECORD_SIZE];                                  
    int exitCode = 0, i;

    inventoryFile = fopen(inventoryFileName, "r+");
    if ((exitCode = fileError(inventoryFileName, inventoryFile)))
    {
        return exitCode;
    }

    /* SET CURSOR TO START OF FILE */
    rewind(inventoryFile);                                                          
    
    /* CHECK FOR HEADER */
    for (i = 1; i <= 3; i++) 
    {
        fgets(buffer, MAX_RECORD_SIZE, inventoryFile);              // buffer stores last line of the header
    } 

   /* CREATE INVENTORY HEADER */                  
    switch (inventoryOption)
    {
        case 1:
            if (!strstr(buffer, "+---------------+"))                   // for correct inventory ---> header pattern is non-existent
            {
                fprintf(inventoryFile, "+---------------+---------------------------+---------------------------+--------------------------------+-------------------+-----------------+---------------+-----------------------+\n");
                fprintf(inventoryFile, "| %-13s | %-25s | %-25s | %-30s | %-17s | %-15s | %-13s | %-21s |\n", 
                "Invoice ID", "First Name", "Last Name", "Product Purchased", "Date Purchased", "Purchase Qty", "MSRP (JMD)", "Total Price");
                fprintf(inventoryFile, "+---------------+---------------------------+---------------------------+--------------------------------+-------------------+-----------------+---------------+-----------------------+\n");
            }
            break;

        case 5:
            if (!strstr(buffer, "+---------------+"))
            {
                fprintf(inventoryFile, "+---------------+---------------------------+---------------------------+------------------------------------------+--------------------------------+-----------------+----------------+\n");
                fprintf(inventoryFile, "| %-13s | %-25s | %-25s | %-40s | %-30s | %-15s | %-14s |\n", 
                "Supplier ID", "Surname", "Contact No.", "Email", "Product Supplied", "Product Qty", "MSRP (JMD)");
                fprintf(inventoryFile, "+---------------+---------------------------+---------------------------+------------------------------------------+--------------------------------+-----------------+----------------+\n");
            }
            break;
    }

    fclose(inventoryFile);
    return exitCode;
}

void handleBulkEntry(int inventoryOption, int bulkRecords, Invoice *customerInfo, Supplier *supplierInfo, FILE *inventoryFile)  
{
    char primaryKey[MAX_RECORD_SIZE];
    int invoiceID, supplierID, space , noSpace, textOnly;
    int count, dupe, exitCode = 0;                                                                         

    /* ENTER ALL RECORDS */
    for (count = 1; count <= bulkRecords; count++)
    {
        if (inventoryOption == 1)
        {
            // Invoice ID
            invoiceID = 1;
            enterInventory("\nKindly enter the invoice ID [INV-XXXX] for customer #%d: ",
                           count, customerInfo->invoiceID, invoiceID, 0, 0, 0, 0, ID_LENGTH);
            invoiceID = 0;

            // No-space Text ---> [Names]
            noSpace = textOnly = 1;
            enterInventory("\nKindly enter the first name for customer #%d: ",
                           count, customerInfo->firstName, 0, 0, 0, noSpace, textOnly, MAX_NAME_LENGTH);
            enterInventory("\nKindly enter the last name for customer #%d: ",
                           count, customerInfo->lastName, 0, 0, 0, noSpace, textOnly, MAX_NAME_LENGTH);
            noSpace = textOnly = 0;

            // Text with Space ---> [Product]
            space = textOnly = 1;
            enterInventory("\nKindly enter the product purchased by customer #%d: ",
                           count, customerInfo->productPurchased, 0, 0, space, 0, textOnly, MAX_PRODUCT_LENGTH);
            space = textOnly = 0;

            // Date ---> [No spaces]
            noSpace = 1;
            enterInventory("\nKindly enter the date [dd/mm/yy] the product was purchased by customer #%d: ",
                           count, customerInfo->datePurchased, 0, 0, 0, noSpace, 0, MAX_DATE_LENGTH);
            noSpace = 0;

            // Quantity and Price
            customerInfo->qty = getValidQty("\nKindly enter the quantity of the product purchased: ", 1);
            customerInfo->priceJMD = getValidPrice("\nKindly enter the base price [JMD] of the product purchased: ", 100);
        }
        else if (inventoryOption == 5)
        {
            // Supplier ID
            supplierID = 1;
            enterInventory("\nKindly enter supplier ID [SUP-XXXX] for supplier #%d: ",
                           count, supplierInfo->supplierID, 0, supplierID, 0, 0, 0, ID_LENGTH);
            supplierID = 0;

            // No-space Text ---> [Surname]
            noSpace = textOnly = 1;
            enterInventory("\nKindly enter the surname for supplier #%d: ",
                           count, supplierInfo->surname, 0, 0, 0, noSpace, textOnly, MAX_NAME_LENGTH);
            noSpace = textOnly = 0;

            // No-space Input ---> [Contact & Email]
            noSpace = 1;
            enterInventory("\nKindly enter the contact number [XXX-XXX-XXXX] for supplier #%d: ",
                           count, supplierInfo->contact, 0, 0, 0, noSpace, 0, MAX_PHONE_NUMBER_LENGTH);
            enterInventory("\nKindly enter the email [username@example.com] for supplier #%d: ",
                           count, supplierInfo->supplierEmail, 0, 0, 0, noSpace, 0, MAX_EMAIL_LENGTH);
            noSpace = 0;

            // Text with Space ---> [Product]
            space = textOnly = 1;
            enterInventory("\nKindly enter the product supplied by supplier #%d: ",
                           count, supplierInfo->productSupplied, 0, 0, space, 0, textOnly, MAX_PRODUCT_LENGTH);
            space = textOnly = 0;

            // Quantity and Price
            supplierInfo->qty = getValidQty("\nKindly enter the quantity of the product supplied: ", 1);
            supplierInfo->priceJMD = getValidPrice("\nKindly enter the base price [JMD] of the product supplied: ", 100);
        }

        /* WAIT FOR USER INPUT */  
        printf("\nPress any key to confirm record...");
        getchar();

        /* CHECK FOR DUPLICATE RECORD */     
        // Use ternary construct ---> (condition) ? TRUE : FALSE 
        strcpy(primaryKey, (inventoryOption == 1) ? customerInfo->invoiceID : supplierInfo->supplierID);    // copies primary key based on chosen inventory
        dupe = duplicateInventory(primaryKey, inventoryFile);
        if (dupe)
        {
            // Reset incrementation
            count--;                                                                                        // decrements [eg., #1 ---> #0] 
            continue;                                                                                       // iterates [eg., #0 ---> #1]
        }

        /* HANDLE RECORD LIMIT EXCEEDED */
        exitCode = writeInventoryRecords(inventoryOption, customerInfo, supplierInfo, inventoryFile);      
        if (exitCode)
        {
            getchar();
            count--;
            continue;
        }
    }
}

char* saveModifyRecord(int inventoryOption, char record[], Invoice *customerInfo, Supplier *supplierInfo) 
{
    static char savedRecord[MAX_RECORD_SIZE];                                                               // stores the saved record until next boot
    char *token = NULL;
    int i = 0;

    /* RETURN EXISTING SAVED RECORD */
    if (!record)                                                                                            // NULL parameter for record[] when invoked
    { 
        return savedRecord; 
    }
    else
    {
        /* COPY THE FOUND RECORD */
        strcpy(savedRecord, record);
        savedRecord[strcspn(savedRecord, "\n")] = 0;                                                        // removes '\n' for future comparisons
    }

    /* UPDATE RECORD FIELDS */                                                                              // user only updates selected field in modifyInventory()
    switch (inventoryOption)
    {
        case 2:  // Invoice modification
            token = strtok(record, "|");                                                                    // strtok() ---> gets each field, seperated by "|"  
            if (token) { trimSpaces(token); strcpy(customerInfo->invoiceID, token); }                       

            token = strtok(NULL, "|");                                                                      // NULL parameter after strtok() ---> next record field
            if (token) { trimSpaces(token), strcpy(customerInfo->firstName, token); }

            token = strtok(NULL, "|");
            if (token) { trimSpaces(token), strcpy(customerInfo->lastName, token); }                             

            token = strtok(NULL, "|");
            if (token) { trimSpaces(token), strcpy(customerInfo->productPurchased, token); }

            token = strtok(NULL, "|");
            if (token) { trimSpaces(token), strcpy(customerInfo->datePurchased, token); }

            token = strtok(NULL, "|");
            if (token) { trimSpaces(token), customerInfo->qty = atoi(token); }                              // atoi() --> char[] to integer

            token = strtok(NULL, "|");
            if (token) 
            {   
                while (token[i] != '\0') 
                {
                    if (token[i] == '$')                                    
                    {
                        token[i] = ' ';                                                                     // removes '$'                                  
                    }
                    i++;
                }
                trimSpaces(token);
                customerInfo->priceJMD = atof(token);                                                       // atof() ---> char[] to float
            }
            break;

        case 6:  // Supplier modification
            token = strtok(record, "|");
            if (token) { trimSpaces(token); strcpy(supplierInfo->supplierID, token); }

            token = strtok(NULL, "|");
            if (token) { trimSpaces(token);  strcpy(supplierInfo->surname, token); }

            token = strtok(NULL, "|");
            if (token) { trimSpaces(token); strcpy(supplierInfo->contact, token); }

            token = strtok(NULL, "|");
            if (token) { trimSpaces(token);  strcpy(supplierInfo->supplierEmail, token); }

            token = strtok(NULL, "|");
            if (token) { trimSpaces(token); strcpy(supplierInfo->productSupplied, token); }

            token = strtok(NULL, "|");
            if (token) { trimSpaces(token); supplierInfo->qty = atoi(token); }

            token = strtok(NULL, "|");
            if (token) 
            {
                while (token[i] != '\0') 
                {
                    if (token[i] == '$') 
                    {
                        token[i] = ' ';
                    } 
                    i++;
                }
                trimSpaces(token);
                supplierInfo->priceJMD = atof(token);
            }
            break;
    }

    return savedRecord;
}

void trimSpaces(char stringHolder[])    
{
    int start = 0, end = strlen(stringHolder) - 1;                                          // string indexes ---> [strlen() - 1] for index, not length
    int i, j = 0;                                                                           // loop variables

    /* SKIP LEADING SPACES */
    while (isspace(stringHolder[start]))                                           
    {
        start++;                                                                            // move start index forward ---> (while space)
    }


    /* SKIP TRAILING SPACES */
    while (end >= start && isspace(stringHolder[end])) 
    {
        end--;                                                                              // move end index backward ---> (while space)
    }

    /* TRIM THE SPACES AT BOTH ENDS */
    for (i = start; i <= end; i++)                                                          
    {
        stringHolder[j++] = stringHolder[i];                                                // start from zero index [j] ---> move each character down the array
    }

    /* END THE TRIMMED STRING */
    stringHolder[j] = '\0';                                                                 // null terminate ---> [end - start] + 1 
}

void showModifyOptions(int inventoryOption)
{
    printf("\n+---------------------------------------------------------+\n");
    printf("|                   MODIFY FIELD OPTIONS                  |\n");
    printf("+---------------------------------------------------------+\n");
    
    if (inventoryOption == 2)  
    {   
        printf("| [1]   Customer Invoice ID                               |\n");
        printf("| [2]   Customer First Name                               |\n");
        printf("| [3]   Customer Last Name                                |\n");
        printf("| [4]   Product Purchased                                 |\n");
        printf("| [5]   Date of Purchase                                  |\n");
        printf("| [6]   Quantity Purchased                                |\n");
        printf("| [7]   MSRP (Base Price) [JMD]                           |\n");
    }
    else if (inventoryOption == 6)
    {
        printf("| [1]   Supplier ID                                       |\n");
        printf("| [2]   Supplier Surname                                  |\n");
        printf("| [3]   Supplier Contact Number                           |\n");
        printf("| [4]   Supplier Email                                    |\n");
        printf("| [5]   Product Supplied                                  |\n");
        printf("| [6]   Quantity Supplied                                 |\n");
        printf("| [7]   Supplier Price (Base Price) [JMD]                 |\n");
    }

    printf("| [8]   Confirm Modification                              |\n");
    printf("+---------------------------------------------------------+\n");
}

void handleModifyEntry(int inventoryOption, Invoice *customerInfo, Supplier *supplierInfo, FILE *inventoryFile)  
{                                                                               
    int space = 0, noSpace = 0, textOnly = 0, invoiceID = 0, supplierID = 0;
    int fieldOption, dupe, exitCode = 0;

    do 
    {
        showModifyOptions(inventoryOption);
        fieldOption = getValidQty("\nMENU OPTION: ", 1);

        switch (inventoryOption)
        {
            /* MODIFY INVOICE INVENTORY */ 
            case 2:
                switch (fieldOption)
                {
                    case 1: // Modify Invoice ID
                        invoiceID = 1;
                        enterInventory("\nKindly enter the invoice ID [INV-XXXX] for customer #%d: ",
                                        1, customerInfo->invoiceID, invoiceID, 0, 0, 0, 0, ID_LENGTH);
                        break;
    
                    case 2: // Modify Customer First Name
                        noSpace = textOnly = 1;
                        enterInventory("\nKindly enter the first name for customer #%d: ", 
                                        1, customerInfo->firstName, 0, 0, 0, noSpace, textOnly, MAX_NAME_LENGTH);
                        break;
    
                    case 3: // Modify Customer Last Name
                        noSpace = textOnly = 1;
                        enterInventory("\nKindly enter the last name for customer #%d: ", 
                                        1, customerInfo->lastName, 0, 0, 0, noSpace, textOnly, MAX_NAME_LENGTH);
                        break;
    
                    case 4: // Modify Purchased Product 
                        space = textOnly = 1;
                        enterInventory("\nKindly enter the product purchased by customer #%d: ", 
                                        1, customerInfo->productPurchased, 0, 0, space, 0, textOnly, MAX_PRODUCT_LENGTH);
                        break;
    
                    case 5: // Modify Purchase Date
                        noSpace = 1;
                        enterInventory("\nKindly enter the date [dd/mm/yy] the product was purchased by customer #%d: ", 
                                        1, customerInfo->datePurchased, 0, 0, 0, noSpace, 0, MAX_DATE_LENGTH);
                        break;
    
                    case 6: // Modify Purchase Qty
                        customerInfo->qty = getValidQty("\nKindly enter the quantity of the product purchased: ", 1);
                        break;
    
                    case 7: // Modify Product MSRP
                        customerInfo->priceJMD = getValidPrice("\nKindly enter the base price [JMD] of the product purchased: ", 100);
                        break;
    
                    case 8: // Apply Modifications
                        printf("\n\nPress any key to confirm modification...");
                        getchar();
                        dupe = duplicateInventory(supplierInfo->supplierID, inventoryFile);
                        break;
                }
                break;
    
            /* MODIFY SUPPLIER INVENTORY */ 
            case 6: 
                switch (fieldOption)
                {
                    case 1: // Modify Supplier ID
                        supplierID = 1;
                        enterInventory("\nKindly enter the supplier ID [SUP-XXXX] for supplier #%d: ", 
                                        1, supplierInfo->supplierID, 0, supplierID, 0, 0, 0, ID_LENGTH);
                        break;
    
                    case 2: // Modify Supplier Surname
                        noSpace = textOnly = 1;
                        enterInventory("\nKindly enter the surname for supplier #%d: ", 
                                        1, supplierInfo->surname, 0, 0, 0, noSpace, textOnly, MAX_NAME_LENGTH);
                        break;
    
                    case 3: // Modify Contact No.
                        noSpace = 1;
                        enterInventory("\nKindly enter the contact number [XXX-XXX-XXXX] for supplier #%d: ", 
                                        1, supplierInfo->contact, 0, 0, 0, noSpace, 0, MAX_PHONE_NUMBER_LENGTH);
                        break;
    
                    case 4: // Modify Email
                        noSpace = 1;
                        enterInventory("\nKindly enter the email [username@example.com] for supplier #%d: ", 
                                        1, supplierInfo->supplierEmail, 0, 0, 0, noSpace, 0, MAX_EMAIL_LENGTH);
                        break;
    
                    case 5: // Modify Supplied Product
                        space = textOnly = 1;
                        enterInventory("\nKindly enter the product supplied by supplier #%d: ", 
                                        1, supplierInfo->productSupplied, 0, 0, space, 0, textOnly, MAX_PRODUCT_LENGTH);
                        break;
    
                    case 6: // Modify Supplied Qty
                        supplierInfo->qty = getValidQty("\nKindly enter the quantity of the product supplied: ", 1);
                        break;
    
                    case 7: // Modify Product MSRP
                        supplierInfo->priceJMD = getValidPrice("\nKindly enter the price [JMD] of the product supplied: ", 100);
                        break;
    
                    case 8: // Apply Modifications
                        printf("\n\nPress any key to confirm modification...");
                        getchar();
                        dupe = duplicateInventory(supplierInfo->supplierID, inventoryFile);
                        break;
                }
                break;
        }

    } while (dupe);
    
    if ((exitCode = writeInventoryRecords(inventoryOption, customerInfo, supplierInfo, inventoryFile)))
    {
        handleInventoryErrors(inventoryOption, exitCode, NULL);                                 // NULL --> inventoryFileName[] is not needed for this error
    }
}
