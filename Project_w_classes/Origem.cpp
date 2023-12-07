#define _USE_MATH_DEFINES

#include <iostream>
#include<ctime>
#include <cmath>
#include<iomanip>
#include <string> 
#include <vector>
#include <fstream>
#include <windows.h>


using namespace std;


// Funções auxiliares que permitem ler um inteiro e um character e evitar erros. Também permite criar um ficheiro de texto com os dados do jogo e vencedores.
int readInt(string type, int N_COL=8,int N_ROW=8) {

	//Lê um inteiro, limpa erros,limpa o buffer e pede que se digite um novo caso algo está errado(entrada invalida). 
	//Type: variavel que permite saber que tipo de inteiro estamos a ler para definir o range do mesmo: Exemplo:type=SizeBoard --> range[6-10]
	int value;
	cin >> value;

	if (cin.fail())
	{

		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Error! Invalid number! Put new one:";
		value = readInt(type);

	}
	if (cin.peek() != '\n') // verfica se o primeiro char na memória é um '/n' (significa que não ficou nada no buffer). Caso contrário dá erro... 
	{
		cin.ignore(1000, '\n');
		cout << "Error! Invalid number! Put new one:";
		value = readInt(type);
	}

	if (type == "SizeBoard") {
		if (value < 6 || value>10) // verifica se o tamanho inserido pelo utilizador está dentro do range
		{
			cin.ignore(1000, '\n');
			cout << "Put the value in the range [6-10]:";
			value = readInt(type);
		}
	}
	else if (type == "nvenc") {
		if (value < 4|| value>N_COL||value>N_ROW) // verfica se o numero de peças para ganhar é maior que 4 e se é maior que as dimensões do tabuleiro
		{
			cin.ignore(1000, '\n');
			cout << "Invalid input! Value not in the range or invalid with respect to the board size! Put new one [>=4]:";
			value = readInt(type,N_COL,N_ROW);
		}
	}
	else // caso não colocarmos outro "type" significa que é a dificuldade do cpu
	{
		if (value != 1 && value != 2 && value != 3) // verfica se o nivel de dificuldade inserido é 1, 2 ou 3
		{
			cin.ignore(1000, '\n');
			cout << "Invalid value. Choose [1-3]:";
			value = readInt(type);
		}
	}

	return value;

}

char readChar() {

	//Lê um Char, limpa erros,limpa o buffer e pede que se digite um novo char caso algo está errado(mais de um caractere/entrada invalida). 
	char character;
	cin >> character;

	if (cin.fail())
	{

		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Error! Invalid character! Put new one:";
		character = readChar();

	}
	if (cin.peek() != '\n') // verfica se o primeiro char na memória é um '/n' (significa que só foi digitado um caractere). Caso contrário dá erro... 
	{
		cin.ignore(1000, '\n');
		cout << "Only one character! Put new one:";
		character = readChar();
	}

	return character;

}

void results_file_text(string name_winner, string player1, string player2)
{
	// cria um ficheiro de texto com a data e hora atual; jogadores e o vencedor

	time_t tmNow;
	struct tm t;
	tmNow = time(0);
	localtime_s(&t, &tmNow);

	// Create and open a text file
	ofstream MyFile;
	MyFile.open("Results.txt", ios_base::app);
	// Write to the file
	MyFile << t.tm_mday << "-" << t.tm_mon + 1 << "-" << t.tm_year + 1900 << " / " << setfill('0') << setw(2) << t.tm_hour << ":" << setfill('0') << setw(2) << t.tm_min << ":" << setfill('0') << setw(2) << t.tm_sec << " - " << "1)" << player1 << " vs 2)" << player2 << " vencedor:" << name_winner << endl;

	// Close the file
	MyFile.close();
}


class Player
{

private:
	string name; // nome do jogador
	char marker; // marcador ('X' ou 'O')
	int level_cpu;// nivel de dificuldade do cpu
public:

	Player();
	void setname();
	void setname_cpu(int level_cpu);
	void SetMarker(char marker);
	string getname();
	char get_marker();
};
Player::Player()
{}
void Player::setname() //Define o nome do jogador
{
	getline(cin, name);

	if (cin.fail())
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cerr << "Error. Invalid input. Try another name." << endl;
		setname();

	}
}
void Player::setname_cpu(int level_cpu) //Define o nome do CPU de acordo com o nivel de dificuldade
{
	if (level_cpu == 1)
		name = "CPU Amador";
	else if (level_cpu == 2)
		name = "CPU Intermediate";
	else
		name = "CPU Hard";

}
void Player::SetMarker(char marker)//Define o marcador do jogador -> X ou O
{
	this->marker = marker;
}
char Player::get_marker() { //Retorna o marcador do jogador
	return marker;
}
string Player::getname() //Retorna o nome definido anteriormente
{
	return name;
}


class CPU
{
	//Class para definir jogadas do CPU

private:
	char letter; // letra escolhida pelo PC que vai ser devolvida
	char letters[10] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G','H','I','J' }; // array de char que contem todas as letras para um máximo de 10 colunas
	bool found; //variavel que permite saber se uma coluna foi "encontrada" para o PC colocar a peça
public:

	CPU();
	char pc_amador(const int N_COL);
	char pc_intermediate(const int N_COL, const int N_ROW, const int nVenc, const char marker_p1, const char marker_p2, const vector< vector<char>>board, const char empty_symbol);
	char pc_hard(const int N_COL, const int N_ROW, const int nVenc, const char marker_p1, const char marker_p2, const vector< vector<char>>board, const char empty_symbol);
	bool check_line(const string line, const char marker_to_analyse, const int N_COL, const int N_ROW, const int nVenc, const int actual_row, const vector< vector<char>>board, const char empty_symbol);
	bool check_coloumn(const string line, const char marker_to_analyse, const int nVenc, const int col, const char empty_symbol);
	bool check_diagonal(const string diagonal, const string type, const char marker_to_analyse, const int col, const int row, const int N_ROW, const int N_COL, const int nVenc, const vector< vector<char>>board,const char empty_symbol);
	void set_letter(char letter);
	char get_letter();
	void set_found(bool found);
	bool get_found();
};
CPU::CPU() {}

