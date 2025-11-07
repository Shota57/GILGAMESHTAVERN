//ヘッダーファイル
#include <iostream>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
// Define macros マクロの定義

#define V_MAX			(25)
#define H_MAX			(80)


// Define constants 定数の定義
const int CONSOLE_WIDTH = 80;
const int CONSOLE_HEIGHT = 26;
typedef struct {
	char name[20];
	int ATK;
	int HP;
	int SPD;
}status;
status enemyParty[5] = {
		{"Goblin", 10, 30, 5},
		{"Orc", 15, 40, 4},
		{"Troll", 20, 50, 2},
		{"Elf", 12, 28, 7},
		{"Knight", 18, 35, 3}
};
const int direction_x[8] = { -1,-1,-1,0,0,1,1,1 };
const int direction_y[8] = { -1,0,1,-1,1,-1,0,1 };
void givePartyStatus(status* party, int partyNo);              // パーティのステータスを入力する関数
void inputStringToUi(int a, int b, int c);					 // UIに文字列を入力する関数
void enterMoveOn();                     // Enterキーで先に進む関数
void drawTitle();					 // タイトルを描画する関数
void drawUi();						 // UIを描画する関数
void writeSentakushi();                 // 選択肢UIを描画する関数
void saveUiToTemp();                  // UIを一時バッファに保存する関数
void resetUiFromTemp();              // 一時バッファからUIを復元する関数
void writeIntro();                            // 白文字でイントロ表示
void SetCursorVisibility(BOOL visible);		// カーソルの表示/非表示を設定する関数
void MoveCursorToTop();		                // カーソルを画面の左上に移動する関数
void clearInputBuffer();                    // キーボード入力バッファのクリア関数
bool changeConsoleSize(int x, int y);       // コンソールサイズを変更する関数
bool CL11Startup();                         // コンソールの初期化関数
bool CL11Cleanup();                         // コンソールのクリーンアップ関数
void HideConsoleTitleBar();                 // コンソールのタイトルバーを隠す関数
void ShowConsoleTitleBar();                 // コンソールのタイトルバーを表示する関数
void setConsoleFont(int width, int height, int screenWidth, int screenHeight, int dpiX, int dpiY); // コンソールフォントの設定関数
void centerConsoleWindow(int screenWidth, int screenHeight);    // コンソールウィンドウを中央に配置する関数
void EnableVTMode();						// VTシーケンスモードを有効化
void SetTextColorRGB(int r, int g, int b);	//コンソールウィンドウの表示色をRGB形式で変更
void SetBackgroundColorRGB(int r, int g, int b);	//コンソールウィンドウの背景色をRGB形式で変更
bool isValidPosition(int x, int y);		// 指定座標が有効かどうか判定する関数

char temp[V_MAX][H_MAX + 1]; // UI描画用の一時バッファ
char ui[V_MAX][H_MAX + 1] =
//   0         1         2         3         4         5         6         7         8
{//  01234567890123456789012345678901234567890123456789012345678901234567890123456789
	"********************************************************************************",//0
	"*                                                                              *",//1
	"*                                                                              *",//2
	"*                                                                              *",//3
	"*                                                                              *",//4
	"*                                                                              *",//5
	"*                                                                              *",//6
	"*                                                                              *",//7
	"*                                                                              *",//8
	"*                                                                              *",//9
	"*                                                                              *",//0
	"*                                                                              *",//1
	"*                                                                              *",//2
	"*                                                                              *",//3
	"*                                                                              *",//4
	"*                                                                              *",//5
	"*                                                                              *",//6
	"*                                                                              *",//7
	"********************************************************************************",//8
	"*                                                                              *",//9
	"*                                                                              *",//0
	"*                                                                              *",//1
	"*                                                                              *",//2
	"*                                                                              *",//3
	"********************************************************************************",//4
};

