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
	double m_priceBid;
	double m_BidSize;
	double m_AskSize;

	

	virtual void tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute)
	{
		switch (field)
		{
		case TickType::ASK:
			m_priceAsk[tickerId] = price;
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

void GetContractDetails(Contract & spyContract, int contract) //1-EUR, 2-GBP, 3-SPY, 4-DIA, 5-IWM 
{

	// use either set to get what you want. both work
	//forex (GBP) works 24/7 mon-friday
	switch (contract)
	{case 1:
		spyContract.symbol = "EUR";
		spyContract.secType = *SecType::CASH;		//"STK"
		spyContract.currency = "USD";
		spyContract.exchange = *Exchange::IDEALPRO;	//"SMART";
		return;
	case 2:
		spyContract.symbol	= "SPY";
		spyContract.secType	= *SecType::STK;		//"STK"
		spyContract.currency	= "USD";
		spyContract.exchange	= *Exchange::ARCA;	//"SMART";
		//spyContract.primaryExchange = *Exchange::ARCA;	//"ARCA";
		return;
	case 2:
		spyContract.symbol = "SPY";
		spyContract.secType = *SecType::STK;		//"STK"
		spyContract.currency = "USD";
		spyContract.exchange = *Exchange::ARCA;	//"SMART";
				
		return;
	
	}
	spyContract.symbol = "EUR";
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
	//create a for loop that iterates 5 times

	
	//set up the first thread:
	//this is the main way of getting data in and out of TWS - send requests by EClientL0 and receive answers by ResponseFromTWS

	//first twsReply will be SPY
	ResponseFromTWS	 * twsReply = new ResponseFromTWS();
	EClientL0*	pClientTWS = EClientL0::New(twsReply);
	bool bIsConnected = pClientTWS->eConnect("", 7496);
	
	////second, DIAReply and pClientDIA will be DIA
	//ResponseFromTWS	 * DIAReply = new ResponseFromTWS();
	//EClientL0*	pClientDIA = EClientL0::New(DIAReply);
	////bIsConnected = pClientDIA->eConnect("", 7496);
	//pClientDIA->eConnect("", 7496);

	////third, IWMReply and pClientIWM will be IWM
	//ResponseFromTWS	 * IWMReply = new ResponseFromTWS();
	//EClientL0*	pClientIWM = EClientL0::New(IWMReply);
	//pClientIWM->eConnect("", 7496);

	//the below works without launching it in a thread:
	//setting up SPY with tickerId = 1;
	Contract	spyContract;
	GetContractDetails(spyContract);
	int  tickerID = 1;
	pClientTWS->reqMktData(tickerID, spyContract, "", false);

	//setting up DIA with tickerId = 2;
	Contract	diaContract;
	//spyContract.symbol = "GBP";
	//spyContract.secType = *SecType::CASH;		//"STK"
	//spyContract.currency = "USD";
	//spyContract.exchange = *Exchange::IDEALPRO;	//"SMART";

	diaContract.symbol	= "GBP";
	diaContract.secType	= *SecType::CASH;		//"STK"
	diaContract.currency	= "USD";
	diaContract.exchange	= *Exchange::IDEALPRO;	//"SMART";

	//diaContract.symbol = "DIA";
	//diaContract.secType = *SecType::STK;		//"STK"
	//diaContract.currency = "USD";
	//diaContract.exchange = *Exchange::ARCA;	//"SMART";
	tickerID = 2;
	//pClientDIA->reqMktData(tickerID, diaContract, "", false);		//commented out?

	pClientTWS->reqMktData(tickerID, diaContract, "", false);


	//setting up IWM with tickerId = 3;
	Contract	iwmContract;
	iwmContract.symbol = "DIA";
	iwmContract.secType = *SecType::STK;		//"STK"
	iwmContract.currency = "USD";
	iwmContract.exchange = *Exchange::ARCA;	//"SMART";
	tickerID = 3;
	//pClientIWM->reqMktData(tickerID, iwmContract, "", false);		//commented out?


	using namespace std;
	ofstream out("output.out");

	clock_t time;
	time = clock();

	for (int i = 0; i < 10; i++)
	{
		int Wait = 250;
		Sleep(Wait);
		cout << "sleeping for: " << Wait << " ms" << endl;
		Contract tempContract;
		string contractName;
		int tickerId = 1;

		double ask;
		double bid;
		double bidSize;
		double askSize;


		for (int i = 1; i < 4; i++) {		// iterate through contracts 1,2,3

			tickerID = i;
			switch (i) {
			case 1:
				tempContract = spyContract;
				contractName = "SPY";
				pClientTWS->reqMktData(tickerID, spyContract, "", false);
				ask = twsReply->m_priceAsk[tickerID];
				bid = twsReply->m_priceBid;
				bidSize = twsReply->m_BidSize;
				askSize = twsReply->m_AskSize;

				time = clock();
				cout << setprecision(15);

				cout << endl << contractName << endl;
				cout << "ask: " << ask << endl << "bid: " << bid << endl << "ask size: " << askSize << endl << "bid size: " << bidSize << endl;

				out << setprecision(15);

				out << "time: " << time << endl;
				out << endl << contractName << endl;
				out << "ask: " << ask << endl << "bid: " << bid << endl << "ask size: " << askSize << endl << "bid size: " << bidSize << endl;

				break;
			case 2:
				tempContract = diaContract;
				contractName = "DIA";

				//pClientTWS->reqMktData(tickerID, diaContract, "", false);
				ask = twsReply->m_secondBid;



				//pClientDIA->reqMktData(tickerID, diaContract, "", false);	//CHANGED
				//ask = DIAReply->m_priceAsk;							//CHANGED
				//bid = DIAReply->m_priceBid;
				//bidSize = DIAReply->m_BidSize;
				//askSize = DIAReply->m_AskSize;						//CHANGED UP TO HERE

				time = clock();
				cout << setprecision(15);

				cout << endl << contractName << endl;
				cout << "ask: " << ask << endl;

				out << setprecision(15);

				out << "time: " << time << endl;
				out << endl << contractName << endl;
				out << "ask: " << ask << endl;

				break;
			case 3:
				tempContract = iwmContract;
				contractName = "IWM";
				//pClientIWM->reqMktData(tickerID, iwmContract, "", false);	//CHANGED
				//ask = IWMReply->m_priceAsk;							//CHANGED
				//bid = IWMReply->m_priceBid;
				//bidSize = IWMReply->m_BidSize;
				//askSize = IWMReply->m_AskSize;						//CHANGED UP TO HERE

				//time = clock();
				//cout << setprecision(15);

				//cout << endl << contractName << endl;
				//cout << "ask: " << ask << endl << "bid: " << bid << endl << "ask size: " << askSize << endl << "bid size: " << bidSize << endl;

				//out << setprecision(15);

				//out << "time: " << time << endl;
				//out << endl << contractName << endl;
				//out << "ask: " << ask << endl << "bid: " << bid << endl << "ask size: " << askSize << endl << "bid size: " << bidSize << endl;

				break;
			}
			
			//pClientTWS->reqMktData(tickerID, spyContract, "", false);
			//	double ask = twsReply->m_priceAsk;
			//	double bid = twsReply->m_priceBid;
			//	double bidSize = twsReply->m_BidSize;
			//	double askSize = twsReply->m_AskSize;

			//	time = clock();
			//	cout << setprecision(15);

			//	cout << endl << contractName << endl;
			//	cout << "ask: " << ask << endl << "bid: " << bid << endl << "ask size: " << askSize << endl << "bid size: " << bidSize << endl;

			//	out << setprecision(15);

			//	out << "time: " << time << endl;
			//	out << endl << contractName << endl;
			//	out << "ask: " << ask << endl << "bid: " << bid << endl << "ask size: " << askSize << endl << "bid size: " << bidSize << endl;
			
		}
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
