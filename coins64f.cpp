// coins64f.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "coins64f.h"

extern uint64_t hashmap_mask;
extern uint64_t* hashmap;

uint64_t total = 0;
mutex e_lock;

void Process(uint64_t* key, uint64_t* inc, uint64_t len)
{
	Secp256k1 Curve;
	Curve.SetPrivateKey(key);
	Curve.SetIncrement(inc);

	uint64_t k[4];
	uint64_t Px[4], Py[4];
	uint64_t adr[3];
	uint64_t e_adr[3];
	
	uint64_t pretotal = 0;

	for (uint64_t i = 1; i < len; i++)
	{
		Curve.Get(k, Px, Py);
		
		keccak1600m(Px, Py, adr);

		e_lock.lock();
		//----------------WORK WITH HASHMAP--------------

		uint64_t index = adr[0] & hashmap_mask;
		memcpy(e_adr, &hashmap[3 * index], sizeof(e_adr));
		if ((e_adr[0] == adr[0]) && (e_adr[1] == adr[1]) && (e_adr[2] == e_adr[2]))
		{
			cout << this_thread::get_id() << ":";
			cout << "HIJACKED:";
			printkp(k, adr);
		}
		if (pretotal > 10000000)
		{
			total += pretotal;
			cout << "TOTAL SCANNED:" << dec << total << endl;
			pretotal = 0;
			cout << this_thread::get_id() << ":";
			cout << "CHECKIFOK:";
			printkp(k, adr);
		}
		//-----------------------------------------------
		e_lock.unlock();
		pretotal++;

		Curve.Increment();
	}	
}

void Process2(uint64_t* key, uint64_t* inc, uint64_t len, uint64_t* EQUAL, uint64_t mask, uint64_t EQUALSIZE)
{
	Secp256k1 Curve;
	Curve.SetPrivateKey(key);
	Curve.SetIncrement(inc);

	uint64_t k[4];
	uint64_t Px[4], Py[4];
	uint64_t adr[3];
	uint64_t e_adr[3];

	//uint64_t pretotal = 0;

	for (uint64_t i = 1; i < len; i++)
	{
		Curve.Get(k, Px, Py);

		keccak1600m(Px, Py, adr);
		for (int j = 0; j < EQUALSIZE; j++)
		{
			if ((adr[0] & mask) == EQUAL[j])
			{
				e_lock.lock();
				//----------------WORK WITH HASHMAP--------------
				cout << this_thread::get_id() << ":";
				cout << "HIJACKED:";
				printkp(k, adr);
				/*	uint64_t index = adr[0] & hashmap_mask;
					memcpy(e_adr, &hashmap[3 * index], sizeof(e_adr));
					if ((e_adr[0] == adr[0]) && (e_adr[1] == adr[1]) && (e_adr[2] == e_adr[2]))
					{
						cout << this_thread::get_id() << ":";
						cout << "HIJACKED:";
						printkp(k, adr);
					}
					if (pretotal > 10000000)
					{
						total += pretotal;
						cout << "TOTAL SCANNED:" << dec << total << endl;
						pretotal = 0;
						cout << this_thread::get_id() << ":";
						cout << "CHECKIFOK:";
						printkp(k, adr);
					}*/
					//-----------------------------------------------
				e_lock.unlock();
			}
		}
		//pretotal++;

		Curve.Increment();
	}
}


int main()
{
	//HashMap();

	system("pause");
	uint64_t s = 0;

	uint64_t fixedsted = 4294967296u/7u;
	uint64_t k[4] = { 0x0,0x0,0x0,0x1 };
	uint64_t k2[4] = { 0x0,0x0,0x0,fixedsted};
	uint64_t k3[4] = { 0x0,0x0,0x0,2* fixedsted };
	uint64_t k4[4] = { 0x0,0x0,0x0,3* fixedsted};
	uint64_t k5[4] = { 0x0,0x0,0x0,4* fixedsted };
	uint64_t k6[4] = { 0x0,0x0,0x0,5* fixedsted };
	uint64_t i[4] = { 0x0,0x0,0x0,0x1};
	//uint64_t eq = 0x000000001111DEAD;
	uint64_t eqsize = 10;
	uint64_t eq[10] =
	{
		0x000000001111DEAD,
		0x000000002222DEAD,
		0x000000003333DEAD,
		0x000000004444DEAD,
		0x000000005555DEAD,
		0x000000006666DEAD,
		0x000000007777DEAD,
		0x000000008888DEAD,
		0x000000009999DEAD,
		0x00000000AAAADEAD,
	};
	uint64_t mask = 0x00000000FFFFFFFF;
		thread thr1(Process2, k, i, fixedsted,eq,mask,eqsize);
		thread thr2(Process2, k2, i, fixedsted, eq, mask, eqsize);
		thread thr3(Process2, k3, i, fixedsted, eq, mask, eqsize);
		thread thr4(Process2, k4, i, fixedsted, eq, mask, eqsize);
		thread thr5(Process2, k5, i, fixedsted, eq, mask, eqsize);
		thread thr6(Process2, k6, i, fixedsted,eq, mask, eqsize);
		thr1.join();
		thr2.join();
		thr3.join();
		thr4.join();
		thr5.join();
		thr6.join();
		cout << "We are done now" << dec << s << endl;
	system("pause");
	system("pause");
	system("pause");
	system("pause");

	//DeleteHashMap();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