char CPU::pc_amador(const int N_COL) {
	// o computador escolhe uma letra aleatória para colocar a peça

	srand(time(NULL)); // seed muda cada segundo
	int m = rand() % N_COL; // m é o indice da coluna que é escolhido aleatoriamente pelo pc
	letter = letters[m];
	return letter;

}

char CPU::pc_intermediate(const int N_COL, const int N_ROW, const int nVenc, const char marker_p1, const char marker_p2, const vector< vector<char>>board, const char empty_symbol) {

	// o computador escolhe uma letra analisando as linhas e colunas

	char marker_for_win = marker_p2; // marker do CPU
	char marker_for_avoid_win = marker_p1;// marker do jogador 1

	// O código que se apresenta é de forma a o CPU seguir o seguinte raciocinio logico:
	//1-Verificar se pode ganhar pelas linhas;2-Verificar se pode ganhar pelas colunas;3-Verificar se pode impedir de ganhar pelas linhas; 4-Verificar se pode impedir de ganhar pelas colunas

	//1-Verificar se pode ganhar pelas linhas
	for (int row = 0;row <= N_ROW - 1;row++) {
		//line representa cada uma das N_ROW linhas do tabuleiro 
		string line = "";
		for (auto c : board.at(row)) {
			line = line + c;
		}
		//Retorna a letra da coluna que é preciso colocar a peça caso existir hipotese de vitória numa linha
		if (check_line(line, marker_for_win, N_COL, N_ROW, nVenc, row, board, empty_symbol)) {
			letter = get_letter();
			set_found(true);
			return letter;
		}

	}

	//2 - Verificar se pode ganhar pelas colunas
	for (int col = 0;col <= N_COL - 1;col++) {
		string coloumn = "";
		for (int i = 0; i <= N_ROW - 1; i++) {
			coloumn = coloumn + board[i][col];
		}
		//Retorna a letra da coluna que é preciso colocar a peça caso existir hipotese de vitória numa coluna
		if (check_coloumn(coloumn, marker_for_win, nVenc, col, empty_symbol)) {
			letter = get_letter();
			set_found(true);
			return letter;
		}

	}
	//3-Verificar se pode impedir de ganhar pelas linhas;
	for (int row = 0;row <= N_ROW - 1;row++) {
		string line = "";
		//line representa cada uma das N_ROW linhas do tabuleiro 
		for (auto c : board.at(row)) {
			line = line + c;
		}
		//Retorna a letra da coluna que é preciso colocar a peça caso existir hipotese de vitória do oponente numa linha
		if (check_line(line, marker_for_avoid_win, N_COL, N_ROW, nVenc, row, board, empty_symbol)) {
			letter = get_letter();
			set_found(true);
			return letter;
		}

	}
	//4-Verificar se pode impedir de ganhar pelas colunas;
	for (int col = 0;col <= N_COL - 1;col++) {
		string coloumn = "";
		for (int i = 0; i <= N_ROW - 1; i++) {
			coloumn = coloumn + board[i][col];
		}
		//Retorna a letra da coluna que é preciso colocar a peça caso existir hipotese de vitória do oponente numa coluna
		if (check_coloumn(coloumn, marker_for_avoid_win, nVenc, col, empty_symbol)) {
			letter = get_letter();
			set_found(true);
			return letter;
		}

	}

	//Caso não encontrar nenhuma hipotese devolve uma letra aleatória
	set_found(false);// significa que não encontramos nenhuma coluna para colocar a peça	
	letter = pc_amador(N_COL);
	return letter;
}

