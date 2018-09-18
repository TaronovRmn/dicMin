#include <conio.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <direct.h>
#include <string>
#include <math.h>
#include <set>
#include <vector>
#include <ctime>
#include <algorithm>
using namespace std;

int DI�(int N, int M[]) {
	setlocale(LC_ALL, "rus");
	FILE *input, *output, *cfg, *tmp, *wrds, *wl;
	const string NIF = "ouR.txt"; //���� � ���������
	const string IWF = "inV.txt"; //���� � ���������
	const string CGF = "enU.txt"; //���� � ��������
	const string DIC = "dictionaryStr"; //������� �� ��������
	const string NOF = "outputMIN.txt"; //�����
	const string TMF = "tmp.txt"; // temporary file
	const int LA = 31;// ���� ����� ��������
	const int LW = 40;// ���� ����� �����

	

	struct Struct { //��������� �����
		char abc[LW] = ""; //���������
		int nWords = 0; // ���������� ���� ���������
		int first = 0; //������ ����� ���������
	};

	struct Word { //�����
		char abc[LW] = ""; //�����
		unsigned int freq = 0; //�������
		int next = 0; //��������� � ������
	};

	struct Dict { //����� � �������
		bool isThere = 0;
		unsigned int nStructs;
		unsigned int nWords;
		vector<Struct> structs;
		vector<Word> words;
	};
	
	struct Context {
		vector<int> lens;
		vector<Word> words;
	};

	struct Solution { // �������
		char abc[LA] = ""; //���
		unsigned int n = 0; // ����� � ������
		unsigned int nT = 0; // ����� � ������
		char met[6]; // �����
		double score = 0; // ������ 
	};

	struct Nw {
		vector<char> ch [LA];
	};

	struct f {
		Nw checkCont(Context cover, Context cont, string sol) {
			Nw nws;
			for (vector<int>::size_type i = 0; i != cover.words.size(); i++) {
				for (int j = 0; j < cover.lens[i]; j++) {
					int ism = sol.find_first_of(cover.words[i].abc[j]);
						
							if (find(nws.ch[ism].begin(), nws.ch[ism].end(), cont.words[i].abc[j]) == nws.ch[ism].end()) {
								nws.ch[ism].push_back(cont.words[i].abc[j]);
							}
						
					}
					
				
			}
			return nws;
		}
		void readDictionary(FILE *file, Dict& dictionary) {
			

			dictionary.isThere = 1;
			dictionary.nStructs = 0;
			dictionary.nWords = 0;

			fscanf_s(file, "%d", &dictionary.nWords);
			fscanf_s(file, "%d", &dictionary.nStructs);

			// read structures
			Struct strbf;
			for (int i = 0; i < dictionary.nStructs; i++)
			{
				fscanf_s(file, "%s", strbf.abc, (unsigned)_countof(strbf.abc));
				fscanf_s(file, "%d", &strbf.nWords);
				fscanf_s(file, "%d", &strbf.first);
				dictionary.structs.push_back(strbf);
			}
			
			// read words
			Word wrdbf;
			for (int i = 0; i < dictionary.nWords; i++)
			{
				fscanf_s(file, "%s", wrdbf.abc, (unsigned)_countof(wrdbf.abc));
				fscanf_s(file, "%d", &wrdbf.freq);
				fscanf_s(file, "%d", &wrdbf.next);
				dictionary.words.push_back(wrdbf);
			}
			

		}
		void printSol(FILE *output, Solution sl, int nSym) { //�����
			for (int ic = 0; ic < nSym; ic++) { fprintf_s(output, "%c", sl.abc[ic]); }
			fprintf_s(output, " %i\t%i\t", sl.n, sl.nT);
			for (int ic = 0; ic < 6; ic++) { fprintf_s(output, "%c", sl.met[ic]); }
			//fprintf(output, " %lf", sl.score);
			fprintf_s(output, "\n");
		}

		void decode(char* openWord, char* closeWord, Solution sl, char* stand, int len) { //�������������
			int ind = 0;
			for (int i = 0; i < len; i++) {
				int ist = 0;
				while (closeWord[i] != sl.abc[ist]) { ist++; }
				openWord[i] = stand[ist];
			}
		}
		void toStruct(char* str, char* word, int len) { //������� � ��������� �����
			int k = 1;
			char c = word[0];
			int io = 0;
			str[io] = (char)-64; io++;
			for (int i = 1; i < len; i++) {
				char c = word[i];
				bool b = false;
				for (int j = 0; j < i; j++) {
					if (word[j] == c) { b = true; str[io] = str[j]; io++; break; }
				}
				if (!b) {
					str[io] = (char)-64 + k; io++;
					k++;
				}
			}
		}
		void countLetters(int* cnts, char* word, char* stand, int len, int nSym) { //������� ��������� ���� � �����
			for (int i = 0; i < nSym; i++) {
				cnts[i] = count(word, word + strlen(word), stand[i]);
			}
		}
		void compLetters(int* inc, char* word, char* stand, int len, int nSym) { //�������� ����������� ���� � �����
			for (int i = 0; i < nSym; i++) {
				if (strchr(word, stand[i]) != NULL) inc[i] = 1;
				else inc[i] = 0;
			}
		}

		double compWords(char* opword, char* dicword, char* stand, int *tru, int len, int nSym) {//��������� ����� �� ��������� (0-1) 1 - ��������� ���������
			double eq = 0;
			for (int i = 0; i < nSym; i++) tru[i] = 0;
			for (int i = 0; i < len; i++) {
				if (opword[i] == dicword[i]) { eq += 1; tru[strchr(stand, opword[i]) - stand] = 1; } 
			}
			eq /= len;
			return eq;
		}
		bool comStr(char* first, char* second, int len) {
			bool eq = true;
			for (int i = 0; i < len; i++) {
				if (first[i] != second[i]) { eq = false; break; }
			}
			return eq;
		}

	};

	

	f f; //init functions
	errno_t err;
	//char buffer[1048576];
	char buffer[1024];
	int nFrag = 0, nSol = 0;
	set<int> nums;
	Dict dictionary[LW];



	if ((err = fopen_s(&cfg, CGF.c_str(), "r")) != 0) {
		cout << "Cannot open enU file.\n";
		return 1;
	}
	if ((err = fopen_s(&input, NIF.c_str(), "r")) != 0) {
		cout << "Cannot open ouR file.\n";
		return 1;
	}
	

	//��������� ������
	int nLetters = 0;
	fscanf_s(cfg, "%i\t%*i\n", &nLetters); //nLetters - ���������� ���� � ��������
	char standart[LA]; //������ � ��������
	for (int k = 0; k < nLetters; k++) {
		char c;
		fscanf_s(cfg, "%c\t", &c, 1);
		standart[k] = c;
	}
	if (N == 0) { nums.insert(1); }
	else
	for (int k = 0; k < N; k++) {
		nums.insert(M[k]); // ��������� � �������� �������, ������� ����� ���������
	}

	//�������� �������
	fopen_s(&output, NOF.c_str(), "w");
	fopen_s(&tmp, TMF.c_str(), "w");
	fscanf_s(input, "%i %i\n", &nFrag, &nSol); //��������� ���������� ���������� � ���-�� �������
	fprintf_s(tmp, "%i %i\n", nFrag, nSol + 1);
	
	for (int frag = 0; frag < nFrag; frag++) {

		system("cls");
		std::cout << frag + 1 << "/" << nFrag;
		vector<Solution> uniqSols;
		int nfrg = 0;
		fscanf_s(input, "_%i_\n", &nfrg);
		fprintf_s(tmp, "_%i_\n", nfrg);
		//if (nfrg != frag) { frag++; fprintf_s(tmp, "_%i_\n", nfrg);	}
		int cntUS = 0;
		for (int s = 0; s < nSol; s++) {
			//���������� �������
			char ch;
			Solution currSol;
			for (int ic = 0; ic < nLetters; ic++) {
				ch = fgetc(input);
				currSol.abc[ic] = ch;
			}
			ch = fgetc(input);
			fscanf_s(input, "%i\t%i", &currSol.n, &currSol.nT);
			ch = fgetc(input);
			for (int ic = 0; ic < 6; ic++) {
				ch = fgetc(input);
				currSol.met[ic] = ch;
			}
			ch = fgetc(input);
			f.printSol(tmp, currSol, nLetters);
			//
			//����� � �������� ���������� ������� (�������� ������ ����������)
			if (nums.count(currSol.n) != 0) { //���� �� ������� � ����� ������� � ������ �� ��������
				bool isEqual = false;
				int inS = 0;
				while (!isEqual && cntUS != 0 && inS < cntUS) {
					for (int ic = 0; ic < nLetters; ic++) {
						if (currSol.abc[ic] != uniqSols[inS].abc[ic]) { break; }
						if (ic == nLetters - 1) isEqual = true;
					}
					inS++;
				}
				if (!isEqual) {
					uniqSols.push_back(currSol);
					cntUS++;
				}
				else isEqual = false;
			}
		}
		Solution min;
		Context minCont;
		Context minCover;
		//�� ������ ����� ����������� ������ �� �������, ����� ������ �������
		for (int iSol = 0; iSol < cntUS; iSol++) {
			Context cont;
			Context cov;
			// ���������� ��������� �����
			if ((err = fopen_s(&wrds, IWF.c_str(), "r")) != 0) {
				cout << "Cannot open inW file.\n";
				return 1;
			}
			int nFragW;
			fscanf_s(wrds, "%d\n", &nFragW);
			if (nFragW > nFrag) nFrag = nFragW; //��������� ���������� ���������� � ��������� ����� � ��������
			fscanf_s(wrds, "%*s\n");
			int nWords = 0;
			int nLens = 0;
			fscanf_s(wrds, "%*d %d %d %*d\n", &nWords, &nLens); //���������� ���� � ���� �� ���������
			for (int iL = 0; iL < nLens; iL++) { //���� ����
				int currLn = 0;
				int nwLn = 0;
				fscanf_s(wrds, "%d %d %*d %*d\n", &currLn, &nwLn); //�����, ���-�� ����
				for (int iW = 0; iW < nwLn; iW++) { //���� ���� � �����
					char closeWord[LW];
					char c;
					int iwc = 0;
					double points = 0;
					while ((c = fgetc(wrds)) != ' ') { closeWord[iwc] = c; iwc++; }//�����
					while ((c = fgetc(wrds)) != ' ') {}//������� (�� �����)
					//�����
					Word openWord;
					f.decode(openWord.abc, closeWord, uniqSols[iSol], standart, currLn); //������������� � ������� �������
					char strWord[LW] = "";
					f.toStruct(strWord, openWord.abc, currLn); //���������� ���������
					int cntLet[LA];
					f.countLetters(cntLet, openWord.abc, standart, currLn, nLetters); //���������� ������ �����

					//����� ����� � �������
					string DICWF = DIC + "\\w" + to_string(currLn) + ".txt"; //�������� ����� ������ �����
					if ((err = fopen_s(&wl, DICWF.c_str(), "r")) != 0) {
						//���� ����� ����� ��� � �������
						points = 0;
					}
					else {
						if (!dictionary[currLn].isThere) {
							f.readDictionary(wl, dictionary[currLn]);
						}

						bool found = false;
						Struct currStr;
						for (int iStr = 0; iStr < dictionary[currLn].nStructs; iStr++) { //����� ��������� � �������
							 //���� ��������� ���������
							if (f.comStr(dictionary[currLn].structs[iStr].abc, strWord, currLn)) {
								currStr = dictionary[currLn].structs[iStr];
								found = true;
								break;
							}
						}
						if (found) { //���� ��������� �������
							int cmpLet[LA];
							Word bestWord;
							double maxMatch = 0;
							int nxt = currStr.first;
							for (int iW = 0; iW < dictionary[currLn].nWords; iW++) { //���� � ������ ������ ����������
								if (iW == nxt) {
									nxt = dictionary[currLn].words[iW].next;
									if (maxMatch != 1) {
										int cLet[LA];
										double sc = f.compWords(openWord.abc, dictionary[currLn].words[iW].abc, standart, cLet, currLn, nLetters);
										if (sc > maxMatch) {
											maxMatch = sc; bestWord = dictionary[currLn].words[iW]; memcpy(cmpLet, cLet, sizeof(cLet));
										}
									}

								}
							}


							//cmpLet = f.compLetters(bestWord.abc, standart, currLn, nLetters); //����������� ������ ����� � ������ �����
							//������� ����� ��� �����
							for (int i = 0; i < nLetters; i++) {
								points += cntLet[i] * cmpLet[i]; // ����� 
							}
							points /= currLn; // ������ ������� ��� �����
							cont.words.push_back(bestWord); // ���������� ����� � ��������
							cov.words.push_back(openWord);
							cont.lens.push_back(currLn);
							cov.lens.push_back(currLn);

							for (int ic = 0; ic < nLetters; ic++) { fprintf_s(output, "%i ", cntLet[ic]); }
							fprintf_s(output, "\n");
							for (int ic = 0; ic < nLetters; ic++) { fprintf_s(output, "%i ", cmpLet[ic]); }
							fprintf_s(output, "\n");
							for (int ic = 0; ic < nLetters; ic++) { fprintf_s(output, "%c ", standart[ic]); }
							fprintf_s(output, "\n");

							for (int ic = 0; ic < currLn; ic++) { fprintf_s(output, "%c ", bestWord.abc[ic]); }
							fprintf_s(output, "\n");

							for (int ic = 0; ic < currLn; ic++) { fprintf_s(output, "%c ", openWord.abc[ic]); }
							fprintf_s(output, "\n");

							fprintf_s(output, " %lf - points", points);
							fprintf_s(output, "\n");

						}
						else {

							fprintf_s(output, "���� � ����� ���������� ���\n");
							for (int ic = 0; ic < currLn; ic++) { fprintf_s(output, "%c", strWord[ic]); }
							fprintf_s(output, "\n");

							points = 0;
						}


					}
					fclose(wl);
					//uniqSols[iSol].cont = cont;
					uniqSols[iSol].score += points; //��������� ���������� ����� ��� ������
					if (uniqSols[iSol].score > min.score) { min = uniqSols[iSol]; minCont = cont; minCover = cov; }
				}
			}
			fclose(wrds);
		}



		min.met[0] = ' '; min.met[1] = '['; min.met[2] = 'D'; min.met[3] = 'I'; min.met[4] = '�'; min.met[5] = ']';
		min.n = nSol + 1;
		min.nT = 0;
		fprintf_s(output, "Solution score = %lf \n", min.score);
		//������ ���������
		for (auto const& wrd : minCont.words) {
			fprintf_s(output, "%s ", wrd.abc);
		}

		fprintf_s(output, "\n");
		for (auto const& wrd : minCover.words) {
			fprintf_s(output, "%s ", wrd.abc);
		}

		Nw nw = f.checkCont(minCover, minCont, min.abc);
		fprintf_s(output, "\n");
		for (int i = 0; i < LA; i++) {
			fprintf_s(output, "%i ", nw.ch[i].size());
		}
		fprintf_s(output, "\n");
		for (int i = 0; i < LA; i++) {
			if (nw.ch[i].size() != 0) fprintf_s(output, "%c ", nw.ch[i][0]);
		}

		int k2 = 0;
		for (int i = 0; i < LA; i++) {
			k2 += nw.ch[i].size();
		}
		k2 /= LA;

		if (k2 == 1) {
			for (int i = 0; i < LA; i++) {
				min.abc[i] = nw.ch[i][0];
			}
		}

		//�����
		fprintf_s(output, "\n");
		fprintf_s(output, "_%i_\n", frag + 1);
		fprintf_s(output, "%i\n", cntUS);
		for (int ic = 0; ic < nLetters; ic++) { fprintf_s(output, "%c", standart[ic]); }
		fprintf_s(output, "\n");

		for (int s = 0; s < cntUS; s++) {
			f.printSol(output, uniqSols[s], nLetters);
		}
		f.printSol(output, min, nLetters);

		f.printSol(tmp, min, nLetters);
	
	}

	fclose(input);
	fclose(output);
	fclose(cfg);
	fclose(tmp);
	//������� ������ �� ���������� �����
	/*
	fopen_s(&tmp, TMF.c_str(), "r");
	fopen_s(&input, NIF.c_str(), "w");
	char cc[256];
	while (!feof(tmp))
	{
		fgets(cc, sizeof(cc), tmp);
		fputs(cc, input);
	}
	fclose(input);
	fclose(tmp);
	remove(TMF.c_str());
	*/
	return 0;
}
int main() {
	int M[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	DI�(7, M);
	std::cout << std::endl << "runtime = " << clock() / 1000.0 << "seconds" << std::endl; // ����� ������ ���������                  
	system("pause");
	return 0;
}
