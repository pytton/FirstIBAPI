#include <conio.h>
#include <iostream>
#include <iomanip> //for setprecision
#include <fstream>	//for ofstream out("order.out");
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

#include <Windows.h>

#include "TwsApiL0.h"
#include "TwsApiDefs.h"
//#include <process.h>
using namespace TwsApi;

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


class ResponseFromTWS : public EWrapperL0
{
public:

	double m_priceAsk;
	double m_priceBid;
	double m_BidSize;
	double m_AskSize;

	virtual void tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute)
	{
		switch (field)
		{
		case TickType::ASK:
			m_priceAsk = price;
			break;
		case TickType::BID:
			m_priceBid = price;
			break;
		}
	}

	virtual void tickSize(TickerId tickerId, TickType field, int size)
	{
		switch (field)
		{
		case TickType::BID_SIZE:
			m_BidSize = double(size);
			break;
		case TickType::ASK_SIZE:
			m_AskSize = double(size);
			break;

		}
	}
};

void GetContractDetails(Contract & spyContract)
{

	// use either set to get what you want. both work
	//forex (GBP) works 24/7 mon-friday

	spyContract.symbol = "GBP";
	spyContract.secType = *SecType::CASH;		//"STK"
	spyContract.currency = "USD";
	spyContract.exchange = *Exchange::IDEALPRO;	//"SMART";


	//spyContract.symbol	= "SPY";
	//spyContract.secType	= *SecType::STK;		//"STK"
	//spyContract.currency	= "USD";
	//spyContract.exchange	= *Exchange::ARCA;	//"SMART";
	//spyContract.primaryExchange = *Exchange::ARCA;	//"ARCA";
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
	EClientL0*	pClientTWS = EClientL0::New(twsReply);
	bool bIsConnected = pClientTWS->eConnect("", 7496);
	//the below works without launching it in a thread:
	Contract	spyContract;
	GetContractDetails(spyContract);
	int  tickerID = 100;
	pClientTWS->reqMktData(tickerID, spyContract, "", false);

	using namespace std;
	ofstream out("output.out");

	clock_t time;
	time = clock();

	for (int i = 0; i < 20; i++)
	{
		int Wait = 250;
		Sleep(Wait);
		cout << "sleeping for: " << Wait << " ms" << endl;

		pClientTWS->reqMktData(tickerID, spyContract, "", false);
		double ask = twsReply->m_priceAsk;
		double bid = twsReply->m_priceBid;
		double bidSize = twsReply->m_BidSize;
		double askSize = twsReply->m_AskSize;
		
		time = clock();
		//long currentTime = twsReply->currentTime(); //not working
		cout << setprecision(15);

		cout << "ask: " << ask << endl;
		cout << "bid: " << bid << endl;
		cout << "ask size: " << askSize << endl;
		cout << "bid size: " << bidSize << endl;

		out << setprecision(15);

		out << "time: " << time << endl;
		out << "ask: " << ask << endl;
		out << "bid: " << bid << endl;
		out << "ask size: " << askSize << endl;
		out << "bid size: " << bidSize << endl;

		//cout << "time: " << currentTime << endl;
	}


	cout << endl << "Finished." << endl;
	waitforuserend();

	////////////////////////////////////////////////////////////////
	pClientTWS->eDisconnect();
	delete twsReply;
	////////////////////////////////////////////////////////////////

	return 0;
}