char CPU::pc_hard(const int N_COL, const int N_ROW, const int nVenc, const char marker_p1, const char marker_p2, const vector< vector<char>>board, const char empty_symbol) {
	letter = pc_intermediate(N_COL, N_ROW, nVenc, marker_p1, marker_p2, board,empty_symbol);// faço toda a verificação das linha e colunas
	found = get_found();

	// o computador escolhe uma letra analisando as linhas e colunas

	char marker_for_win = marker_p2; // marker do CPU
	char marker_for_avoid_win = marker_p1;// marker do jogador 1

	if (found == false) // se ainda não se encontrou nenhuma coluna para se colocar a peça(sem ser aleatório), verifica agora se existem diagonais onde o pc pode colocar uma peça
	{

		// Verificamos todas as diagonais 
		
		for (int col = 0;col <= N_COL - 1; col++) {// percorre todas as colunas
			string diagonal_left = ""; // Guarda a diagonal para a esquerda (sentido de baixo para cima)
			string diagonal_right = "";// Guarda a diagonal para a direita (sentido de baixo para cima)
			
			int row = 0;

			if (col == 0) // Para a coluna 0 , verificar a metade inferior esquerda do tabuleiro(caso das diagonais para a esquerda (sentido de cima para baixo))
			{
				
				for (int row = 0;row <= N_ROW - 1;row++) {// percorre todas as linhas para a coluna 0
					string diagonal_left = "";
					int new_col = 0;// variavel que permite percorrer as colunas da diagonal que começa na coluna 0 e linha "row".
					
					for (int row_diagonal = row;row_diagonal <= N_ROW - 1;row_diagonal++) { // percorre a diagonal que começa na linha "row" e coluna 0.
						
						diagonal_left = diagonal_left + board[row_diagonal][new_col];
						new_col = new_col + 1;
						if (new_col > N_COL - 1)// fora do tabuleiro
							break;
					}

					//Verifica se é possivel ganhar com as diagonais da esquerda
					if (check_diagonal(diagonal_left, "diagonal_left", marker_for_win, col, row, N_ROW, N_COL, nVenc, board, empty_symbol) == true) {
						letter = get_letter();
						return letter;
					}
					if (check_diagonal(diagonal_left, "diagonal_left", marker_for_avoid_win, col, row, N_ROW, N_COL, nVenc, board, empty_symbol) == true) {
						letter = get_letter();
						return letter;
					}
				}


			}
			else if (col == N_COL - 1)
			{//Verificar a metade inferior direita do tabuleiro(caso das diagonais para a direita (sentido de cima para baixo))

				for (int row = 0;row <= N_ROW - 1;row++) {// percorre todas as linhas para a ultima coluna
					string diagonal_right = "";
					int new_col = col; // variavel que permite percorrer as colunas da diagonal que começa na ultima coluna e linha "row".
					
					for (int row_diagonal = row;row_diagonal <= N_ROW - 1;row_diagonal++) {
						diagonal_right = diagonal_right + board[row_diagonal][new_col];
						new_col = new_col - 1;
						if (new_col < 0)// fora do tabuleiro
							break;
					}

					//Verifica se é possivel ganhar com as diagonais da direita
					if (check_diagonal(diagonal_right, "diagonal_right", marker_for_win, col, row, N_ROW, N_COL, nVenc, board, empty_symbol) == true) {
						letter = get_letter();
						return letter;
					}
					//Verifica se é possivel evitar o adversario ganhar com as diagonais da direita
					if (check_diagonal(diagonal_right, "diagonal_right", marker_for_avoid_win, col, row, N_ROW, N_COL, nVenc, board, empty_symbol) == true) {
						letter = get_letter();
						return letter;
					}
				}

			}
			else {

				//Verificar a metade superior direita do tabuleiro(caso das diagonais para a esquerda (sentido de cima para baixo))
				//Verificar a metade inferior esquerda do tabuleiro(caso das diagonais para a direita (sentido de cima para baixo))

				row = 0; // indice inicial para a linha(importante para o check_diagonal)
				//
				int new_row = 0;// começar de cima do tabuleiro
				// forma a string para cada diagonal que começa na coluna "col"

				//Diagonal esquerda :
				for (int col_diagonal = col; col_diagonal <= N_COL - 1; col_diagonal++) {
					diagonal_left = diagonal_left + board[new_row][col_diagonal];
					new_row = new_row + 1;
					if (new_row > N_ROW - 1)// fora do tabuleiro
						break;
				}
				//Diagonal direita :
				new_row = 0;// começar de cima do tabuleiro
				for (int col_diagonal = col; col_diagonal >= 0; col_diagonal--) {
					diagonal_right = diagonal_right + board[new_row][col_diagonal];
					new_row = new_row + 1;
					if (new_row > N_ROW - 1)// fora do tabuleiro
						break;
				}
			}

			//Verifica se é possivel ganhar com as diagonais da esquerda
			if (check_diagonal(diagonal_left, "diagonal_left", marker_for_win, col, row, N_ROW, N_COL, nVenc, board, empty_symbol) == true) {
				letter = get_letter();
				return letter;
			}
			//Verifica se é possivel ganhar com as diagonais da direita
			if (check_diagonal(diagonal_right, "diagonal_right", marker_for_win, col, row, N_ROW, N_COL, nVenc, board, empty_symbol) == true) {
				letter = get_letter();
				return letter;
			}
			//Verifica se pode impedir o adversário de ganhar para a diagonal da esquerda
			if (check_diagonal(diagonal_left, "diagonal_left", marker_for_avoid_win, col, row, N_ROW, N_COL, nVenc, board, empty_symbol) == true) {
				letter = get_letter();
				return letter;
			}
			//Verifica se pode impedir o adversário de ganhar para as diagonais da direita
			if (check_diagonal(diagonal_right, "diagonal_right", marker_for_avoid_win, col, row, N_ROW, N_COL, nVenc, board, empty_symbol) == true) {
				letter = get_letter();
				return letter;
			}
		}
	}
	else {
	// Se found==true retorna a letra que foi encontrada pelo modo do pc_intermediate
		return letter;
	}

	//Caso não encontrar nenhuma hipotese devolve uma letra aleatória
	set_found(false);// significa que não encontramos nenhuma coluna para colocar	
	letter = pc_amador(N_COL);
	return letter;

}

