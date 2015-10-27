#include <conio.h>
#include <iostream>
#include <random>
#include <Windows.h>

#include "TwsApiL0.h"
#include "TwsApiDefs.h"
#include <process.h>
using namespace TwsApi;

class RandomNumber;

int generateID()
{
	static int s_itemID = 0;
	return s_itemID++;
}

void waitforuserend()
{
	using namespace std;
	cin.clear();
	cin.ignore(255, '\n');
	cin.get();
}

class RandomNumber
{
public:
	unsigned int myRandNum;

	static double m_AskPrice;
	static double m_BidPrice;
	static double m_AskSize;
	static double m_BidSize;

	RandomNumber()
	{
		myRandNum = 0;
	};

	double getAskPrice()
	{
		return m_AskPrice;
		//return myRandNum;
	}
	double getBidPrice()
	{
		return m_BidPrice;
		//return myRandNum;
	}
	double getBidSize()
	{
		return m_BidSize;
		//return myRandNum;
	}
	double getAskSize()
	{
		return m_AskSize;
		//return myRandNum;
	}
};


double RandomNumber::m_AskPrice = -1.0;
double RandomNumber::m_BidPrice = -1.0;
double RandomNumber::m_AskSize = -1.0;
double RandomNumber::m_BidSize = -1.0;

class ResponseFromTWS: public EWrapperL0
{
public:

	double m_priceAsk;
	double m_priceBid;
	double m_BidSize;
	double m_AskSize;

	virtual void tickPrice( TickerId tickerId, TickType field, double price, int canAutoExecute )
	{ 
		switch (field)
		{
		case TickType::ASK:
			m_priceAsk = price;
			RandomNumber::m_AskPrice = price;
			break;
		case TickType::BID:
			m_priceBid = price;
			RandomNumber::m_BidPrice = price;
			break;
		}
	}

	virtual void tickSize(TickerId tickerId, TickType field, int size)
	{
		switch (field)
		{
		case TickType::BID_SIZE:
			m_BidSize = double(size);
			RandomNumber::m_BidSize = double(size);
			break;
		case TickType::ASK_SIZE:
			m_AskSize = double(size);
			RandomNumber::m_AskSize = double(size);
			break;

		}
	}
};

void GetContractDetails(Contract & spyContract)
{

	//spyContract.symbol = "GBP";
	//spyContract.secType = *SecType::CASH;		//"STK"
	//spyContract.currency = "USD";
	//spyContract.exchange = *Exchange::IDEALPRO;	//"SMART";


	spyContract.symbol	= "SPY";
	spyContract.secType	= *SecType::STK;		//"STK"
	spyContract.currency	= "USD";
	spyContract.exchange	= *Exchange::IB_SMART;	//"SMART";
	spyContract.primaryExchange = *Exchange::ARCA;	//"ARCA";
}

static unsigned __stdcall RequestMktData(void * pVoidTwsClient)
{
	EClientL0*	pClientTWS = (EClientL0*) pVoidTwsClient;

	Contract	spyContract;
	GetContractDetails(spyContract);

	int  tickerID = 100;

	pClientTWS->reqMktData( tickerID, spyContract, "", false );

	return 1;
}


int StartTWSConn()
{
	//create a function that connects to TWS
	//create a class that has the following:
	//setter - creates a new random number
	//getter - returns a random float

	//create a for loop that iterates 5 times
	//call the function to get a random number
	//cast the number to char *
	//print the number

	//set up the first thread:
	ResponseFromTWS	 * twsReply = new ResponseFromTWS();
	EClientL0*	pClientTWS = EClientL0::New( twsReply );
	bool bIsConnected = pClientTWS->eConnect("", 7496);

	//launch the first thread:
	unsigned  nThreadID = 1;
	HANDLE hThrd = (HANDLE)_beginthreadex( NULL, 0, RequestMktData, (void*) pClientTWS,	CREATE_SUSPENDED, &nThreadID );
	ResumeThread( hThrd );   // serves the purpose of Jaeschke's t1->Start()
	WaitForSingleObject( hThrd, INFINITE );


	using namespace std;
	RandomNumber myNumber; //this is random now - I want this changed to bid price(no need for ask price now)

	for (int i = 0; i < 100; i++)
	{
		int Wait = 50;
		Sleep(Wait);

	//	myNumber.setNumber();
		double ask = myNumber.getAskPrice();
		double bid = myNumber.getBidPrice();
		double bidSize = myNumber.getBidSize();
		double askSize = myNumber.getAskSize();
		
		cout << "ask: " << ask << endl;
		cout << "bid: " << bid << endl;
		cout << "bid size: " << bidSize << endl;
		cout << "ask size: " << askSize << endl;


		cout << "sleeping for: " << Wait << " ms" << endl; 
	}


	cout << endl << "Finished." << endl;
	waitforuserend();

	////////////////////////////////////////////////////////////////
	pClientTWS->eDisconnect();
	delete twsReply;
	////////////////////////////////////////////////////////////////

	return 0;
}