char title[20][80] = {
	"==============================================================================\0",
	" _______  ___   ___      _______  _______  __   __  _______  _______  __   __ \0",
	"|       ||   | |   |    |       ||   _   ||  |_|  ||       ||       ||  | |  |\0",
	"|    ___||   | |   |    |    ___||  |_|  ||       ||    ___||  _____||  |_|  |\0",
	"|   | __ |   | |   |    |   | __ |       ||       ||   |___ | |_____ |       |\0",
	"|   ||  ||   | |   |___ |   ||  ||       ||       ||    ___||_____  ||       |\0",
	"|   |_| ||   | |       ||   |_| ||   _   || ||_|| ||   |___  _____| ||   _   |\0",
	"|_______||___| |_______||_______||__| |__||_|   |_||_______||_______||__| |__|\0",
	"             _______  _______  __   __  _______  ______    __    _            \0",
	"            |       ||   _   ||  | |  ||       ||    _ |  |  |  | |           \0",
	"            |_     _||  |_|  ||  |_|  ||    ___||   | ||  |   |_| |           \0",
	"              |   |  |       ||       ||   |___ |   |_||_ |       |           \0",
	"              |   |  |       ||       ||    ___||    __  ||  _    |           \0",
	"              |   |  |   _   | |     | |   |___ |   |  | || | |   |           \0",
	"              |___|  |__| |__|  |___|  |_______||___|  |_||_|  |__|           \0",
	"                                                                              \0",
	"==============================================================================\0",
	"                      - G I L G A M E S H ' S   T A V E R N -                 \0",
	"==============================================================================\0",
	"                           > Press Enter To Move On <                         \0"
};
char sentakushi[2][15] = {
	" 1. Start Game",
	" 2. To Title"
};
status parties[5][5];


int main() {
	if (!CL11Startup()) {
		perror("CL11Startup error");
		return 0;
	}
	SetCursorVisibility(FALSE);
	drawTitle();
	rewind(stdin);(void)getchar();
	writeIntro();
	enterMoveOn();
	resetUiFromTemp();
	drawUi();
	
	for (int p = 0; p < 5; ++p) {
		givePartyStatus(parties[p], p);
		resetUiFromTemp();
	}

	for (int p = 0; p < 5; ++p) {
		std::cout << "\nParty No." << (p + 1) << "PlayerNo.1(" << parties[p][0].name << ")vs(" << enemyParty[0].name << "\n";
		int hp1 = parties[p][0].HP;
		int hp2 = enemyParty[0].HP;
		while (hp1 > 0 && hp2 > 0) {
			hp2 -= parties[p][0].ATK;
			if (hp2 <= 0) break;
			hp1 -= enemyParty[0].ATK;
		}
		if (hp1 > 0) {
			std::cout << parties[p][0].name << "Wins!!\n";
			enterMoveOn();
		}
		else {
			std::cout << enemyParty[0].name << "Wins!!\n";
		}
	}
	
	

	system("cls");
	return 0;
}
void givePartyStatus(status* party, int partyNo) {
	// メッセージを作成
	
	char message[][80]{
		"Please input party No.\0",
		"'s status\0",
		"Member Name : \0",
		"ATK:\0",
		"HP:\0",
		"SPD:\0"
	};

	MoveCursorToTop();
	char* tempPtr = &ui[19][1];
	for (int i = 0; i < strlen(message[0]); ++i) {
		tempPtr = &ui[19][1 + i];
		*tempPtr = message[0][i];
		drawUi();
		Sleep(5);
	}
	tempPtr = &ui[19][strlen(message[0]) + 1];
	*tempPtr = '0' + (partyNo + 1);
	drawUi();

	for (int i = 0; i < strlen(message[1]); ++i) {
		tempPtr = &ui[19][strlen(message[0]) + 2 + i];
		*tempPtr = message[1][i];
		drawUi();
		Sleep(5);
	}
	enterMoveOn();
	resetUiFromTemp();
	for (int i = 2; i < 6; ++i) {
		for (int j = 0; j < strlen(message[i]); ++j) {
			tempPtr = &ui[18 + i][1 + j];
			*tempPtr = message[i][j];
			drawUi();
			Sleep(5);
		}
		inputStringToUi(i, strlen(message[i]), partyNo);
	}
	enterMoveOn();

	
	// 各メンバーの入力
	/*for (int m = 0; m < 5; ++m) {
		std::cout << "\n  Member" << (m + 1) << " Name : ";
		std::cin >> party[m].name;
		std::cout << "    ATK: ";
		std::cin >> party[m].ATK;
		std::cout << "    HP: ";
		std::cin >> party[m].HP;
		std::cout << "    SPD: ";
		std::cin >> party[m].SPD;
	}*/
}