bool CPU::check_diagonal(const string diagonal, const string type, const char marker_to_analyse, const int col, const int row, const int N_ROW, const int N_COL, const int nVenc, const vector< vector<char>>board,const char empty_symbol) {

	string empty_space_in_the_middle;
	empty_space_in_the_middle = empty_space_in_the_middle + marker_to_analyse + empty_symbol + marker_to_analyse; // "X.X" ou "O.O"
	bool found = false;// variavel que permite parar de procurar a coluna certa se já tiver encontrada(found=true)
	int actual_row_l, actual_col_l, actual_row_r, actual_col_r, actual_row, actual_col;

	// Verficar se existe na string "diagonal" nVenc -1 markers consecutivos. Caso o numero de peças necessárias para vencer(nVenc) é 4, logo ex:(XXX.) OR (.XXX)

	if ((diagonal.find(string(nVenc - 1, marker_to_analyse))) != (string::npos)) {
		
		int pos_left = diagonal.find(string(nVenc - 1, marker_to_analyse)) - 1; // indice da posição da esquerda onde estará empty_symbol caso existir, ex: "[.]XXX", 
		int pos_right = diagonal.find(string(nVenc - 1, marker_to_analyse)) + nVenc - 1;//indice da posição da direita onde estará empty_symbol caso existir, ex: "XXX[.]",

		if (type == "diagonal_left") {// caso for uma diagonal da esquerda(sentido baixo para cima)
			actual_row_l = row + pos_left; // ver a linha relativamente ao tabuleiro (row: número da linha onde começa a string diagonal de indice 0)
			actual_col_l = col + pos_left;// ver o numero da coluna relativamente ao tabuleiro (col:número da coluna onde começa a string diagonal de indice 0)
			actual_row_r = row + pos_right; // ver a linha relativamente ao tabuleiro(row: número da linha onde começa a string diagonal de indice 0)
			actual_col_r = col + pos_right;//igual...
		}
		else { // caso for digonal da direita
			actual_row_l = row + pos_left; // mesmo raciocinio do que em cima
			actual_col_l = col - pos_left;
			actual_row_r = row + pos_right; 
			actual_col_r = col - pos_right;
		}

		if (pos_left >= 0)

			if (diagonal[pos_left] == empty_symbol) { //caso existir o empty_symbol, ex: "[.]XXX"

				if (actual_row_l == N_ROW - 1) { //  se for a linha de baixo do tabuleiro (a peça cai sempre nessa linha).	
					letter = letters[actual_col_l]; // já encontramos uma coluna para colocar a peça
					set_letter(letter);
					found = true;
				}
				else {

					if (board[actual_row_l + 1][actual_col_l] != empty_symbol) {// verificar se na linha de baixo existe alguma peça, se existir podemos colocar a peça nessa coluna porque vai ficar em cima
						letter = letters[actual_col_l]; // já encontramos uma coluna para colocar a peça
						set_letter(letter);
						found = true;
					}

				}
			}
		if (found == false && actual_col_r <= N_COL - 1) // se ainda nada foi encontrado, ver a posição da direita. O resto é exatamente o mesmo raciocinio explicado anteriormente
			if (diagonal[pos_right] == empty_symbol) {
				if (actual_row_r == N_ROW - 1) { //  se for a linha de baixo...

					letter = letters[actual_col_r];
					set_letter(letter);
					found = true;
				}
				else {
					if (board[actual_row_r + 1][actual_col_r] != empty_symbol) {// verificar se na linha de baixo existe alguma peça
						letter = letters[actual_col_r];
						set_letter(letter);
						found = true;
					}
				}
			}
	}

	// Verficar se existe na string "diagonal" (XX.X) OR (X.XX) de modo a verificar se colocarmos uma peça no meio, ganhamos ou evitamos o adversário ganhar
	
	if (found == false && diagonal.find(empty_space_in_the_middle) != (string::npos)) { // se encontramos este caso, colocamos o marker no empty_symbol e vemos se ganhamos ou se o adbersário ganha. Se sim, colocar peça
		int pos_empty = diagonal.find(empty_space_in_the_middle) + 1;// posição do empty_symbol

		if (type == "diagonal_left") {
			actual_row = row + pos_empty; // ver a linha relativamente o tabuleiro(row:linha onde começa a string diagonal; pos_empty:indice de onde está o empty_symbol na string)
			actual_col = col + pos_empty;
		}
		else {
			actual_row = row + pos_empty; // ver a linha relativamente o tabuleiro(row:linha onde começa a string diagonal; pos_empty:indice de onde está o empty_symbol na string)
			actual_col = col - pos_empty;
		}

		string false_line = diagonal; // esta variavél serve para criar uma string "falsa" que é uma cópia da diagonal.
		false_line[pos_empty] = marker_to_analyse;//substituir o empty_symbol pela peça.
		if ((false_line.find(string(nVenc, marker_to_analyse))) != (string::npos)) { // se ganharmos ou o adversário ganha com nVenc peças consecutivas, devemos colocar a peça nessa coluna
			
			if (actual_row == N_ROW - 1) { //  se for a linha de baixo...
				letter = letters[actual_col];
				set_letter(letter);
				found = true;
			}
			else {
				if (board[actual_row + 1][actual_col] != empty_symbol) {// verificar se na linha de baixo existe alguma peça
					letter = letters[actual_col];
					set_letter(letter);
					found = true;
				}
			}
		}
	}
	return found;


}

bool CPU::check_coloumn(const string coloumn, const char marker_to_analyse, const int nVenc, const int col, const char empty_symbol) {

	bool found = false;
	// Verficar se existe na string "diagonal" nVenc -1 markers consecutivos. Caso o numero de peças necessárias para vencer(nVenc) é 4, logo ex:(XXX.) OR (.XXX)
	if ((coloumn.find(string(nVenc - 1, marker_to_analyse))) != (string::npos)) {
		// se encontramos este caso, colocamos o marker no empty_symbol e vemos se ganhamos ou se o adversário ganha.
		int pos_empty = coloumn.find(string(nVenc - 1, marker_to_analyse)) - 1;// posição do empty_symbol na string
		string false_line = coloumn;// esta variavél serve para criar uma string "falsa" que é uma cópia da coluna.
		if (pos_empty >= 0) {
			false_line[pos_empty] = marker_to_analyse;//substituir empty_symbol pela peça a analisar
			if (coloumn[pos_empty] == empty_symbol && (false_line.find(string(nVenc, marker_to_analyse))) != (string::npos)) {//Caso ganharmos ou o adversário ganha:
				letter = letters[col];// escolhemos a coluna
				set_letter(letter);
				found = true;
			}
			else
				found = false;
		}
		else
			found = false;
	}
	return found;

}

