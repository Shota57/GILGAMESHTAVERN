#include <iostream>
#include <fstream>
#include <string>

// 定数定義：最大問題数と選択肢数
#define MAX_QUESTIONS 5
#define MAX_CHOICES 3

// クイズ構造体：問題文と選択肢を格納
struct Quiz {
    std::string question;
    std::string choices[MAX_CHOICES];
};

// 関数プロトタイプ宣言
int loadQuizFile(const char* filename, Quiz quizArray[], int maxQuestions);
bool loadAnswers(const char* filename, int answers[], int numQuestions);
void askQuestions(Quiz quizArray[], int userAnswers[], int numQuestions);
void saveResults(const char* filename, Quiz quizArray[], int userAnswers[], int correctAnswers[], int numQuestions);

int main() {
    Quiz quizArray[MAX_QUESTIONS];         // クイズ配列
    Quiz* quizPtr = quizArray;             // ポインタでアクセス
    int correctAnswers[MAX_QUESTIONS];     // 正解インデックス
    int userAnswers[MAX_QUESTIONS];        // ユーザーの回答

    // クイズファイル読み込み
    int numLoaded = loadQuizFile("quiz.txt", quizPtr, MAX_QUESTIONS);
    if (numLoaded == 0) {
        std::cerr << "エラー: クイズファイルの読み込みに失敗しました。\n";
        return 1;
    }

    // 正解ファイル読み込み
    if (!loadAnswers("answers.txt", correctAnswers, numLoaded)) {
        std::cerr << "エラー: 正解ファイルの読み込みに失敗しました。\n";
        return 1;
    }

    // 質問を表示して回答を取得
    askQuestions(quizPtr, userAnswers, numLoaded);

    // 結果を保存
    saveResults("results.txt", quizPtr, userAnswers, correctAnswers, numLoaded);

    std::cout << "結果をresults.txtに保存しました。\n";
    return 0;
}

// クイズファイルを読み込む関数
int loadQuizFile(const char* filename, Quiz quizArray[], int maxQuestions) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "エラー: ファイルを開けません: " << filename << "\n";
        return 0;
    }

    std::string line;
    int count = 0;

    while (count < maxQuestions) {
        // 問題文の読み込み（コメント行と空行はスキップ）
        do {
            if (!std::getline(file, line)) {
                std::cerr << "警告: ファイルの終端に到達しました。読み込んだ問題数: " << count << "\n";
                file.close();
                return count;
            }
        } while (line.empty() || line[0] == '#');

        // 問題文が空でないかチェック
        if (line.empty()) {
            std::cerr << "エラー: 問題文が空です（問題 " << (count + 1) << "）\n";
            file.close();
            return count;
        }

        quizArray[count].question = line;

        // 選択肢3つを読み込む
        for (int i = 0; i < MAX_CHOICES; ++i) {
            if (!std::getline(file, line)) {
                std::cerr << "エラー: 選択肢の読み込みに失敗しました（問題 " << (count + 1) << "、選択肢 " << (i + 1) << "）\n";
                file.close();
                return count;
            }

            // 空の選択肢がある場合は警告
            if (line.empty()) {
                std::cerr << "警告: 選択肢 " << (i + 1) << " が空です（問題 " << (count + 1) << "）\n";
            }

            quizArray[count].choices[i] = line;
        }

        ++count;
    }

    file.close();
    return count;
}

// 正解ファイルを読み込む関数
bool loadAnswers(const char* filename, int answers[], int numQuestions) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "エラー: 正解ファイルを開けません: " << filename << "\n";
        return false;
    }

    for (int i = 0; i < numQuestions; ++i) {
        if (!(file >> answers[i])) {
            std::cerr << "エラー: 正解インデックスの読み込みに失敗しました（問題 " << (i + 1) << "）\n";
            file.close();
            return false;
        }

        // インデックスが0?2の範囲外ならエラー
        if (answers[i] < 0 || answers[i] >= MAX_CHOICES) {
            std::cerr << "エラー: 正解インデックスが不正です（問題 " << (i + 1) << "）\n";
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}

// ユーザーに質問を表示して回答を取得する関数
void askQuestions(Quiz quizArray[], int userAnswers[], int numQuestions) {
    for (int i = 0; i < numQuestions; ++i) {
        std::cout << "問題 " << (i + 1) << ": " << quizArray[i].question << "\n";

        // 選択肢を表示
        for (int j = 0; j < MAX_CHOICES; ++j) {
            std::cout << j << ": " << quizArray[i].choices[j] << "\n";
        }

        // ユーザーに回答を求める
        std::cout << "番号で回答してください (0～2): ";
        std::cin >> userAnswers[i];

        // 入力チェック
        while (std::cin.fail() || userAnswers[i] < 0 || userAnswers[i] >= MAX_CHOICES) {
            std::cin.clear(); // エラー状態をクリア
            std::cin.ignore(1000, '\n'); // バッファをクリア
            std::cout << "無効な入力です。0～2の番号で回答してください: ";
            std::cin >> userAnswers[i];
        }
    }
}

// 結果をファイルに保存する関数
void saveResults(const char* filename, Quiz quizArray[], int userAnswers[], int correctAnswers[], int numQuestions) {
    std::ifstream check(filename);
    std::ofstream file;

    // ファイルが存在する場合は追記モード、それ以外は新規作成
    if (check.good()) {
        file.open(filename, std::ios::app);
    }
    else {
        file.open(filename);
    }

    if (!file.is_open()) {
        std::cerr << "エラー: 結果ファイルを開けません: " << filename << "\n";
        return;
    }

    for (int i = 0; i < numQuestions; ++i) {
        file << "問題 " << (i + 1) << ": " << quizArray[i].question << "\n";
        file << "あなたの回答: " << quizArray[i].choices[userAnswers[i]] << "\n";
        file << "正解: " << quizArray[i].choices[correctAnswers[i]] << "\n";
        file << ((userAnswers[i] == correctAnswers[i]) ? "正解！" : "不正解") << "\n\n";
    }

    file.close();
}