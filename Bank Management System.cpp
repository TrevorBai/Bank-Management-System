#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>

using namespace std;

class AccountInfo
{
	int acountNumber;
	char accountHolderName[20];  // Allocated 2 more bytes at
	                // the end of 20 bytes memory for some reason, this can
					// be seen after generating binary file
	int depositAmount;
	char accountType;     // checking acount or saving account. Allocated 3 more bytes at
	                // the end of 1 byte memory for some reason, kinda balance out
				    // the memory because ahead of it is a 4 bytes integer, this can
					// be seen after generating binary file
public:
	void createAccount();  // function to get data from user
	int getAcountNumber() const;  // function to return account number
	void showAccount() const;  // function to show data on screen
	void depositAmountIntoExistingAccount(int);  // function to deposite money and add to balance amount
	int getBalanceAmount() const;  // function to return balance amount
	void withdrawAmountIntoExistingAccount(int); // function to withdraw money and update balance amount
	void showAccountInTabularFormat() const;  // function to show data in tabular format
	void modifyExistingAccount();
};

void AccountInfo::createAccount()
{
	cout << "\nEnter The Account No. : ";
	cin >> acountNumber;
	cout << "\n\nEnter The Name of The Account Holder : ";
	cin.ignore();
	cin.getline(accountHolderName, 20);  // extract from stream into char* c-string "name"
	cout << "\n\nEnter Type of The Account (Checking(C) / Saving(S)) : ";
	cin >> accountType;
	accountType = toupper(accountType);  // a method function in cctype header
	cout << "\n\nEnter The Initial Amount ( >= 500 for Checking and >= 1000"
			" for Saving) : ";
	cin >> depositAmount;
}

int AccountInfo::getAcountNumber() const
{
	return acountNumber;
}

void AccountInfo::showAccount() const
{
	cout << "\nAccount No. : " << acountNumber;
	cout << "\nAccount Holder Name : " << accountHolderName;
	cout << "\nType of Account : " << accountType;
	cout << "\nBalance Amount : " << depositAmount;
}

void AccountInfo::depositAmountIntoExistingAccount(int newlyDepositAmount)
{
	depositAmount += newlyDepositAmount;
}

int AccountInfo::getBalanceAmount() const
{
	return depositAmount;
}

void AccountInfo::withdrawAmountIntoExistingAccount(int newlyWithdrawAmount)
{
	depositAmount -= newlyWithdrawAmount;
}

void AccountInfo::showAccountInTabularFormat() const
{
	cout << setw(9) << acountNumber << setw(20) << accountHolderName 
		<< setw(10) << accountType << setw(11) << depositAmount << endl;
	// setw() a method function in iomanip header
}

void AccountInfo::modifyExistingAccount()
{
	cout << "\nAccount No. : " << acountNumber;
	cout << "\n\nPlease Enter Modified Account Holder Name : ";
	cin.ignore();
	cin.getline(accountHolderName, 20);
	cout << "\nPlease Enter Modified Type of The Account (Checking(C) / Saving(S)) : ";
	cin >> accountType;
	accountType = toupper(accountType);
	cout << "\nPlease Enter Modified Balance Amount : ";
	cin >> depositAmount;
}

//########################################################
// The end of class definition
//########################################################

//#######################################################################

//####################################################
// Declare useful functions
//####################################################
void introductionPage();
void mainMenu();
bool checkIfEnteredAccountExists(int);
void writeAccountInfoIntoBinaryFile(); 
void depositOrWithdrawMoneyIntoOrFromAccount(int, int); // function to deposit/withdraw certain
								 // amount for given account
void displayAccountBalance(int);
void displayAllAccountHolders();
void closeExistingAccount(int);
void modifyExistingAccount(int);


// Introduction Page
void introductionPage()
{
	cout << "\n\n\n###############################";
	cout << "\n\n\n\t   BANK";
	cout << "\n\n\tMANAGEMENT";
	cout << "\n\n\t  SYSTEM";
	cout << "\n\n\n###############################";
	cout << "\n\n\n\nMADE BY : Trevor Bai";
	cout << "\n\n\t JUST FOR FUN";
	cin.get();
}