bool CPU::check_line(const string line, const char marker_to_analyse, const int N_COL, const int N_ROW, const int nVenc, const int actual_row, const vector< vector<char>>board, const char empty_symbol) {

	string empty_space_in_the_middle;
	empty_space_in_the_middle = empty_space_in_the_middle + marker_to_analyse + empty_symbol + marker_to_analyse;// "X.X" ou "O.O"
	bool found = false;// variavel que permite parar de procurar a coluna certa se já tiver encontrada(found=true)

	// Verficar se existe na string "line" nVenc -1 markers consecutivos. Caso o numero de peças necessárias para vencer(nVenc) é 4, logo ex:(XXX.) OR (.XXX)

	if ((line.find(string(nVenc - 1, marker_to_analyse))) != (string::npos)) {
		int pos_left = line.find(string(nVenc - 1, marker_to_analyse)) - 1; // indice da posição da esquerda onde estará empty_symbol caso existir, ex: "[.]XXX", 
		int pos_right = line.find(string(nVenc - 1, marker_to_analyse)) + nVenc - 1;// indice da posição da direita onde estará empty_symbol caso existir, ex: "XXX[.]", 
		
		if (pos_left >= 0)

			if (line[pos_left] == empty_symbol) {//caso existir o empty_symbol, ex: "[.]XXX"
				if (actual_row == N_ROW - 1) { //  se for a linha de baixo do tabuleiro (a peça cai sempre nessa linha).
					letter = letters[pos_left];// já encontramos uma coluna para colocar a peça
					set_letter(letter);
					found = true;
				}
				else {
					if (board[actual_row + 1][pos_left] != empty_symbol) {// verificar se na linha de baixo existe alguma peça, se existir podemos colocar a peça nessa coluna porque vai ficar em cima
						letter = letters[pos_left];
						set_letter(letter);
						found = true;
					}

				}
			}
		if (found == false && pos_right <= N_COL - 1) // caso na posição da esquerda não encontrou empty_symbol, então ver a posição da direita
			if (line[pos_right] == empty_symbol) {
				if (actual_row == N_ROW - 1) { //  se for a linha de baixo...

					letter = letters[pos_right];
					set_letter(letter);
					found = true;
				}
				else {
					if (board[actual_row + 1][pos_right] != empty_symbol) {// verificar se na linha de baixo existe alguma peça
						letter = letters[pos_right];
						set_letter(letter);
						found = true;
					}
				}
			}
	}
	// Verficar se existe na string "line" (XX.X) OR (X.XX) de modo a verificar se colocarmos uma peça no meio, ganhamos ou evitamos o adversário ganhar

	if (found == false && line.find(empty_space_in_the_middle) != (string::npos)) { // se encontramos este caso, colocamos o marker no empty_symbol e vemos se ganhamos ou o adversário ganha. Se sim, colocamos.
		int pos_empty = line.find(empty_space_in_the_middle) + 1;
		string false_line = line;// esta variavél serve para criar uma string "falsa" que é uma cópia da linha.
		false_line[pos_empty] = marker_to_analyse;//substituir empty_symbol pela peça.
		
		if ((false_line.find(string(nVenc, marker_to_analyse))) != (string::npos)) {// se ganharmos ou o adversário ganha com nVenc peças consecutivas, devemos colocar a peça nessa coluna
			
			if (actual_row == N_ROW - 1) { //  se for a linha de baixo...
				letter = letters[pos_empty];
				set_letter(letter); // econtramos a coluna para colocar a peça
				found = true;
			}
			else {
				if (board[actual_row + 1][pos_empty] != empty_symbol) {
					letter = letters[pos_empty];
					set_letter(letter);
					found = true;
				}
			}
		}
	}
	return found; // retorna se encontramos ou não a coluna para colocar a peça
}

void CPU::set_letter(char letter) {
	this->letter = letter;
}
char CPU::get_letter() {
	return letter;
}
void CPU::set_found(bool found) {
	this->found = found;
}
bool CPU::get_found() {
	return found;
}

class Board
{ // representa todas as funções que atuam no tabuleiro
private:
	vector< vector<char> > board;
	char current_marker;// marcador atual do jogador
	char letter;// letra escolhida pelo jogador 
	int N_COL; // Número de colunas da board
	int N_ROW; // Número de linhas da board
	int nVenc; //Número de peças necessárias para vencer o jogo
	char empty_symbol; // símbolo dos espaços vazios
	int actual_row, actual_col; // linha e coluna da peça depositada atualmente
	
public:

	Board(int N_COL, int N_ROW, int nVenc, char empty_symbol = '.');
	bool check_cin(char& letter);
	bool valid_letter(const char& letter);
	bool check_column_full(const int& col);
	int check_row_to_put();
	void update_board(const char& current_marker);
	bool check_line(const char &current_marker);
	bool check_coloumn(const char& current_marker);
	bool check_diagonal(const char& current_marker);
	bool check_winner(const char& current_marker);
	bool all_board_full();
	char Color(const char marker, int colour = 7);
	void DisplayBoard();
	
	// Poder ter estas variaveis importantes para todas as funções da classe sem ser necessário ser sempre argumentos das funções
	int get_actual_row();
	int get_actual_col();
	void set_actual_row(int actual_row);
	void set_actual_col(int actual_col);

	// Importante poder devolver o vector que representa o tabuleiro para o pc inteligente;
	vector< vector<char>> get_Board();
};

Board::Board(int N_COL, int N_ROW, int nVenc, char empty_symbol)
{
	this->N_COL = N_COL;
	this->N_ROW = N_ROW;
	this->nVenc = nVenc;
	this->empty_symbol = empty_symbol;

	//2D vector with N_ROW lines and N_COL columns para criar tabuleiro
	this->board = vector< vector<char>>(N_ROW, vector <char>(N_COL, empty_symbol));

}

bool Board::check_cin(char& letter) {
	//Função que vai defenir e avaliar a letra inserida pelo utilizador
	//Retorna true se não houver nenhuma flag de erro no cin, se jogador decide continuar e se o caracter escolhido é valido(caracter é guardado por referencia na variavel letter).
	//Retorna falso se o jogador não quer continuar o jogo e acaba a execuçao do programa

	cin >> letter;

	if (cin.fail())
	{

		if (cin.eof()) // Retorna verdadeiro se o jogador colocou CRTL-Z
		{
			cin.clear();
			cout << "Do you want to stop the game (y/n)? ";
			char stop;

			stop = readChar();// resposta para a questão do jogador querer parar de jogar.

		inicio:
			if (toupper(stop) == 'Y')
				return false; // devolve o valor falso se o o jogador não quer jogar mais
			else if (toupper(stop) == 'N') {

				cin.ignore(1000, '\n');
				cout << "Ok, so continue..." << endl;
				cout << "Digit the character to put your piece(CTRL-Z to end): ";
				check_cin(letter);// Volta a ler o valor inserido do jogador e faz todo o processo de novo
			}
			else {
				cout << "Invalid choise, put new one (y/n):"; // não colocou uma opçao válida (y ou n)
				stop = readChar(); // voltar de novo a ler o valor 
				goto inicio; // voltar ao inicio e avaliar escolha
			}

		}
		else // se não for CTRL-Z ...
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Error!" << endl;
			cout << "Digit the character to put your piece(CTRL-Z to end): ";

			check_cin(letter);
		}
	}
	else if (cin.peek() != '\n') { // se não houver um '/n' no buffer quer dizer que o jogador colocou mais caracteres

		cin.ignore(1000, '\n');
		cout << "Too many characters! Put only one! ";
		cout << "Digit the character to put your piece(CTRL-Z to end): ";
		check_cin(letter); // voltar recursivamente a ler o valor inserido pelo jogador
	}
	else
		if (isdigit(letter)) { // caso o char introduzido seja um digito
			cin.ignore(1000, '\n');
			cout << "Invalid character because it is a digit! Digit new character to put your piece (CTRL-Z to end): ";
			check_cin(letter);
		}
		else if(!isalpha(letter)) { // caso o char introduzido seja um digito
			cin.ignore(1000, '\n');
			cout << "Invalid character because it is not alphanumeric! Digit new character to put your piece (CTRL-Z to end): ";
			check_cin(letter);
		}
		else if (valid_letter(toupper(letter)))// se for verdadeiro o programa continua (letra bem escolhida)
		{
			set_actual_col(toupper(letter) - 'A');// a coluna da peça actual está encontrada
			return true;
		}
		else {
			cin.ignore(1000, '\n');
			cout << "Invalid character, digit new character to put your piece (CTRL-Z to end): ";
			check_cin(letter);// se for falso, manda a mensagem que está na valid_letter e voltamos a testar recursivamente, pedindo novamente o caracter ao player
		}
}

