#include <string>
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


class ResponseFromTWS : public EWrapperL0		//this implementation will only support up to 100 tickerId's - limited by array size
{
public:

	double m_priceAsk[100];
	double m_priceBid[100];
	double m_BidSize[100];
	double m_AskSize[100];

	

	virtual void tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute)
	{
		switch (field)
		{
		case TickType::ASK:
			m_priceAsk[tickerId] = price;
			break;
		case TickType::BID:
			m_priceBid[tickerId] = price;
			break;
		}
	}

	virtual void tickSize(TickerId tickerId, TickType field, int size)
	{
		switch (field)
		{
		case TickType::BID_SIZE:
			m_BidSize[tickerId] = double(size);
			break;
		case TickType::ASK_SIZE:
			m_AskSize[tickerId] = double(size);
			break;
		}
	}
};

void GetContractDetails(Contract & myContract, int contract) //1-EUR, 2-GBP, 3-SPY, 4-DIA, 5-IWM 
{
	//forex (GBP) works 24/7 mon-friday
	switch (contract)
	{case 1:
		myContract.symbol = "EUR";
		myContract.secType = *SecType::CASH;		//"STK"
		myContract.currency = "USD";
		myContract.exchange = *Exchange::IDEALPRO;	//"SMART";
		return;
	case 2:
		myContract.symbol = "GBP";
		myContract.secType = *SecType::CASH;		//"STK"
		myContract.currency = "USD";
		myContract.exchange = *Exchange::IDEALPRO;	//"SMART";
		//spyContract.primaryExchange = *Exchange::ARCA;	//"ARCA";
		return;
	case 3:
		myContract.symbol = "SPY";
		myContract.secType = *SecType::STK;		//"STK"
		myContract.currency = "USD";
		myContract.exchange = *Exchange::ARCA;	//"SMART";
		return;
	case 4:
		myContract.symbol = "DIA";
		myContract.secType = *SecType::STK;		//"STK"
		myContract.currency = "USD";
		myContract.exchange = *Exchange::ARCA;	//"SMART";
		return;
	case 5:
		myContract.symbol = "IWM";
		myContract.secType = *SecType::STK;		//"STK"
		myContract.currency = "USD";
		myContract.exchange = *Exchange::ARCA;	//"SMART";
		return;
	default:
		myContract.symbol = "SPY";
		myContract.secType = *SecType::STK;		//"STK"
		myContract.currency = "USD";
		myContract.exchange = *Exchange::ARCA;	//"SMART";
		return;
	}
}

int StartTWSConn()
{
	//create a function that connects to TWS	
	//create a for loop that iterates 5 times

	
	//set up the first thread:
	//this is the main way of getting data in and out of TWS - send requests by EClientL0 and receive answers by ResponseFromTWS

	//connecting to TWS:
	ResponseFromTWS	 * twsReply = new ResponseFromTWS();
	EClientL0*	pClientTWS = EClientL0::New(twsReply);
	bool bIsConnected = pClientTWS->eConnect("", 7496);
	
	Contract	myContract;
	//setting up EUR with tickerID = 1;
	GetContractDetails(myContract,1);
	int  tickerID = 1;
	pClientTWS->reqMktData(tickerID, myContract, "", false);
	//setting up GBP with tickerID = 2;
	GetContractDetails(myContract, 2);
	tickerID = 2;
	pClientTWS->reqMktData(tickerID, myContract, "", false);
	//setting up DIA with tickerID = 3;
	GetContractDetails(myContract, 3);
	tickerID = 3;
	pClientTWS->reqMktData(tickerID, myContract, "", false);
	//setting up DIA with tickerID = 4;
	GetContractDetails(myContract, 4);
	tickerID = 4;
	pClientTWS->reqMktData(tickerID, myContract, "", false);
	//setting up DIA with tickerID = 5;
	GetContractDetails(myContract, 5);
	tickerID = 5;
	pClientTWS->reqMktData(tickerID, myContract, "", false);
	using namespace std;
	ofstream out("myApiOutput.txt");
	out << endl << "Time: " << "," << "Contract: " << "," << "Bid: " << "," << "Ask: " << "," << "Bid size: " << "," << "Ask size: " << endl;

	clock_t time;
	time = clock();

	cout << endl << "How many seconds do you want to capture?" << endl;
	int duration;
	cin >> duration;

	duration = duration * 10;

	for (int i = 0; i < duration; i++)
	{
		int Wait = 100;
		Sleep(Wait);
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		cout << "sleeping for: " << Wait << " ms" << endl;
		//Contract tempContract;
		string contractName;
		int tickerId = 1;

		double ask;
		double bid;
		double bidSize;
		double askSize;

		pClientTWS->reqMktData(tickerID, myContract, "", false);

		for (int i = 1; i <= 5; i++) {		// iterate through contracts 1,2,3

			tickerID = i;
			switch (i) {
			case 1:
			//	tempContract = myContract;
				contractName = "EUR";
				break;
			case 2:
				//				tempContract = diaContract;
				contractName = "GBP";
				break;
			case 3:
				//				tempContract = iwmContract;
				contractName = "SPY";
				break;
			case 4:
				//				tempContract = iwmContract;
				contractName = "DIA";
				break;
			case 5:
				//				tempContract = iwmContract;
				contractName = "IWM";
				break;
			}

			
				ask = twsReply->m_priceAsk[tickerID];
				bid = twsReply->m_priceBid[tickerID];
				bidSize = twsReply->m_BidSize[tickerID];
				askSize = twsReply->m_AskSize[tickerID];

				time = clock();
				cout << setprecision(15);
				
				cout << endl << contractName << endl;
				cout << "ask: " << ask << endl << "bid: " << bid << endl << "ask size: " << askSize << endl << "bid size: " << bidSize << endl;

				out << setprecision(15);

				out << time << "," << contractName << "," << bid << "," << ask << "," << bidSize << "," << askSize << endl;

			
		}
		//cout << "time: " << currentTime << endl;
	}


	cout << endl << "Finished." << endl;
	
	out.close();
	waitforuserend();

	////////////////////////////////////////////////////////////////
	pClientTWS->eDisconnect();
	delete twsReply;
	////////////////////////////////////////////////////////////////

	return 0;
}