// Main Menu
void mainMenu()
{
	cout << "\n\n\n\tMAIN MENU";
	cout << "\n\n\t01. NEW ACCOUNT";
	cout << "\n\n\t02. DEPOSIT AMOUNT";
	cout << "\n\n\t03. WITHDRAW AMOUNT";
	cout << "\n\n\t04. BALANCE ENQUIRY";
	cout << "\n\n\t05. ALL ACCOUNT HOLDER LIST";
	cout << "\n\n\t06. CLOSE AN ACCOUNT";
	cout << "\n\n\t07. MODIFY AN ACCOUNT";
	cout << "\n\n\t08. EXIT";
	cout << "\n\n\tSelect Your Option (1 - 8) ";
}

bool checkIfEnteredAccountExists(int enteredAccountNumber)
{
	bool enteredAccountExists = false;
	ifstream inputFileStream;
	AccountInfo accountInfo;
	inputFileStream.open("account.dat", ios::binary | ios::in);
	
	while (!inputFileStream.eof())
	{
		inputFileStream.read((char*)(&accountInfo), sizeof(AccountInfo)); // read from file 
												   // account.dat, not from
												   // memory. Then put into
												   // memory.		
		if (accountInfo.getAcountNumber() == enteredAccountNumber)
		{
			enteredAccountExists = true;
		}
	}
	return enteredAccountExists;
}


//#######################################################################
// write new created account info into a binary file
//#######################################################################

void writeAccountInfoIntoBinaryFile()
{
	int enteredAccountNumber;
	AccountInfo accountInfo;
	ofstream outputFileStream;  // write on files
	outputFileStream.open("account.dat", ios::binary | ios::app | ios::out);
	if (!outputFileStream.is_open())
	{
		cout << "Unable to open your account info! Please check it up...";
		return;
	}
	accountInfo.createAccount();
	enteredAccountNumber = accountInfo.getAcountNumber();

	if (!checkIfEnteredAccountExists(enteredAccountNumber))
	{
		//cout << sizeof(account); // sizeof(account) provides us more than 
							 // 4+20+4+1 for some alignment reasons
		outputFileStream.write((char*)(&accountInfo), sizeof(AccountInfo));  // write from memory to
														// file account.dat
		cout << "\n\n\nAccount Created..";
	}
	else
	{
		cout << "\n\nThe Account Number You Entered Already Exists, Please Press Enter"
			    " And Try Another Number!";
		return;
	}
	outputFileStream.close();
}

//#########################################################
// deposit and withdraw money from an existing account
//#########################################################

