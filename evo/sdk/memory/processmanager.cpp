#include "processmanager.hpp"
#include <string>

inline const DWORD BLOCKMAXSIZE = 409600;


int get_signature_array(const std::string& Signature, std::vector<WORD>& SignatureArray)
{
	std::string Sig = Signature;
	Sig.erase(std::remove(Sig.begin(), Sig.end(), ' '), Sig.end());

	std::size_t Size = Sig.size();

	if (Size % 2 != 0)
		return 0;

	for (int i = 0; i < Size; i += 2)
	{
		std::string ByteString = Sig.substr(i, 2);
		WORD Byte = (ByteString == "??") ? 256 : std::stoi(ByteString, nullptr, 16);
		SignatureArray.push_back(Byte);
	}
	return SignatureArray.size();
}

void get_next_array(std::vector<short>& NextArray, const std::vector<WORD>& SignatureArray)
{
	std::size_t Size = SignatureArray.size();
	for (int i = 0; i < Size; i++)
		NextArray[SignatureArray[i]] = i;
}

void search_memory_block(byte* MemoryBuffer, const std::vector<short>& NextArray, const std::vector<WORD>& SignatureArray, DWORD64 StartAddress, DWORD Size, std::vector<DWORD64>& ResultArray)
{
	if (!_proc_manager.read_memory(StartAddress, *MemoryBuffer, Size))
		return;

	int SignatureLength = SignatureArray.size();

	for (int i = 0, j, k; i < Size;)
	{
		j = i; k = 0;

		for (; k < SignatureLength && j < Size && (SignatureArray[k] == MemoryBuffer[j] || SignatureArray[k] == 256); k++, j++);

		if (k == SignatureLength)
			ResultArray.push_back(StartAddress + i);

		if ((i + SignatureLength) >= Size)
			return;

		int Num = NextArray[MemoryBuffer[i + SignatureLength]];
		if (Num == -1)
			i += (SignatureLength - NextArray[256]);
		else
			i += (SignatureLength - Num);
	}
}

std::vector<DWORD64> ProcessManager::search_memory(const std::string& Signature, DWORD64 StartAddress, DWORD64 EndAddress, int SearchNum)
{
	std::vector<DWORD64> ResultArray;
	std::vector<WORD> SignatureArray;
	std::vector<short> NextArray(260, -1);

	byte* MemoryBuffer = new byte[BLOCKMAXSIZE];

	if (get_signature_array(Signature, SignatureArray) <= 0)
		return ResultArray;

	get_next_array(NextArray, SignatureArray);

	MEMORY_BASIC_INFORMATION mbi;
	int Count;
	while (VirtualQueryEx(hProcess, reinterpret_cast<LPCVOID>(StartAddress), &mbi, sizeof(mbi)) != 0)
	{
		Count = 0;
		auto BlockSize = mbi.RegionSize;

		while (BlockSize >= BLOCKMAXSIZE)
		{
			if (ResultArray.size() >= SearchNum)
				goto END;

			search_memory_block(MemoryBuffer, NextArray, SignatureArray, StartAddress + (BLOCKMAXSIZE * Count), BLOCKMAXSIZE, ResultArray);

			BlockSize -= BLOCKMAXSIZE;
			Count++;
		}

		search_memory_block(MemoryBuffer, NextArray, SignatureArray, StartAddress + (BLOCKMAXSIZE * Count), BlockSize, ResultArray);

		StartAddress += mbi.RegionSize;

		if (ResultArray.size() >= SearchNum || EndAddress != 0 && StartAddress > EndAddress)
			break;
	}

END:

	delete[] MemoryBuffer;
	return ResultArray;
}