void inputStringToUi(int a, int b, int c) {
	char input[H_MAX - 2 + 1] = { 0 }; 
	int pos = 0;
	char* ptr = &ui[18+a][1+b];
	drawUi();
	MoveCursorToTop();


	while (true) {
		char ch = _getch();
		int i = 0;
		if (ch == '\r') {
			++i;
			break;
		}
		else if (ch == 0x08) { 
			if (pos > 0) {
				if (i > 0) {
					--i;
				}
				--pos;
				input[pos] = '\0';
				ptr[pos] = ' ';
				drawUi();
				MoveCursorToTop();
			}
		}
		else if (ch >= 32 && ch <= 126 && pos < H_MAX - 2 && i == 0) {
			input[pos] = ch;
			ptr[pos] = ch;
			++pos;
			parties[c][a - 2].name[pos - 1] = ch;
			drawUi();
			MoveCursorToTop();
		}
		else if (ch >= 0x31 && ch <= 0x39 && pos < H_MAX - 2 && i == 1) {
			input[pos] = ch;
			ptr[pos] = ch;
			++pos;
			parties[c][a - 2].ATK= (int)ch;
			++i;
			drawUi();
			MoveCursorToTop();
		}
		else if (ch >= 0x31 && ch <= 0x39 && pos < H_MAX - 2 && i == 2) {
			input[pos] = ch;
			ptr[pos] = ch;
			++pos;
			parties[c][a - 2].ATK = (parties[c][a - 2].ATK) * 10;
			parties[c][a - 2].ATK = parties[c][a - 2].ATK + (int)ch;
			++i;
			drawUi();
			MoveCursorToTop();
		}
		else if (ch >= 0x31 && ch <= 0x39 && pos < H_MAX - 2 && i == 3) {
			drawUi();
			MoveCursorToTop();
		}
		else if (ch >= 0x31 && ch <= 0x39 && pos < H_MAX - 2 && i == 4) {
			input[pos] = ch;
			ptr[pos] = ch;
			++pos;
			parties[c][a - 2].HP = (int)ch;
			++i;
			drawUi();
			MoveCursorToTop();
		}
		else if (ch >= 0x31 && ch <= 0x39 && pos < H_MAX - 2 && i == 5) {
			input[pos] = ch;
			ptr[pos] = ch;
			++pos;
			parties[c][a - 2].HP = (parties[c][a - 2].HP) * 10;
			parties[c][a - 2].HP = parties[c][a - 2].HP + (int)ch;
			++i;
			drawUi();
			MoveCursorToTop();
		}
		else if (ch >= 0x31 && ch <= 0x39 && pos < H_MAX - 2 && i == 6) {
			drawUi();
			MoveCursorToTop();
		}
		else if (ch >= 0x31 && ch <= 0x39 && pos < H_MAX - 2 && i == 7) {
			input[pos] = ch;
			ptr[pos] = ch;
			++pos;
			parties[c][a - 2].SPD = (int)ch;
			++i;
			drawUi();
			MoveCursorToTop();
		}
		else if (ch >= 0x31 && ch <= 0x39 && pos < H_MAX - 2 && i == 8) {
			input[pos] = ch;
			ptr[pos] = ch;
			++pos;
			parties[c][a - 2].SPD = (parties[c][a - 2].SPD) * 10;
			parties[c][a - 2].SPD = parties[c][a - 2].SPD + (int)ch;
			++i;
			drawUi();
			MoveCursorToTop();
		}
		else if (ch >= 0x31 && ch <= 0x39 && pos < H_MAX - 2 && i == 9) {
			drawUi();
			MoveCursorToTop();
		}
		
	}
}
void drawTitle() {
	MoveCursorToTop();
	for (int i = 0; i < 20; ++i) {
		std::cout << title[i] << std::endl;
		Sleep(50);
	}
}

