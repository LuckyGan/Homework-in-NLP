/*
 * ���ߣ�MF1733012 ������
 * ��棺��Ȼ���Դ���γ�Project2�������Զ��ִ�ʵ�֣�
 * ʱ�䣺2017-11-04
 */
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
using std::ifstream;
using std::multiset;
using std::string;
using std::vector;
using std::reverse;
using std::cout;
using std::endl;
using std::cin;

using compareType = bool(*)(const string&, const string&);
/*
 * multiset������,�����ﳤ����������
 */
bool compareWordSize(const string &lhs, const string &rhs) {
	return lhs.size() > rhs.size();
}

/*
 * ��ʼ������multiset,���ݴ��ﳤ����������
 */
void initWordSet(multiset<string, compareType> &wordSet) {
	ifstream ifs("ce��ms-word��.txt");
	if (!ifs) {
		cout << "open file error!" << endl;
		exit(1);
	}
	string line;
	string::size_type end;
	while (getline(ifs, line)) {
		end = line.find_first_of(",");
		wordSet.insert(line.substr(0, end));
	}
	ifs.close();
}

/*
 * ��������(FMM)ȡ��Ĵ�
 */
void FMM(vector<string> &fmmVec, const multiset<string, compareType> &wordMultiSet, const string &sentence) {
	multiset<string, compareType>::const_iterator iter;
	string::size_type begin = 0;
	while (1) {
		iter = wordMultiSet.begin();
		bool isInsert = false;
		while (iter != wordMultiSet.end()) {
			if (begin < sentence.size() && sentence.substr(begin, iter->size()) == *iter) {
				isInsert = true;
				fmmVec.push_back(*iter);
				begin += iter->size();
				iter = wordMultiSet.begin();
				continue;
			}
			++iter;
		}
		if (!isInsert) {
			if(begin < sentence.size())
				fmmVec.push_back(sentence.substr(begin));
			break;
		}
	}
}

/*
 * ��������(RMM)ȡ��Ĵ�
 */
void RMM(vector<string> &rmmVec, const multiset<string, compareType> &wordMultiSet, const string &sentence) {
	multiset<string, compareType>::const_iterator iter;
	string::size_type begin = sentence.size();
	while (1) {
		iter = wordMultiSet.begin();
		bool isInsert = false;
		while (iter != wordMultiSet.end()) {
			if (begin - iter->size() < sentence.size() && sentence.substr(begin - iter->size(), iter->size()) == *iter) {
				isInsert = true;
				rmmVec.push_back(*iter);
				begin -= iter->size();
				iter = wordMultiSet.begin();
				continue;
			}
			++iter;
		}
		if (!isInsert) {
			if (begin > 0)
				rmmVec.push_back(sentence.substr(0, begin));
			break;
		}
	}
}

/*
 * ��ʽ����ӡVector
 */
void displayVec(const vector<string> &svec, string segType) {
	cout << "-----------------  " << segType << "  -----------------" << endl;
	bool isFirst = true;
	for (const auto &elem : svec) {
		if (isFirst) {
			cout << elem;
			isFirst = false;
		} else {
			cout << " / " << elem;
		}
	}
	cout << endl;
	cout << "-----------------  " << segType << "  -----------------" << endl;
}

/*
 * ��ʽ����ӡ���ӷִʽ��
 */
void displayConclusion(const vector<string> &fmmVec, const vector<string> &rmmVec) {
	cout << "--------------  " << "Conclusion" << "  --------------" << endl;
	if (fmmVec == rmmVec)
		cout << "�þ��ӷִʳɹ���" << endl;
	else
		cout << "�þ��ӷִ�ʧ�ܣ�" << endl;
	cout << "--------------  " << "Conclusion" << "  --------------" << endl;
}

int main() {
	multiset<string, compareType> wordMultiSet(compareWordSize);
	initWordSet(wordMultiSet);
	cout << "�����벻���κα����ż��ո�����ľ��ӣ�" << endl;
	string sentence;
	vector<string> fmmVec, rmmVec;
	while (cin >> sentence) {
		fmmVec.clear();
		rmmVec.clear();
		FMM(fmmVec, wordMultiSet, sentence);
		displayVec(fmmVec, "FMM");
		RMM(rmmVec, wordMultiSet, sentence);
		reverse(rmmVec.begin(), rmmVec.end());
		displayVec(rmmVec, "RMM");
		displayConclusion(fmmVec, rmmVec);
	}
	return 0;
}
