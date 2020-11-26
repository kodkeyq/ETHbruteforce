#include "coins64f.h"

uint64_t* hashmap;
uint64_t hashmap_shift = 25u;
uint64_t hashmap_size = (uint64_t)1u << hashmap_shift;
uint64_t hashmap_mask = hashmap_size - 1u;
const uint64_t hashmap_maxsize = 268435456;

string filename = "list/list_address";
uint64_t filecount = 35;

uint64_t Add(string line)
{
	bool hexdigit = true;
	uint64_t result = 0;
	for (uint64_t i = 2; i < line.length(); i++)
	{
		if (!isxdigit(line[i]))
		{
			hexdigit = false;
			break;
		}
	}
	if (hexdigit)
	{
		uint64_t buff[3];
		memset(buff, 0, sizeof(buff));
		string str;

		str.assign(&line[2], 8);
		buff[2] = ((uint64_t)std::stoul(str, nullptr, 16) & 0x00000000FFFFFFFF);
		str.assign(&line[18], 8);
		buff[1] = ((uint64_t)std::stoul(str, nullptr, 16) & 0x00000000FFFFFFFF);
		str.assign(&line[10], 8);
		buff[1] |= (((uint64_t)std::stoul(str, nullptr, 16) << 32) & 0xFFFFFFFF00000000);
		str.assign(&line[34], 8);
		buff[0] = ((uint64_t)std::stoul(str, nullptr, 16) & 0x00000000FFFFFFFF);
		str.assign(&line[26], 8);
		buff[0] |= (((uint64_t)std::stoul(str, nullptr, 16) << 32) & 0xFFFFFFFF00000000);

		if ((buff[0] != 0) || (buff[1] != 0) || (buff[2] != 0))
		{
			uint64_t index = buff[0] & hashmap_mask;
			if ((hashmap[3 * index] == 0) && (hashmap[3 * index + 1] == 0) && (hashmap[3 * index + 2] == 0))
			{
				memcpy(&hashmap[3 * index], buff, sizeof(buff));
				result++;
			}
		}
	}

	return result;
}

uint64_t LoadFiles()
{
	uint64_t totalitems = 0;
	if ((filename != "") && (filecount > 0))
	{
		for (uint64_t i = 0; i < filecount; i++)
		{
			cout << "Trying file " + filename + to_string(i) + ".txt...";
			ifstream File(filename + to_string(i) + ".txt");
			if (File)
			{
				string line;
				uint64_t items = 0;
				while (getline(File, line))
				{
					items += Add(line);
				}
				cout << "...Success!" << dec << items << " new items!" << endl;
				totalitems += items;
			}
			else
			{
				cout << "...No result!" << endl;
			}
		}
	}
	return totalitems;
}

void HashMap()
{
	if ((hashmap_size != 0) && (hashmap_size <= hashmap_maxsize))
	{
		cout << "Hashmap size:" << dec << hashmap_size << endl;
		cout << "Hashmap mask:" << hex << hashmap_mask << endl;
		hashmap = new uint64_t[hashmap_size * 3];
		memset(hashmap, 0, sizeof(uint64_t) * hashmap_size * 3);

		uint64_t totalitems = LoadFiles();
		if (totalitems > 0)
		{
			cout << "Total " << dec << totalitems << " items" << endl;
		}
		else
		{
			cout << "No data!" << endl;
		}
	}

	return;
}

void DeleteHashMap()
{
	delete[]hashmap;

	return;
}