void resetUiFromTemp() {
	for (int i = 0; i < V_MAX; ++i)
		strcpy_s(ui[i], temp[i]);
}

void writeSentakushi() {
	saveUiToTemp();
	MoveCursorToTop();
	char* tempPtr = &ui[20][30];
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < strlen(sentakushi[i]); ++j) {
			tempPtr = &ui[20 + i][30 + j];
			*tempPtr = sentakushi[i][j];
			drawUi();
			Sleep(5);
		}
	}
}

void saveUiToTemp() {
	for (int i = 0; i < V_MAX; ++i)
		strcpy_s(temp[i], ui[i]);
}

void enterMoveOn() {
	MoveCursorToTop();
	char text[] = ">Press Enter To Move On<";
	char* tempPtr = &ui[23][55];
	for (int i = 0; i < strlen(text); ++i) {
		tempPtr = &ui[23][55 + i];
		*tempPtr = text[i];
		drawUi();
		Sleep(5);
	}
	rewind(stdin);
	(void)getchar();
}
void writeIntro() {
	saveUiToTemp();
	MoveCursorToTop();
	char* tempPtr = &ui[0][0];
	char introtext[][80] {
		"Welcome to Gilgamesh's Tavern!",
		"Adventurers from all over gather to share tales of their journeys here.",
		"Enjoy a hearty meal and a refreshing drink as you unwind from your quests.",
		"May your stay be filled with camaraderie and unforgettable stories!",
	};

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < strlen(introtext[i]); ++j) {
			tempPtr = &ui[19 + i][1 + j];
			*tempPtr = introtext[i][j];
			drawUi();
			Sleep(5);
		}
	}
}

bool isValidPosition(int x, int y) {
	return x >= 0 && x < H_MAX && y >= 0 && y < V_MAX;
}

void drawUi() {
	MoveCursorToTop();
	for (int i = 0; i < V_MAX; ++i)
		std::cout << ui[i] << std::endl;
}

void SetCursorVisibility(BOOL visible) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// カーソルを先頭に戻す
void MoveCursorToTop() {
	COORD coord = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// コンソールの初期化
bool CL11Startup() {
	// Change console size コンソールサイズの変更
	if (!changeConsoleSize(CONSOLE_WIDTH, CONSOLE_HEIGHT)) {
		return FALSE;
	}
	// コンソールのタイトルバーを隠す
	HideConsoleTitleBar();

	// 画面の幅と高さを取得
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	HDC screen = GetDC(0);
	int dpiX = GetDeviceCaps(screen, LOGPIXELSX);
	int dpiY = GetDeviceCaps(screen, LOGPIXELSY);
	ReleaseDC(0, screen);

	// コンソールフォントの設定
	setConsoleFont(CONSOLE_WIDTH, CONSOLE_HEIGHT, screenWidth, screenHeight, dpiX, dpiY);
	// Center the console window コンソールウィンドウを中央に配置
	centerConsoleWindow(screenWidth, screenHeight);
	return TRUE;
}

// コンソールフォントの設定
void setConsoleFont(int width, int height, int screenWidth, int screenHeight, int dpiX, int dpiY) {
	int fontSizeX = (screenWidth / width) * (dpiX / 96);
	int fontSizeY = (screenHeight / height) * (dpiY / 96);

	// Set font information フォント情報の設定
	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX), 0, {0, 0}, FF_DONTCARE, FW_NORMAL, L"MS Gothic" };
	DISPLAY_DEVICE dd = { sizeof(dd) };
	EnumDisplayDevices(NULL, 0, &dd, 0);
	DEVMODE dm = { sizeof(dm) };
	EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm);

	// フォントサイズの設定
	if (dm.dmPelsHeight > dm.dmPelsWidth) {
		cfi.dwFontSize.X = static_cast<SHORT>(fontSizeX);
		cfi.dwFontSize.Y = static_cast<SHORT>(floor(fontSizeX * 2));
	}
	else {
		cfi.dwFontSize.X = static_cast<SHORT>(floor(fontSizeY / 2));
		cfi.dwFontSize.Y = static_cast<SHORT>(fontSizeY);
	}

	// コンソールフォントの適用
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
	SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width) - 1, static_cast<SHORT>(height) - 1 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
}

