#include <Windows.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include "Enums.h"
#include "Generator.h"
#include "Utils.h"
#include "OffsetFinder.h"
#include "Offsets.h"
#include "Package.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

enum class EFortToastType : uint8
{
        Default                        = 0,
        Subdued                        = 1,
        Impactful                      = 2,
        EFortToastType_MAX             = 3,
};

DWORD MainThread(HMODULE Module)
{
	AllocConsole();
	FILE* Dummy;
	freopen_s(&Dummy, "CONOUT$", "w", stdout);
	freopen_s(&Dummy, "CONIN$", "r", stdin);
	
	Generator::GenerateSDK();

	/*std::cout << "Some FullName: " << ObjectArray::GetByIndex(69).GetFullName() << "\n";

	auto t_1 = high_resolution_clock::now();
	
	std::ofstream DumpStream("Properties.txt");

	DumpStream << "Properties:\n\n";

	for (auto Object : ObjectArray())
	{
		if (Object.IsA(EClassCastFlags::UProperty) && !Object.HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
		{
			DumpStream << Object.GetFullName() << "\n";
			DumpStream << Object.Cast<UEProperty>().GetCppType() << " " << Object.Cast<UEProperty>().GetValidName() << ";\n";
		}
	}
	DumpStream.close();

	auto t_2 = high_resolution_clock::now();

	
	UEClass Class = ObjectArray::FindClassFast("FortItemDefinition");
	UEEnum Enum = ObjectArray::FindObjectFast("EFortDayPhase", EClassCastFlags::UEnum).Cast<UEEnum>();
	UEFunction Func = ObjectArray::FindObjectFast("CreateTemporaryInstanceFromExistingItemBP", EClassCastFlags::UFunction).Cast<UEFunction>();

	std::cout << "Super: " << Class.GetSuper().GetFullName() << "\n\n";

	for (auto Field = Class.GetChild(); Field; Field = Field.GetNext())
	{
		std::cout << Field.GetFullName() << "\n";
	}


	Package Test(nullptr);

	Types::Class TestClass = Test.GenerateClass(Class);
	Types::Enum TestEnum = Test.GenerateEnum(Enum);

	UEStruct Super = Func.GetOuter().Cast<UEStruct>();
	Types::Function TestFunc = Test.GenerateFunction(Func, Super);

	std::cout << TestClass.GetGeneratedBody() << "\n\n";
	std::cout << TestEnum.GetGeneratedBody() << "\n\n";
	std::cout << TestFunc.GetGeneratedBody() << "\n\n";
	std::cout << TestFunc.GetParamStruct().GetGeneratedBody() << "\n\n";

	/*Types::Class TS = Test.GenerateClass(Class);

	Types::Enum tE = Test.GenerateEnum(Enum);

	UEStruct Super = Func.GetOuter().Cast<UEStruct>();
	Types::Function tF = Test.GenerateFunction(Func, Super);

	std::cout << tS.GetGeneratedBody() << "\n\n";
	std::cout << tE.GetGeneratedBody() << "\n\n";
	std::cout << tF.GetGeneratedBody() << "\n\n";
	std::cout << tF.GetParamStruct().GetGeneratedBody() << "\n\n";*/

	/*{
		auto t_1 = high_resolution_clock::now();
		auto t_2 = high_resolution_clock::now();
	
		auto ms_int_ = duration_cast<milliseconds>(t_2 - t_1);
		duration<double, std::milli> ms_double_ = t_2 - t_1;
	
		std::cout << "\n\nComparing 0 times took (" << ms_int_.count() << "ms)\n\n\n";
	}*/

	while (true)
	{
		if (GetAsyncKeyState(VK_F6) & 1)
		{
			fclose(stdout);
			fclose(Dummy);
			FreeConsole();

			FreeLibraryAndExitThread(Module, 0);
		}

		Sleep(100);
	}

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0);
		break;
	}

	return TRUE;
}