bool Board::valid_letter(const char& letter) // verifica se a letra escolhida é válida ou seja, se existe no tabuleiro ou se a coluna já está preenchida.
{

	bool valid_letter; // Verdadeiro se a letra é válida, falso caso contrário
	int col = letter - 'A'; // indice da coluna da letra.

	if (col < N_COL && (check_column_full(col))) // verifica se a letra está no tabuleiro, verificando também se a coluna dessa letra não está cheia
	{
		valid_letter = true;// letra é valida
	}
	else if (col < N_COL && (!check_column_full(col))) // verifica se a letra está no tabuleiro, verificando também se a coluna dessa letra não está cheia
	{
		cout << "This column is full!" << endl;
		valid_letter = false; // coluna já está cheia
	}
	else {
		cout << "The character doesn't exist in the board!" << endl;
		valid_letter = false; // a letra não existe no tabuleiro
	}
	return valid_letter;
}

bool Board::check_column_full(const int& col) { // funçao que devolve falso se uma coluna já esta preenchida
	 // col: coluna escolhida pelo jogador
	if (board[0][col] != empty_symbol)
		return false;
	else
		return true;

}

int Board::check_row_to_put() { // vai encontrar a linha onde a peça é colocada para a coluna "col"
	for (int row = N_ROW - 1; row >= 0; row--)
		if (board[row][actual_col] == empty_symbol)
			return row;
}

void Board::update_board(const char& current_marker) // função que coloca a peça do tipo "current marker", de acordo com a letra ("letter") escolhida
{
	// introduce new piece
	int row;

	row = check_row_to_put(); // vê qual é a linha onde vai ser colocada a peça
	board[row][actual_col] = current_marker; // coloca a peça na linha "row" encontrada 
	set_actual_row(row);

}

bool Board::check_line(const char& current_marker) {

	//Função que vai verificar a linha da peça que foi depositada. Retorna Verdadeiro se alguém ganha.
	string line;

	for (auto c : board.at(actual_row))// percorre todas as colunas da linha actual_row e adiciona à string 
		line = line + c;

	if ((line.find(string(nVenc, current_marker))) != (string::npos)) { // se existe nVenc peças consecutivas, há um vencedor

		return true;

	}
	else
		return false;

}

bool Board::check_coloumn(const char& current_marker) {

	//Função que vai verificar a coluna da peça que foi depositada. Retorna Verdadeiro se alguém ganha.

	string coloumn;

	for (int i = 0; i <= N_ROW - 1; i++) { // percorre todas as linhas da coluna actual_col e adiciona à string 
		coloumn = coloumn + board[i][actual_col];
	}

	if ((coloumn.find(string(nVenc, current_marker))) != (string::npos)) {// se existe nVenc peças consecutivas, há um vencedor

		return true;
	}
	else
		return false;


}