// コンソールウィンドウを中央に配置する関数
void centerConsoleWindow(int screenWidth, int screenHeight) {
	HWND consoleWindow = GetConsoleWindow();
	RECT rect;
	GetWindowRect(consoleWindow, &rect);
	MoveWindow(consoleWindow, (screenWidth - (rect.right - rect.left)) / 2, (screenHeight - (rect.bottom - rect.top)) / 2, rect.right - rect.left, rect.bottom - rect.top, TRUE);
}

// コンソールのクリーンアップ関数
bool CL11Cleanup() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

	// Set console mode コンソールモードの設定
	SetConsoleMode(hOut, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
	SetConsoleMode(hIn, ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_MOUSE_INPUT);

	// Set console buffer size コンソールバッファサイズの設定
	COORD bufferSize = { CONSOLE_WIDTH, CONSOLE_HEIGHT };
	SetConsoleScreenBufferSize(hOut, bufferSize);
	CONSOLE_CURSOR_INFO cursorInfo = { 25, TRUE };
	SetConsoleCursorInfo(hOut, &cursorInfo);
	SMALL_RECT windowSize = { 0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1 };
	SetConsoleWindowInfo(hOut, TRUE, &windowSize);

	// Set font information フォント情報の設定
	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX), 0, {0, 16}, FF_DONTCARE, FW_NORMAL, L"Consolas" };
	SetCurrentConsoleFontEx(hOut, FALSE, &cfi);

	// Reset console window position コンソールウィンドウの位置をリセット
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	ShowConsoleTitleBar();

	return TRUE;
}

// コンソールサイズを変更する関数
bool changeConsoleSize(int x, int y) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SMALL_RECT rectConsoleSize = { 0, 0, 1, 1 };

	// Temporarily reduce console window size コンソールウィンドウサイズの一時的な縮小
	SetConsoleWindowInfo(hStdout, TRUE, &rectConsoleSize);
	if (!SetConsoleScreenBufferSize(hStdout, coord)) {
		return FALSE;
	}
	rectConsoleSize.Right = x - 1;
	rectConsoleSize.Bottom = y - 1;
	SetConsoleWindowInfo(hStdout, TRUE, &rectConsoleSize);

	return TRUE;
}

// コンソールのタイトルバーを隠す関数
void HideConsoleTitleBar() {
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) {
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		style &= ~WS_CAPTION;
		SetWindowLong(hwnd, GWL_STYLE, style);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

// コンソールのタイトルバーを表示する関数
void ShowConsoleTitleBar() {
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) {
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		style |= WS_CAPTION;
		SetWindowLong(hwnd, GWL_STYLE, style);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

// キーボード入力バッファのクリア関数
void clearInputBuffer() {
	while (_kbhit()) {
		rewind(stdin);
		(void)getchar();
	}
}

// VTシーケンスモードを有効化
void EnableVTMode() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}

// コンソールウィンドウの表示色をRGB形式で変更
void SetTextColorRGB(int r, int g, int b) {
	printf("\x1b[38;2;%d;%d;%dm", r, g, b);
}

// コンソールウィンドウの背景色をRGB形式で変更
void SetBackgroundColorRGB(int r, int g, int b) {
	printf("\x1b[48;2;%d;%d;%dm", r, g, b);
}