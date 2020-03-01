using namespace std;
#include <cstdlib>
#include <ctime>
#include <windows.h>

int AppTitleNum = 0;
char TempTitle[100];

void PrintGreetings(char AppTitle[]) {
	memset(TempTitle, ' ', sizeof(TempTitle));
	srand(int(time(0)));
	for (AppTitleNum = 0; AppTitleNum < strlen(AppTitle); AppTitleNum++) {
		for (int TempTitleIdx = AppTitleNum + 1; TempTitleIdx < strlen(AppTitle); TempTitleIdx++) {
			if (AppTitle[TempTitleIdx] != ' ')TempTitle[TempTitleIdx] = rand() % 94 + 33;
		}
		TempTitle[AppTitleNum] = AppTitle[AppTitleNum];
		system("cls");
		cout << TempTitle << endl;
		Sleep(100);
	}
}