void depositOrWithdrawMoneyIntoOrFromAccount(int existingAccountNumber, int deposit1Withdraw2)
{
	int newlyDepositOrWithdrawAmount;
	bool checkIfExistingAccountInfoFound = false;
	int currentBalance;
	int amountWithdrawable;
	AccountInfo accountInfo;  // there is nothing in local accountInfo because it is not defined yet
	fstream inputAndOutputFileStream;  // both write on files and read from files
	inputAndOutputFileStream.open("account.dat", ios::binary | ios::in | ios::out);
	if (!inputAndOutputFileStream.is_open())
	{
		cout << "Unable to open your account info! Please check it up...";
		return;  // terminate function depositOrWithdrawMoneyIntoOrFromAccount
	}
	while (!inputAndOutputFileStream.eof() && checkIfExistingAccountInfoFound == false)
	{
		inputAndOutputFileStream.read((char*)(&accountInfo), sizeof(AccountInfo));
		if (accountInfo.getAcountNumber() == existingAccountNumber)
		{
			accountInfo.showAccount();
			if (deposit1Withdraw2 == 1)
			{
				cout << "\n\n\t TO DEPOSITE AMOUNT ";
				cout << "\n\n Please Enter the Amount to be Deposited : ";
				cin >> newlyDepositOrWithdrawAmount;
				accountInfo.depositAmountIntoExistingAccount(newlyDepositOrWithdrawAmount);
			}
			if (deposit1Withdraw2 == 2)
			{
				cout << "\n\n\t TO WITHDRAW AMOUNT ";
				cout << "\n\n Please Enter the Amount to be Withdrawn : ";
				cin >> newlyDepositOrWithdrawAmount;
				currentBalance = accountInfo.getBalanceAmount();
				amountWithdrawable = (currentBalance >= newlyDepositOrWithdrawAmount) ? 1 : 0;
				if (amountWithdrawable)
				{
					accountInfo.withdrawAmountIntoExistingAccount(newlyDepositOrWithdrawAmount);
				}
				else
				{
					cout << "\nCan Not Withdraw $ " << newlyDepositOrWithdrawAmount <<
						" From Your Balance, Insufficient Balance! ";
				}
			}
			int pos = (-1)*static_cast<int>(sizeof(AccountInfo)); // shift pointer to the beginning of
																  // found account info
																  // cast it into a signed int
			inputAndOutputFileStream.seekp(pos, ios::cur);
			inputAndOutputFileStream.write((char*)(&accountInfo), sizeof(AccountInfo));
			cout << "\n\n\t Account Updated Successfully...";
			checkIfExistingAccountInfoFound = true;
		}
	}
	if (inputAndOutputFileStream.eof())
	{
		cout << "\nThe Account Number You Entered Does Not Exist,"
			 << "Please Double Check The Account Number Or "
			 << "Press Enter And Create A New Account! " << endl;
	}
	inputAndOutputFileStream.close();
}

//#############################################################
// display existing account balance picked by user
//#############################################################

void displayAccountBalance(int existingAccountNumber)
{
	AccountInfo accountInfo;
	bool checkIfExistingAccountInfoFound = false;
	ifstream inputFileStream;
	inputFileStream.open("account.dat", ios::binary | ios::in);
	if (!inputFileStream.is_open())
	{
		cout << "Unable to open your account info! Please check it up...";
		return;
	}
	cout << "\n\nBALANCE DETAILS\n";
	while (!inputFileStream.eof() && !checkIfExistingAccountInfoFound)
	{
		inputFileStream.read((char*)(&accountInfo), sizeof(AccountInfo));
		if (accountInfo.getAcountNumber() == existingAccountNumber)
		{
			accountInfo.showAccount();
			checkIfExistingAccountInfoFound = true;
		}
	}
	inputFileStream.close();
	if (inputFileStream.eof())
	{
		cout << "\nThe Account Number You Entered Does Not Exist,"
			<< "Please Double Check The Account Number Or "
			<< "Press Enter And Create A New Account! " << endl;
	}
}

//#############################################################
// display all account holders list
//#############################################################

void displayAllAccountHolders()
{
	AccountInfo accountInfo;
	ifstream inputFileStream;
	inputFileStream.open("account.dat", ios::binary | ios::in);
	if (!inputFileStream.is_open())
	{
		cout << "Unable to open your account info! Please check it up...";
		return;
	}
	cout << "\n\n\t\tACCOUNT HOLDERS LIST\n\n";
	cout << "==============================================================\n";
	cout << "ACCOUNT NUMBER      HOLDER NAME      TYPE    BALANCE\n";
	cout << "==============================================================\n";
	while (inputFileStream.read((char*)(&accountInfo), sizeof(AccountInfo)))
		// It will get rid of duplicated last accountInfo compared to using
		// !inputFileStream.eof()
	{
		accountInfo.showAccountInTabularFormat();
	}
	inputFileStream.close();
}

//#############################################################
// close an existing account given by user
//#############################################################