bool Board::check_diagonal(const char& current_marker) {

	//Função que vai formar as diagonais onde se incluí a peça depositada. Retorna Verdadeiro se alguém ganha.

	string diagonal_left;// diagonal da esquerda onde se inclui a peça (sentido cima para baixo)

	string diagonal_left_l;// para a diagonal da esquerda, as peças que estão à esquerda da peça
	string diagonal_left_r;// para a diagonal da esquerda, as peças que estão à direita da peça

	string diagonal_right;// diagonal da direita onde se inclui a peça (sentido cima para baixo)

	string diagonal_right_l;// para a diagonal da direita, as peças que estão à esquerda da peça
	string diagonal_right_r;// para a diagonal da direita, as peças que estão à direita da peça


	int n_line_ciclo = actual_row; // variavel que vai ser constantemente atualizada em cada ciclo e que representa o indice das linhas que permitem formar a diagonal 

	//Formar diagonal da direita que está à esquerda da peça. 
	for (int col = actual_col; col >= 0; col--) {
		diagonal_right_l = diagonal_right_l + board[n_line_ciclo][col];
		n_line_ciclo = n_line_ciclo + 1;// actualiza para a linha de baixo do tabuleiro
		if (n_line_ciclo > N_ROW - 1)// fora do tabuleiro
			break;
	}
	reverse(diagonal_right_l.begin(), diagonal_right_l.end());// inverter a ordem da string (foi concatenada no sentido contrário)

	n_line_ciclo = actual_row - 1;

	//Formar diagonal da direita que está à direita da peça.
	for (int col = actual_col + 1; col <= N_COL - 1; col++) { // adiciona-se actual_col +1 porque a peça na posição [actual_row,actual_col] já foi adicionado na string anterior
		if (n_line_ciclo < 0)// fora do tabuleiro
			break;

		diagonal_right_r = diagonal_right_r + board[n_line_ciclo][col];
		n_line_ciclo = n_line_ciclo - 1;// actualiza para a linha de cima do tabuleiro

		if (n_line_ciclo < 0)// fora do tabuleiro
			break;
	}
	diagonal_right = diagonal_right_l + diagonal_right_r;// concatena as duas diagonais para formar a diagonal inteira!


	n_line_ciclo = actual_row;// definir de novo a posição do row da ultima peça colocada
	//Formar diagonal da esquerda que está à direita da peça.
	for (int col = actual_col; col <= N_COL - 1; col++) {
		diagonal_left_r = diagonal_left_r + board[n_line_ciclo][col];
		n_line_ciclo = n_line_ciclo + 1;
		if (n_line_ciclo > N_ROW - 1)// fora do tabuleiro
			break;
	}
	reverse(diagonal_left_r.begin(), diagonal_left_r.end());// inverter a ordem da string (foi concatenada no sentido contrário)

	n_line_ciclo = actual_row - 1;

	//Formar diagonal da esquerda que está à esquerda da peça.
	for (int col = actual_col - 1; col >= 0; col--) { // adiciona-se actual_col -1 porque já foi adicionado na string inicial
		if (n_line_ciclo < 0)// fora do tabuleiro
			break;

		diagonal_left_l = diagonal_left_l + board[n_line_ciclo][col];
		n_line_ciclo = n_line_ciclo - 1;

		if (n_line_ciclo < 0)// fora do tabuleiro
			break;
	}
	diagonal_left = diagonal_left_r + diagonal_left_l;// concatena as duas diagonais para formar a diagonal inteira!

	//Verifica se existem nVenc peças consecutivas do mesmo "current_marker". Se sim, existe vencedor.
	if ((diagonal_left.find(string(nVenc, current_marker))) != (string::npos) || (diagonal_right.find(string(nVenc, current_marker))) != (string::npos)) {

		return true;

	}
	else

		return false;


}

bool Board::check_winner(const char& current_marker) // Função para analisar a board para ver se há vencedor.Retorna Verdadeiro se há vencedor, senão retorna Falso
{
	//Verifica se se formou uma linha, coluna ou diagonal pelas funções.
	if (check_diagonal(current_marker) || check_coloumn(current_marker) || check_line(current_marker))
		return true;
	else
		return false;


}

bool Board::all_board_full()
{
	//retorna verdadeiro se o tabuleiro está cheio, caso de empate
	for (int i = 1; i <= N_ROW; i++) {
		for (int j = 1; j <= N_COL; j++) {
			if (board[i - 1][j - 1] == empty_symbol)
				return false;
		}
	}
	return true;
}

char Board::Color(char marker, int colour) {
	//Define a cor azul para a peça "X" e vermelho para a peça "0"
	if (marker == 'X')
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour = 1);
	else if (marker == 'O')
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour = 4);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);
	return marker;
}

void Board::DisplayBoard() { // mostra o tabuleiro "board"
	char letters[10] = { 'A','B','C','D','E','F','G','H','I','J' };
	cout << endl;
	for (int l = 0; l <= N_COL - 1; l++)
		cout << setw(3) << letters[l];
	cout << endl;
	for (int i = 0; i <= N_ROW - 1; i++) {
		for (int j = 0; j <= N_COL - 1; j++)
		{
			cout << setw(3) << Color(board[i][j]); // Color define a cor da peça
			if (j == N_COL - 1) // Se chegou ao fim do tabuleiro , muda de linha
				cout << endl;
		}
	}
	Color(empty_symbol); // Qualquer char sem ser uma das peças faz com que retorne à cor original
	for (int l = 0; l <= N_COL - 1; l++)
		cout << setw(3) << letters[l];
	cout << endl;
	cout << endl;
}

int Board::get_actual_row() {
	return actual_row;
}
int Board::get_actual_col() {
	return actual_col;
}
void Board::set_actual_col(int actual_col) {
	this->actual_col = actual_col;
}
void Board::set_actual_row(int actual_row) {
	this->actual_row = actual_row;
}
vector< vector<char>> Board::get_Board() {
	return board;
}


int main()
{
	//Escolher largura e comprimento do tabuleiro

	int N_ROW; // Número de colunas da board
	int N_COL; // Número de linhas da board
	int n_Venc; // Número de peças para ser vencedor

	cout << "Welcome to the game Connect N! Connect N of your checkers in a row. The goal is to align N checkers horizontally, vertically, or diagonally while preventing your opponent from doing the same. But, look out! Your opponent can sneak up on you and win the game!" << endl << endl;

	cout << "Digit the size of the board (6-10), width : ";
	N_COL = readInt("SizeBoard");// Coloquei nomes para o readInt distinguir os ranges pretendidos
	cout << ", length: ";
	N_ROW = readInt("SizeBoard");
	cout << endl;
	cout << "Digit the number of pieces in a row for winning : ";
	n_Venc = readInt("nvenc",N_COL,N_ROW);
	cout << endl;

	char empty_symbol = '.';// variavél em que podemos mudar os simbolos dos espaços vazios
	Board bd(N_COL, N_ROW, n_Venc,empty_symbol);// Inicialização da board
	Player player1, player2; // inicializaçao dos dois jogadores
	CPU c1;// inicialização do CPU e dos vários modos de jogo

	char letter; // Inicialização da variavel "letter" que é a letra escolhida pelo jogador para colocar a peça
	char current_marker; // Peça ou "marker" atual (X or O).
	bool win = false; // variavel para testar no ciclo while quando alguem ganha ou há empate que dita a paragem do programa

	char letters[10] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G','H','I','J' };

	bd.DisplayBoard();

	cin.ignore(1000, '\n');//limpar buffer
	cout << "Name of player 1: ";
	player1.setname();

	int pc;// variavel para saber se o player 2 é pc ou nao (1-é pc;2-não é pc)
	int difficulty_level;// nivel de dificuldade do CPU;
	char pc_or_not;// resposta do utilizador(y/n)
	cout << "Do you want to play with the computer (y/n)?";
	pc_or_not = readChar();

inicio:
	if (toupper(pc_or_not) == 'Y') {
		pc = 1;
		cout << endl << "Do you want to play with which difficulty(Easy-1, Medium-2, Hard-3) ?:";
		difficulty_level = readInt("difficulty");
		player2.setname_cpu(difficulty_level);
	}
	else if (toupper(pc_or_not) == 'N') {
		pc = 2;
		cout << endl;
		cin.ignore(1000, '\n');
		cout << "Name of player 2: ";
		player2.setname();
	}
	else {
		cin.ignore(1000, '\n');
		cout << "Invalid choise, put new one (y/n):";
		pc_or_not = readChar();
		goto inicio;
	}

	//gerar "markers" aleatórios (X ou O para jogador 1 ou 2)

	srand(time(NULL)); // seed muda cada segundo para tornar rand() verdadeiramente aleatório
	int m = rand() % 2;
	int m2 = (m + 1) % 2;

	char markers[2] = { 'X','O' };
	player1.SetMarker(markers[m]);
	player2.SetMarker(markers[m2]);

	do
	{
		//PLAYER 1
		cout << endl;
		cout << "It's your time to play " << player1.getname() << endl << endl;
		cout << "Digit the character (A-" << letters[N_COL - 1] << ") to put your piece (CTRL-Z to end): ";

		if (!bd.check_cin(letter)) // lê a letra inserida pelo jogador. Se check_cin é falso (jogador não quer jogar), o programa para. Caso contrário continua se a letra estiver válida(ver função check_cin)
			return 0;

		current_marker = player1.get_marker(); // define a peça atual que vai ser colocada no tabuleiro

		bd.update_board(current_marker); //Coloca a peça no tabuleiro

		bd.DisplayBoard();// mostra o tabuleiro

		if (bd.check_winner(current_marker) || bd.all_board_full()) // se alguém ganhou ou há empate:
		{
			if (bd.check_winner(current_marker)) { // se alguém ganhou
				results_file_text(player1.getname(), player1.getname(), player2.getname());

				cout << player1.getname() << " WINS! Congratulations! " << endl;
				return 0;// força a paragem do programa
			}
			else {
				// empate se o tabuleiro estiver cheio

				results_file_text("Draw", player1.getname(), player2.getname());
				cout << " Draw! Congratulations both! " << endl;
				return 0; // força a paragem do programa

			}
		}

		else { // Se ó player 1 não ganhou, o jogo continua

			//PLAYER 2
			cout << endl;
			cout << "It's your time to play " << player2.getname() << endl << endl;
			cout << "Digit the character (A-" << letters[N_COL - 1] << ") to put your piece(CTRL-Z to end): ";


			if (pc == 1) { // Se estamos a jogar com o pc
				if (difficulty_level == 1) { // Se o PC é nivel amador
					letter = c1.pc_amador(N_COL);

					int col_choosen = toupper(letter) - 'A'; // indice da coluna escolhida 

					while (!bd.check_column_full(col_choosen))// Enquanto a coluna estiver cheia, escolher outra letra para a coluna
					{
						letter = c1.pc_amador(N_COL);
						col_choosen = toupper(letter) - 'A';

					}
					cout << letter << endl;
					bd.set_actual_col(col_choosen); // definir a coluna da peça escolhida pelo computador para fazer o update_board
				}
				else if (difficulty_level == 2) { // Se o pc tem um nivel intermediário
					letter = c1.pc_intermediate(N_COL, N_ROW, n_Venc, player1.get_marker(), player2.get_marker(), bd.get_Board(), empty_symbol);

					int col_choosen = toupper(letter) - 'A';// indice da coluna escolhida 

					while (!bd.check_column_full(col_choosen))// Enquanto a coluna estiver cheia, escolher outra letra para a coluna
					{
						letter = c1.pc_intermediate(N_COL, N_ROW, n_Venc, player1.get_marker(), player2.get_marker(), bd.get_Board(), empty_symbol);
						col_choosen = toupper(letter) - 'A';

					}
					cout << letter << endl;
					bd.set_actual_col(col_choosen);// definir a coluna da peça escolhida pelo computador para fazer o update_board
				}
				else {// Se o nivel do PC é dificil 
					letter = c1.pc_hard(N_COL, N_ROW, n_Venc, player1.get_marker(), player2.get_marker(), bd.get_Board(), empty_symbol);

					int col_choosen = toupper(letter) - 'A';

					while (!bd.check_column_full(col_choosen))// Enquanto a coluna estiver cheia, escolher outra letra para a coluna
					{
						letter = c1.pc_hard(N_COL, N_ROW, n_Venc, player1.get_marker(), player2.get_marker(), bd.get_Board(), empty_symbol);
						col_choosen = toupper(letter) - 'A';

					}
					cout << letter << endl;
					bd.set_actual_col(col_choosen);// definir a coluna da peça escolhida pelo computador para fazer o update_board

				}
			}
			else { // Se o player 2 é um jogador (Humano)
				if (!bd.check_cin(letter)) // lê a letra inserida pelo jogador. Se check_cin é falso (jogador não quer jogar), o programa para. Caso contrário continua se a letra estiver válida(ver função check_cin)
					return 0;
			}

			current_marker = player2.get_marker();

			bd.update_board(current_marker);//faz o update da board
			bd.DisplayBoard();

			if (bd.check_winner(current_marker) || bd.all_board_full()) // se alguém ganhou ou há empate:
			{
				if (bd.check_winner(current_marker)) {
					results_file_text(player2.getname(), player1.getname(), player2.getname());

					cout << player2.getname() << " WINS! Congratulations! " << endl;
					win = true;
				}
				else {
					results_file_text("Draw", player1.getname(), player2.getname());
					cout << " Draw! Congratulations both! " << endl;
					win = true;
				}

			}

		}

	} while ((win == false));


	return 0;

}