void closeExistingAccount(int existingAccountNumber)
{
	AccountInfo accountInfo;
	ifstream inputFileStream;
	ofstream outputFileStream;
	inputFileStream.open("account.dat", ios::binary | ios::in);
	if (!inputFileStream.is_open())
	{
		cout << "Unable to open your account info! Please check it up...";
		return;
	}
	outputFileStream.open("temp.dat", ios::binary | ios::out);
	while (inputFileStream.read((char*)(&accountInfo), sizeof(AccountInfo)))
	{
		if (accountInfo.getAcountNumber() != existingAccountNumber)
		{
			outputFileStream.write((char*)(&accountInfo), sizeof(AccountInfo));
		}
	}
	inputFileStream.close();
	outputFileStream.close();
	remove("account.dat");
	rename("temp.dat", "account.dat");
	cout << "\n\n\tEntered Account Deleted ...";
}

//#############################################################
// modify an existing account given by user
//#############################################################

void modifyExistingAccount(int existingAccountNumber)
{
	AccountInfo accountInfo;
	bool checkIfExistingAccountInfoFound = false;
	fstream inputAndOutputFileStream;  // both write on files and read from files
	inputAndOutputFileStream.open("account.dat", ios::binary | ios::in | ios::out);
	if (!inputAndOutputFileStream.is_open())
	{
		cout << "Unable to open your account info! Please check it up...";
		return;
	}
	while (!inputAndOutputFileStream.eof() && !checkIfExistingAccountInfoFound)
	{
		inputAndOutputFileStream.read((char*)(&accountInfo), sizeof(AccountInfo));
		if (accountInfo.getAcountNumber() == existingAccountNumber)
		{
			accountInfo.showAccount();
			cout << "\n\nEnter The New Details Of Account Info" << endl;
			accountInfo.modifyExistingAccount();
			int pos = (-1)*static_cast<int>(sizeof(AccountInfo));
			inputAndOutputFileStream.seekp(pos, ios::cur);
			inputAndOutputFileStream.write((char*)(&accountInfo), sizeof(AccountInfo));
			cout << "\nEntered Account Info Modified Successfully ... ";
			checkIfExistingAccountInfoFound = true;
		}
	}
	if (inputAndOutputFileStream.eof())
	{
		cout << "\nThe Account Number You Entered Does Not Exist,"
			<< "Please Double Check The Account Number Or "
			<< "Press Enter And Create A New Account! " << endl;
	}
	inputAndOutputFileStream.close();
}

//#############################################################
// The Main Function
//#############################################################

int main()
{
	char optionsToChoose;  // User to choose one option
	int existingAccountNumber;  // Existing account number

	introductionPage();
	do
	{
		system("cls");  // clearing the screen
		mainMenu();

		cin >> optionsToChoose;
		system("cls");

		switch (optionsToChoose)
		{
		case '1':
			writeAccountInfoIntoBinaryFile();
			break;
		case '2':
			cout << "\n\n\tPlease Enter Your Account No. :";
			cin >> existingAccountNumber;
			depositOrWithdrawMoneyIntoOrFromAccount(existingAccountNumber, 1);
			break;
		case '3':
			cout << "\n\n\tPlease Enter Your Account No. :";
			cin >> existingAccountNumber;
			depositOrWithdrawMoneyIntoOrFromAccount(existingAccountNumber, 2);
			break;
		case '4':
			cout << "\n\n\tPlease Enter Your Account No. :";
			cin >> existingAccountNumber;
			displayAccountBalance(existingAccountNumber);
			break;
		case '5':
			displayAllAccountHolders();
			break;
		case '6':
			cout << "\n\n\tPlease Enter The Account No. You Would Like To Close : ";
			cin >> existingAccountNumber;
			closeExistingAccount(existingAccountNumber);
			break;
		case '7':
			cout << "\n\n\tPlease Enter The Account No. You Would Like To Modify : ";
			cin >> existingAccountNumber;
			modifyExistingAccount(existingAccountNumber);
			break;
		case '8':
			cout << "\n\n\tThanks For Using Bank Management System!";
			break;
		default:
			cout << "\a";
			cout << "\n\n\tPlease Enter A Valid Option!";
		}
		cin.ignore();
		cin.get();
	} while (optionsToChoose != '8');

	cin.get();
	return 0;
}
